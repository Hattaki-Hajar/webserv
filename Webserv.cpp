#include "Webserv.hpp"
#include <cstring>
#include <algorithm>

Webserv::Webserv()
{}

void	Webserv::start()
{
	size_t i = 0;
	int epfd = epoll_create1(0);

	while (i < _Servers.size())
	{
		set_up_Server(*_Servers[i], epfd);
		i++;
	}
	while (1)
	{
		i = 0;
		while (i < _Servers.size())
		{
			_Servers[i]->start();
			i++;
		}
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
}
