#include "webserver.hpp"
#include <cstring>
#include <algorithm>

webserver::webserver()
{}

void	webserver::start()
{
	struct epoll_event	*event;
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0)
		throw std::runtime_error("socket failed!");
	int on = 1, event_nb, fd;
    setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    setsockopt(_socket, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
	_epfd = epoll_create1(0);
	if (_epfd < 0)
		throw std::runtime_error("epoll_create1 failed!");
	event->events = EPOLLIN | EPOLLOUT;
	while (1)
	{
		event_nb = 0;
		if (epoll_ctl(_epfd, EPOLL_CTL_ADD, _socket, event) < 0)
			throw std::runtime_error("epoll_ctl failed!");
		event_nb = epoll_wait(_epfd, event, 1, 0);
		if (event_nb < 0)
			throw std::runtime_error("epoll_wait failed!");
		else if (event_nb == 0)
			continue ;
		else
		{
			for (int i = 0; i < event_nb; i++)
			{
				fd = event[i].data.fd;
				if (event[i].events & EPOLLIN)
				{
					// slay
				}
				if (event[i].events & EPOLLOUT)
				{
					// slay
				}
			}
		}
		// char	buffer[BUFFER_SIZE] = {0};
		// ssize_t	bytesReceived = read(_servers[0].get_new_socket(), buffer, BUFFER_SIZE);
		// epoll_create1(0);
		// if (bytesReceived < 0)
		// 	throw std::runtime_error("read failed!");
		// std::cout << buffer << std::endl;
	}
}

void	webserver::bind_servers()
{
	size_t i = 0, j, found;
	int port;
	std::string host;

	while (i < _servers.size())
	{
		j = 0;
		found = 0;
		port = _servers[i].get_port();
		host = _servers[i].get_ip();
		while (j < i)
		{
			if (_servers[j].get_port() == port && _servers[j].get_ip() == host)
			{
				found = 1;
				break ;
			}
			j++;
		}
		if (found)
			continue ;
		_servers[i].bind_server();
		i++;
	}
}
	/*  setters  */
void	webserver::set_new_server(size_t s)
{
	if (_servers.size() < s + 1)
		_servers.push_back(server());
}
void	webserver::set_port(const std::string &port, int s)
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
	_servers[s].set_port(p);
}

void	webserver::set_host(const std::string &h, int s)
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
		_servers[s].set_host("127.0.0.1");
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
	_servers[s].set_host(h);
}

void	webserver::set_name(const std::string &name, int server)
{
	_servers[server].set_name(name);
}

void	webserver::set_error_page(int error, std::string const& path, int server) {
	_servers[server].set_error_page(error, path);
}

void	webserver::set_body_size(size_t size, int server) {
	_servers[server].set_body_size(size);
}

void	webserver::set_server(int server) {
	_servers[server].set_addr(_servers[server].get_port(), _servers[server].get_ip());
	_servers[server].set_len();
}

void	webserver::set_server_socket(int socket, int server) {
	_servers[server].set_socket(socket);
}

void	webserver::set_root(std::string const &path, int server) {
	_servers[server].set_root(path);
}
void	webserver::set_location(std::string const &path, location &loc, int server) {
	_servers[server].set_location(path, loc);
}
	/*  getters */
const std::string	&webserver::get_name(int server) {
	return (_servers[server].get_name());
}

const server	&webserver::get_server(int server) const {
	return (_servers[server]);
}

size_t	webserver::get_size() const {
	return (_servers.size());
}
/*  << overload for webserver  */
std::ostream& operator<<(std::ostream &os, const webserver& s)
{
	size_t i = 0;
	while (i < s.get_size())
	{
		os << "server index: " << i << std::endl;
		os << s.get_server(i);
		i++;
	}
	return os;
}

webserver::~webserver()
{}
