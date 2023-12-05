#include "webserver.hpp"
#include <cstring>
#include <algorithm>

webserver::webserver()
{}
// i server number
void	webserver::start(int port, std::string const &ip)
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	_servers.push_back(server(port));
	if (_socket < 0)
		throw std::runtime_error("socket failed!");
	std::cout << "socket: " << _socket << std::endl;
	int on = 1;
    setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    setsockopt(_socket, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
	set_server(_socket, port, ip);
	if (bind(_socket,(sockaddr *)&_servers[0].get_addr(), _servers[0].get_socketaddr_len()) < 0)
		throw std::runtime_error("bind failed!");
	_servers[0].start_listen();
	_servers[0].acceptconnection(_servers[0].get_new_socket());
	char	buffer[BUFFER_SIZE] = {0};
	// run this reda in a loop while parsing to continouisly reading
	// note that a body size is set in the header, so make sure to count what you read
	ssize_t	bytesReceived = read(_servers[0].get_new_socket(), buffer, BUFFER_SIZE);
	if (bytesReceived < 0)
		throw std::runtime_error("read failed!");
	std::cout << buffer << std::endl;
}

void	webserver::set_port(const std::string &port, int s)
{
	int p;
	int i = 0;
	std::string nb;

	if (port.empty())
		throw std::runtime_error("Error: port is empty!");
	while (port[i] && isdigit(port[i]))
		nb += port[i++];
	while (port[i] && isspace(port[i]))
		i++;
	if (port[i] != ';')
		throw std::runtime_error("Error: port is not valid!");
	p = atoi(nb.c_str());
	if (p < 0 || p > 65535)
		throw std::runtime_error("Error: port is not valid!");
	if (_servers.size() < (size_t)s + 1)
		_servers.push_back(server(p));
	else
		_servers[s].set_port(p);
}

void	webserver::set_host(const std::string &h, int s)
{
	int i, nb, j = 0;
	if (h.empty())
		throw std::runtime_error("Error: host is empty!");
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
	if ((size_t)server > _servers.size())
		_servers.push_back(server);
	else
		_servers[server].set_name(name);
}

std::string	&webserver::get_name(int server)
{
	return (_servers[server].get_name());
}

void	webserver::set_error_page(int error, std::string const& path, int server)
{
	_servers[server].set_error_page(error, path);
}

void	webserver::set_body_size(size_t size, int server)
{
	_servers[server].set_body_size(size);
}

void	webserver::set_server(int socket, int port, std::string const &ip)
{
	_servers[0].set_socket(socket);
	_servers[0].set_addr(port, ip);
	_servers[0].set_len();
}

webserver::~webserver()
{}
