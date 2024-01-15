#include "Server.hpp"

Server::Server(int port)
{
	_port = port;
	_name = "";
	_root_path = "/nfs/homes/";
	_root_path += USER;
	_root_path += "/www";
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_socketaddr_len = sizeof(_addr);
	_event = new epoll_event;
	_is_bound = false;
}
Server::Server()
{
	_port = 8080;
	_ip = "127.0.0.1";
	_name = "";
	_root_path = "/nfs/homes/";
	_root_path += USER;
	_root_path += "/www";
	_max_body_size = -1;
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_socketaddr_len = sizeof(_addr);
	_event = new epoll_event;
	_is_bound = false;
}
Server::Server(Server const &s)
{
	_socket = s._socket;
	_epfd = s._epfd;
	_ip = s._ip;
	_port = s._port;
	_name = s._name;
	_max_body_size = s._max_body_size;
	_root_path = s._root_path;
	_error_pages = s._error_pages;
	_locations = s._locations;
	_event = 0;
	// _event = new epoll_event;
	// _event->events = s._event->events;
	// _event->data.fd = s._event->data.fd;
	_addr = s._addr;
	_socketaddr_len = s._socketaddr_len;
}
Server::~Server() {
	delete _event;
	close(_socket);
}
	/* additional functions  */
int set_nonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        throw std::runtime_error("fcntl failed 1!");
	}
	flags |= O_NONBLOCK;
    if (fcntl(sockfd, F_SETFL, flags) == -1)
        throw std::runtime_error("fcntl failed 2!");
    return 0;
}
void	Server::bind_Server()
{
	int on = 1;
	this->_addr.sin_port = htons(this->_port);
	ip_to_in_addr_t();
	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket < 0)
		throw std::runtime_error("socket failed!");
	setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    setsockopt(this->_socket, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
	if (bind(this->_socket,(sockaddr *)&this->_addr, this->_socketaddr_len) < 0)
		throw std::runtime_error("bind failed!");
	if (set_nonblocking(this->_socket) < 0)
		throw std::runtime_error("set_nonblocking failed!");
	this->_is_bound = true;
}
void	set_up_Server(Server &s, int epfd)
{
	if (epfd < 0)
		throw std::runtime_error("epoll_create1 failed!");
	if (!s._is_bound)
		return ;
	s.set_epoll_fd(epfd);
	s.get_event()->events = EPOLLIN;
	s.get_event()->data.fd = s.get_socket();
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, s.get_socket(), s.get_event()) < 0)
		throw std::runtime_error("epoll_ctl failed! 1");
	if (listen(s.get_socket(), SOMAXCONN) == -1)
		throw std::runtime_error("listen failed!");
}
void	Server::ip_to_in_addr_t()
{
	std::stringstream ss(this->_ip);
	std::vector<int> addr;
	std::string nb;

	while (std::getline(ss, nb, '.'))
		addr.push_back(atoi(nb.c_str()));
	this->_addr.sin_addr.s_addr = htonl((addr[0] << 24) | (addr[1] << 16) | (addr[2] << 8) | addr[3]);
}
	/*  getters  */
const std::string	&Server::get_name(void) const {
	return (_name);
}

unsigned int	&Server::get_socketaddr_len() {
	return (_socketaddr_len);
}

const sockaddr_in	&Server::get_addr() const {
	return (_addr);
}

int	Server::get_socket() {
	return (_socket);
}

int	Server::get_epfd() const
{
	return (_epfd);
}
int	Server::get_body_size() const {
	return (_max_body_size);
}
const std::string	&Server::get_root() const {
	return (_root_path);
}
const std::string	&Server::get_index() const {
	return (_index_path);
}
const std::string	&Server::get_ip() const {
	return (_ip);
}
int	Server::get_port() const {
	return (_port);
}
epoll_event	*Server::get_event() {
	return (_event);
}
long	Server::get_max_body_size() const {
	return (_max_body_size);
}

const std::map<std::string, location>::const_iterator	Server::get_location_begin_iter() const {
	return (_locations.begin());
}
const std::map<std::string, location>::const_iterator	Server::get_location_end_iter() const {
	return (_locations.end());
}
const std::map<std::string, location>	&Server::get_locations() const {
	return (_locations);
}
const std::map<int, std::string>	&Server::get_error_pages() const {
	return (_error_pages);
}
	/*  setters  */
void	Server::set_port(int port) {
	_port = port;
}

void	Server::set_epoll_fd(int epfd) {
	_epfd = epfd;
}

void	Server::set_host(const std::string &host) {
	_ip = host;
}

void	Server::set_name(const std::string &name) {
	_name = name;
}

void	Server::set_error_page(int error, std::string const& path) {
	_error_pages[error] = path;
}

void	Server::set_body_size(size_t size) {
	_max_body_size = size;
}

void	Server::set_socket(int socket) {
	_socket = socket;
}

void	Server::set_len(void) {
	_socketaddr_len = sizeof(_addr);
}

void	Server::set_addr(int port, std::string const& ip)
{
	_port = port;
	_ip = ip;
	_addr.sin_family = AF_INET;
	ip_to_in_addr_t();
	_addr.sin_port = htons(_port);
}

void	Server::set_root(std::string const &path) {
	_root_path = path;
}

void	Server::set_index(std::string const &path) {
	_index_path = path;
}

void	Server::set_location(std::string const &path, location &loc)
{
	_locations[path].root = loc.root;
	_locations[path].autoindex = loc.autoindex;
	_locations[path].index = loc.index;
	_locations[path].max_body_size = loc.max_body_size;
	_locations[path].return_code = loc.return_code;
	_locations[path].return_path = loc.return_path;
	_locations[path].methods = loc.methods;
	_locations[path].upload_path = loc.upload_path;
	_locations[path].cgi = loc.cgi;
}
	/*  << overload for Server  */
std::ostream& operator<<(std::ostream &os, const Server& s)
{
	os << "Server name: " << s.get_name() << std::endl;
	os << "Server port: " << s.get_port() << std::endl;
	os << "Server ip: " << s.get_ip() << std::endl;
	os << "Server root: " << s.get_root() << std::endl;
	os << "body_size: " << s.get_body_size() << std::endl;
	std::map<std::string, location>::const_iterator it = s.get_location_begin_iter();
	std::map<std::string, location>::const_iterator it_end = s.get_location_end_iter();
	while (it != it_end)
	{
		// std::cout << "location path: " << it->first << std::endl;
		std::cout << "******** " << "location" << " ********" << std::endl;
		os << "location path: " << it->first << std::endl;
		os << "location root: " << it->second.root << std::endl;
		os << "location autoindex: " << it->second.autoindex << std::endl;
		os << "location index: " << it->second.index << std::endl;
		os << "location max_body_size: " << it->second.max_body_size << std::endl;
		os << "location return_code: " << it->second.return_code << std::endl;
		os << "location return_path: " << it->second.return_path << std::endl;
		os << "location upload_path: " << it->second.upload_path << std::endl;
		// os << "location cgi_ext: " << it->second.Cgi.extention
		// << ", location Cgi_path: " << it->second.Cgi.path << std::endl;
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
