#include "Webserv.hpp"
#include <cstring>
#include <algorithm>

Webserv::Webserv()
{
	_event = new epoll_event;
}

void	Webserv::new_connection(Server &s)
{
	int i;
	_Clients.push_back(new Client(s));
	// std::cout << "here " << _Clients.size() << std::endl;
	i = _Clients.size() - 1;
	int new_socket = accept(s.get_socket(), (struct sockaddr *)&_Clients[i]->_addr, &_Clients[i]->_addr_size);
	if (new_socket < 0)
	{
		perror("accept");
		throw std::runtime_error("failed to connect1!");
	}
	// std::cout << "after accept" << std::endl;
	_Clients[i]->set_socket(new_socket);
	if (set_nonblocking(new_socket) < 0)
		throw std::runtime_error("set_nonblocking failed!");
	// std::cout << "after set_nonblocking" << std::endl;
	if (epoll_ctl(s.get_epfd(), EPOLL_CTL_ADD, _Clients[i]->get_socket(), _Clients[i]->get_event()) < 0)
	{
		perror("epoll_ctl");
		throw std::runtime_error("epoll_ctl failed! 2");
	}
	// std::cout << "after epoll_ctl" << std::endl;
	char buffer[BUFFER_SIZE + 1];
	bzero(buffer, BUFFER_SIZE + 1);
	int r = read(new_socket, buffer, BUFFER_SIZE);
	if (r < 0)
	{
		return ;
	}
	// std::cout << "after read " << r << " bytes:" << std::endl;
	std::cout << "buffer: " << buffer << std::endl;
	// std::cout << "socket: " << new_socket << std::endl;
}

void	Webserv::start()
{
	size_t i = 0;
	char buffer[BUFFER_SIZE + 1];
	bzero(buffer, BUFFER_SIZE + 1);
	int epfd = epoll_create1(0), event_nb = 0, fd;
	epoll_event	events[MAX_EVENTS];

	while (i < _Servers.size())
	{
		set_up_Server(*_Servers[i], epfd);
		i++;
	}
	while (1)
	{
		event_nb = epoll_wait(epfd, events, 1, 0);
		if (event_nb < 0)
			throw std::runtime_error("epoll_wait failed!");
		for (int j = 0; j < event_nb; j++)
		{
			fd = events[j].data.fd;
			i = 0;
			while (i < _Servers.size())
			{
				if (fd == _Servers[i]->get_socket())
				{
					new_connection(*_Servers[i]);
					break ;
				}
				i++;
			}
			if (events[j].events & EPOLLIN)
			{
				std::cout << "here3" << std::endl;
				bzero(buffer, BUFFER_SIZE + 1);
				if (read(fd, buffer, BUFFER_SIZE) < 0){
					std::cout << "debug: read failed" << std::endl;
					continue ;
					}
				std::cout << "buffer: " << buffer << std::endl;
			}
		}
		event_nb = 0;
	}
}

void	Webserv::bind_Servers()
{
	size_t i = 0, j, found;
	int port;
	std::string host;

	while (i < _Servers.size())
	{
		j = 0;
		found = 0;
		port = _Servers[i]->get_port();
		host = _Servers[i]->get_ip();
		while (j < i)
		{
			if (_Servers[j]->get_port() == port && _Servers[j]->get_ip() == host)
			{
				found = 1;
				break ;
			}
			j++;
		}
		if (found)
			continue ;
		_Servers[i]->bind_Server();
		i++;
	}
}
	/*  setters  */
void	Webserv::set_new_Server(size_t s)
{
	if (_Servers.size() < s + 1)
		_Servers.push_back(new Server());
}
void	Webserv::set_port(const std::string &port, int s)
{
	int p;
	int i = 0;
	std::string nb;

	if (port.empty())
		return ;
	while (port[i] && isdigit(port[i]))
		nb += port[i++];
	while (port[i] && isspace(port[i]))
		i++;
	p = atoi(nb.c_str());
	if (p < 0 || p > 65535)
		throw std::runtime_error("Error: port is not valid!");
	_Servers[s]->set_port(p);
}

void	Webserv::set_host(const std::string &h, int s)
{
	int i, nb, j = 0;
	if (h.empty())
		return ;
	std::string host = h;
	i = std::count(host.begin(), host.end(), '.');
	if (i != 3)
		throw std::runtime_error("Error: host is not valid!");
	if (h == "localhost")
	{
		_Servers[s]->set_host("127.0.0.1");
		return ;
	}
	char *str = strtok((char *)host.c_str(), ".");
	while (j < 4)
	{
		i = 0;
		while (isdigit(str[i]))
			i++;
		if (str[i])
			throw std::runtime_error("Error: host is not valid 1!");
		nb = atoi(str);
		if (nb < 0 || nb > 255)
			throw std::runtime_error("Error: host is not valid 2!");
		j++;
		str = strtok(0, ".");
	}
	if (j == 4 && str)
		throw std::runtime_error("Error: host is not valid 3!");
	_Servers[s]->set_host(h);
}

void	Webserv::set_name(const std::string &name, int Server)
{
	_Servers[Server]->set_name(name);
}

void	Webserv::set_error_page(int error, std::string const& path, int Server) {
	_Servers[Server]->set_error_page(error, path);
}

void	Webserv::set_body_size(size_t size, int Server) {
	_Servers[Server]->set_body_size(size);
}

void	Webserv::set_Server(int Server) {
	_Servers[Server]->set_addr(_Servers[Server]->get_port(), _Servers[Server]->get_ip());
	_Servers[Server]->set_len();
}

void	Webserv::set_Server_socket(int socket, int Server) {
	_Servers[Server]->set_socket(socket);
}

void	Webserv::set_root(std::string const &path, int Server) {
	_Servers[Server]->set_root(path);
}
void	Webserv::set_location(std::string const &path, location &loc, int Server) {
	_Servers[Server]->set_location(path, loc);
}
	/*  getters */
const std::string	&Webserv::get_name(int Server) {
	return (_Servers[Server]->get_name());
}

const Server	&Webserv::get_Server(int Server) const {
	return (*_Servers[Server]);
}

size_t	Webserv::get_size() const {
	return (_Servers.size());
}
/*  << overload for Webserv  */
std::ostream& operator<<(std::ostream &os, const Webserv& s)
{
	size_t i = 0;
	while (i < s.get_size())
	{
		os << "Server index: " << i << std::endl;
		os << s.get_Server(i);
		i++;
	}
	return os;
}

Webserv::~Webserv()
{
	// std::cout << "Webserv destructor called" << std::endl;
	for (size_t i = 0; i < _Servers.size(); i++)
		delete _Servers[i];
	for (size_t i = 0; i < _Clients.size(); i++)
		delete _Clients[i];
	delete _event;
}
