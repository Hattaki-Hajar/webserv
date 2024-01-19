#include "Cgi.hpp"

std::string generate_file_name() {
    // Seed for the random number generator
    std::srand(static_cast<unsigned>(time(0)));

    const char hex_chars[] = "abcdef0123456789";

    std::string uuid;

    // Generate 32 random hexadecimal characters
    for (int i = 0; i < 32; ++i) {
        uuid += hex_chars[std::rand() % 16];
    }

    // Insert hyphens at appropriate positions
    uuid.insert(8, 1, '-');
    uuid.insert(13, 1, '-');
    uuid.insert(18, 1, '-');
    uuid.insert(23, 1, '-');

    return uuid;
}

Cgi::Cgi(): _outfile(-1), _pid(-1){}

Cgi::Cgi(std::map<std::string, std::string> headers, Response *response)
:_response(response)   {
    this->_extension_map = _response->get_location().cgi;
	this->is_complete = false;
	this->is_timeout = false;
	this->is_running = false;
	this->_pid = -1;
	setup_env(headers);
}

Cgi::~Cgi() {
	// std::cout << "cgi destructor" << std::endl;
	delete [] _env;
}

int Cgi::get_outfile() const {
    return this->_outfile;
}

void	Cgi::setup_env(std::map<std::string, std::string> headers)
{
	std::string	temp;

	if (this->_response->get_method() == "POST")
		this->_headers["CONTENT_LENGTH"] = headers["Content-Length"];
    this->_headers["CONTENT_TYPE"] = headers["Content-Type"];
    this->_headers["PATH_INFO"] = this->_response->get_old_uri();
    this->_headers["PATH_TRANSLATED"] = this->_response->get_uri();
    this->_headers["QUERY_STRING"] = this->_response->get_query();
    this->_headers["REMOTE_HOST"] = headers["Host"];
    this->_headers["REMOTE_USER"] = "";
	this->_headers["SCRIPT_FILENAME"] = this->_response->get_uri();
    this->_headers["HTTP_COOKIE"] = headers["Cookie"];
    this->_headers["REQUEST_METHOD"] = this->_response->get_method();
    this->_headers["GATEWAY_INTERFACE"] = "CGI/1.1";
    // this->_headers["SCRIPT_NAME"] = th.req.current_location.cgi_path;
    // this->_headers["SCRIPT_FILENAME"] = _client.req.current_location.root + "/" + _client.req.current_location.cgi_path;
    this->_headers["SERVER_NAME"] = "";
    this->_headers["SERVER_PROTOCOL"] = "HTTP/1.1";
    std::stringstream ss;
    ss << _response->get_client()->get_server().get_port();
    std::string tmp;
    ss >> tmp;
    this->_headers["SERVER_PORT"] = tmp;
    this->_headers["SERVER_SOFTWARE"] = "webserv/1.0";
    this->_headers["REDIRECT_STATUS"] = "200";
    this->_headers["REQUEST_SCHEME"] = "http";
    this->_headers["HTTPS"] = "off";
    this->_headers["SERVER_SIGNATURE"] = "webserv/1.0";
	_env = new char*[_headers.size() + 1];
	int i = 0;
	std::map<std::string, std::string>::iterator it;
	for (it = this->_headers.begin(); it != this->_headers.end(); it++, i++)
	{
		temp = (it->first) + "=" + (it->second);
		_env[i] = const_cast<char*>((new std::string(temp))->c_str());
	}
    _env[i] = 0;
}

void	Cgi::run(const std::string &bin)
{
	const char 	*av[3];

	// std::string tmp = this->_response->get_uri();
	// int i = tmp.rfind('/');
	// tmp = tmp.substr(0, i);
	// chdir(tmp.c_str());
	if (_fd != -1)
	{
		dup2(_fd, 0);
		close(_fd);
	}
	dup2(this->_outfile, 1);
    close(this->_outfile);
    av[0] = bin.c_str();
    av[1] = this->_response->get_uri().c_str();
    av[2] = 0;
	if (access(av[1], F_OK) == -1)
	{
		std::cerr << "file not found" << std::endl;
		exit(-1);
	}
    execve(bin.c_str(), (char *const *)av, this->_env);
    exit(-1);
}

void	Cgi::php_setup(const std::string &file_path)
{
	// std::cout << "file_path:[" << file_path << "]" << std::endl;
	std::cout << "php setup" << std::endl;
	std::map<std::string, std::string>::iterator    it = this->_extension_map.find("php");
	int check = access((it->second).c_str(), F_OK && X_OK);
	if (check)
	{
		this->_response->set_status_code(500);
		return ;
	}
	std::string	file_name = generate_file_name();
    std::string tmp = "/nfs/homes/";
    tmp += USER;
    tmp += "/.cache/cgi"; 
	file_name = tmp + file_name;
	this->_response->set_file_name(file_name);
	// std::cout << file_name << std::endl;
	// umask(0);
	this->_outfile = open(file_name.c_str(), O_CREAT | O_RDWR, 0666);
	if (_outfile == -1) {
		std::cerr << "file not open" << std::endl;
		this->_response->set_status_code(403);
		return;
	}
	if (!file_path.empty())
	{
		// std::cout << "file path = " << file_path << std::endl;
		_fd = open(file_path.c_str(), O_RDONLY, 0666);
		if (_fd == -1) {
			_response->set_status_code(500);
			return;
		}
	}
	else
		_fd = -1;
	this->is_running = true;
	this->_start = clock();
	this->_pid = fork();
	if (!this->_pid)
		run(it->second);
}

void	Cgi::py_setup(const std::string &file_path)
{
	// std::cout << "py setup" << std::endl;
	std::map<std::string, std::string>::iterator    it = this->_extension_map.find("py");
	int check = access((it->second).c_str(), F_OK && X_OK);
	if (check)
	{
		this->_response->set_status_code(500);
		return ;
	}
	std::string	file_name = generate_file_name();
    std::string tmp = "/nfs/homes/";
    tmp += USER;
    tmp += "/.cache/cgi"; 
	file_name = tmp + file_name;
	// std::cout << file_name << std::endl;
	_response->set_file_name(file_name);
	// umask(0);
	this->_outfile = open(file_name.c_str(), O_CREAT | O_RDWR, 0666);
	if (_outfile == -1) {
		std::cerr << "file not open" << std::endl;
		_response->set_status_code(500);
		return;
	}
	if (!file_path.empty())
	{
		_fd = open(file_path.c_str(), O_RDONLY, 0666);
		if (_fd == -1) {
			_response->set_status_code(500);
			return;
		}
	}
	else
		_fd = -1;
	this->is_running = true;
	this->_start = clock();
	this->_pid = fork();
	if (!this->_pid)
		run("/usr/bin/python3");
}