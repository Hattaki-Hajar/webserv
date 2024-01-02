#include "Cgi.hpp"

std::string generateUUID() {
    // Seed for the random number generator
    std::srand(static_cast<unsigned>(time(0)));

    const char hex_chars[] = "0123456789abcdef";

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

Cgi::Cgi(){}

Cgi::Cgi(std::map<std::string, std::string> config_info, std::map<std::string, std::string> headers, Response *response)
:_response(response)   {
    this->_extension_map = config_info;
	this->is_complete = false;
	this->is_timeout = false;
	setup_env(headers);
}

Cgi::~Cgi() {
}

void	Cgi::setup_env(std::map<std::string, std::string> headers)
{
	_env = new char*[headers.size()];
	std::string	temp;
	// this->_headers["AUTH_TYPE"] = "";
    // this->_headers["CONTENT_LENGTH"] = headers["Content-Length"];
    // this->_headers["CONTENT_TYPE"] = headers["Content-Type"];
    // this->_headers["PATH_INFO"] = this->_response->get_old_uri();
    // this->_headers["PATH_TRANSLATED"] = this->_response->get_uri();
    // // this->_headers["QUERY_STRING"] = _client.req.query;
    // this->_headers["REMOTE_HOST"] = headers["Host"];
    // this->_headers["REMOTE_USER"] = "";
    // // this->_headers["HTTP_COOKIE"] = _client.req.header_map["Cookie"];;
    // this->_headers["REMOTE_IDENT"] = "";
    // this->_headers["REQUEST_METHOD"] = this->_response->get_method();
    // this->_headers["GATEWAY_INTERFACE"] = "CGI/1.1";
    // this->_headers["SCRIPT_NAME"] = th.req.current_location.cgi_path;
    // // this->_headers["SCRIPT_FILENAME"] = _client.req.current_location.root + "/" + _client.req.current_location.cgi_path;
    // this->_headers["SERVER_NAME"] = "";
    // this->_headers["SERVER_PROTOCOL"] = "HTTP/1.1";
    // this->_headers["SERVER_PORT"] = std::to_string(_client.server_ptr->port);
    // this->_headers["SERVER_SOFTWARE"] = "webserv/1.0";
    // this->_headers["REDIRECT_STATUS"] = "200";
    // this->_headers["REQUEST_SCHEME"] = "http";
    // this->_headers["HTTPS"] = "off";
    // this->_headers["SERVER_SIGNATURE"] = "webserv/1.0";
	int i = 0;
	std::map<std::string, std::string>::iterator it;
	for (it = this->_headers.begin(); it != this->_headers.end(); it++)
	{
		temp = (it->first) + "=" + (it->second);
        std::cout << temp << std::endl;
		_env[i] = strdup(temp.c_str());
		i++;
	}
}

// void	Cgi::run(const std::string &path, const std::string &bin)
// {
// 	// const char 	*av[3];
// 	// std::string	file_name = generateUUID();
// 	// this->_outfile = open(file_name.c_str(), O_CREAT | O_RDWR);
// 	// dup2(this->_outfile, 1);
// 	// close(this->_outfile);
// 	// av[0] = bin.c_str();
// 	// av[1] = this->_response.get_uri().c_str();
// 	// av[2] = 0;
// 	// execve(bin.c_str(), (char *const *)av, (char *const *)this->_env);
// 	// exit(500);
// }

// void	Cgi::php_setup(int infile)
// {
// 		std::map<std::string, std::string>::iterator	it = this->_extension_map.find("php");
// 	if (it == this->_extension_map.end())
// 	{
// 		throw std::runtime_error("No extension!");
// 		return ;
// 	}
// 	else
// 	{
// 		int check = access(((*it).second).c_str(), F_OK && X_OK);
// 		if (check)
// 		{
// 			throw std::runtime_error("No binary!");
// 			return ;
// 		}
// 		pipe(this->_fd);
// 		this->_pid = fork();
// 		if (!this->_pid)
// 		{
// 			if (infile)
// 			{
// 				dup2(infile, 0);
// 				close(infile);
// 			}
// 			run((*it).second, "php");
// 		}
// 	}
// }

// void	Cgi::py_setup(int infile)
// {
// 	std::map<std::string, std::string>::iterator	it = this->_extension_map.find("py");
// 	if (it == this->_extension_map.end())
// 	{
// 		throw std::runtime_error("No extension!");
// 		return ;
// 	}
// 	else
// 	{
// 		int check = access(((*it).second).c_str(), F_OK && X_OK);
// 		if (check)
// 		{
// 			throw std::runtime_error("No binary!");
// 			return ;
// 		}
// 		pipe(this->_fd);
// 		this->_pid = fork();
// 		if (!this->_pid)
// 		{
// 			if (infile)
// 			{
// 				dup2(infile, 0);
// 				close(infile);
// 			}
// 			run((*it).second, "python");
// 		}
// 	}
// }