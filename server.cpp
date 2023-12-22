#include "server.hpp"

server::server(int port)
{
	_port = port;
	_name = "no_name";
	_addr.sin_family = AF_INET;
	_socketaddr_len = sizeof(_addr);
}

server::server(const std::string &name)
{
	_name = name;
	_addr.sin_family = AF_INET;
	_socketaddr_len = sizeof(_addr);
}

server::server()
{}

server::~server() {
	close(_socket);
}
	/* additional functions  */
void	server::bind_server()
{
	if (bind(_socket,(sockaddr *)&_addr, _socketaddr_len) < 0)
		throw std::runtime_error("bind failed!");
	start_listen();
	acceptconnection(_newSocket);
	char	buffer[BUFFER_SIZE] = {0};
	ssize_t	bytesReceived = read(_newSocket, buffer, BUFFER_SIZE);
	if (bytesReceived < 0)
		throw std::runtime_error("read failed!");
	std::cout << buffer << std::endl;
	long unsigned int bytesSent;
	bytesSent = write(_newSocket, _message.c_str(), _message.size());
	if (bytesSent != _message.size())
		throw std::runtime_error("write failed!");
}

void	server::start_listen()
{
	if (listen(_socket, SOMAXCONN) < 0)
		throw std::runtime_error("listen failed!");
	std::ostringstream ss;
    ss << "\n*** Listening on ADDRESS: " 
        << inet_ntoa(_addr.sin_addr) 
        << " PORT: " << ntohs(_addr.sin_port) 
        << " ***\n\n";
    std::cout << ss.str();
}

void	server::acceptconnection(int &new_socket)
{
	new_socket = accept(_socket, (sockaddr *)&_addr, 
                        &_socketaddr_len);
    if (new_socket < 0)
    {
        std::ostringstream ss;
        ss << 
        "Server failed to accept incoming connection from ADDRESS: " 
        << inet_ntoa(_addr.sin_addr) << "; PORT: " 
        << ntohs(_addr.sin_port);
        throw std::runtime_error("failed to connect!");
    }
}
	/*  getters  */
const std::string	&server::get_name(void) const {
	return (_name);
}

unsigned int	server::get_socketaddr_len() const {
	return (_socketaddr_len);
}

const sockaddr_in	&server::get_addr() const {
	return (_addr);
}

int	&server::get_new_socket() {
	return (_newSocket);
}

const std::string	&server::get_root() const {
	return (_root_path);
}
const std::string	&server::get_ip() const {
	return (_ip);
}

int	server::get_port() const {
	return (_port);
}

const std::map<std::string, location>					&server::get_location() const {
	return (_locations);
}

const std::map<std::string, location>::const_iterator	server::get_location_begin_iter() const {
	return (_locations.begin());
}

const std::map<std::string, location>::const_iterator	server::get_location_end_iter() const {
	return (_locations.end());
}
	/*  setters  */
void	server::set_port(int port) {
	_port = port;
}

void	server::set_host(const std::string &host) {
	_ip = host;
}

void	server::set_name(const std::string &name) {
	_name = name;
}

void	server::set_error_page(int error, std::string const& path) {
	_error_pages[error] = path;
}

void	server::set_body_size(size_t size) {
	_max_body_size = size;
}

void	server::set_new_socket(int socket) {
	_newSocket = socket;
}

void	server::set_socket(int socket) {
	_socket = socket;
}

void	server::set_len(void) {
	_socketaddr_len = sizeof(_addr);
}

void	server::set_addr(int port, std::string const& ip)
{
	_port = port;
	_ip = ip;
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = inet_addr(_ip.c_str());
	_addr.sin_port = htons(_port);
}

void	server::set_root(std::string const &path) {
	_root_path = path;
}

void	server::set_location(std::string const &path, location &loc)
{
	_locations[path].root = loc.root;
	_locations[path].autoindex = loc.autoindex;
	_locations[path].index = loc.index;
	_locations[path].max_body_size = loc.max_body_size;
	_locations[path].return_code = loc.return_code;
	_locations[path].return_path = loc.return_path;
	_locations[path].methods = loc.methods;
}
	/*  << overload for server  */
std::ostream& operator<<(std::ostream &os, const server& s)
{
	os << "server name: " << s.get_name() << std::endl;
	os << "server port: " << s.get_port() << std::endl;
	os << "server ip: " << s.get_ip() << std::endl;
	os << "server root: " << s.get_root() << std::endl;
	std::map<std::string, location>::const_iterator it = s.get_location_begin_iter();
	std::map<std::string, location>::const_iterator it_end = s.get_location_end_iter();
	while (it != it_end)
	{
		// std::cout << "location path: " << it->first << std::endl;
		os << "location path: " << it->first << std::endl;
		os << "location root: " << it->second.root << std::endl;
		os << "location autoindex: " << it->second.autoindex << std::endl;
		os << "location index: " << it->second.index << std::endl;
		os << "location max_body_size: " << it->second.max_body_size << std::endl;
		os << "location return_code: " << it->second.return_code << std::endl;
		os << "location return_path: " << it->second.return_path << std::endl;
		std::vector<std::string>::const_iterator it2 = it->second.methods.begin();
		std::vector<std::string>::const_iterator it2_end = it->second.methods.end();
		while (it2 != it2_end)
		{
			os << "location methods: " << *it2 << std::endl;
			it2++;
		}
		it++;
	}
	return (os);
}
