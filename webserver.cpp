#include "webserver.hpp"
#include <cstring>

webserver::webserver()
{}

void	webserver::start()
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0)
		throw std::runtime_error("socket failed!");
	int on = 1;
    setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    setsockopt(_socket, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
}

void	webserver::set_port(const std::string &port, int s)
{
	if (port.empty())
		throw std::runtime_error("Error: port is empty!");
	int p;
	int i = 0;
	while (isdigit(port[i]))
		i++;
	if (port[i])
		throw std::runtime_error("Error: port is not valid!");
	p = atoi(port.c_str());
	if (p < 0 || p > 65535)
		throw std::runtime_error("Error: port is not valid!");
	if (_servers.size() < (size_t)s + 1)
		_servers.push_back(server(p));
	else
		_servers[s].set_port(p);
}

void	webserver::set_host(const std::string &h, int s)
{
	if (h.empty())
		throw std::runtime_error("Error: host is empty!");
	std::string host = h;
	char *str = strtok((char *)host.c_str(), ".");
	int i, nb, j = 0;
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
	if (_servers.size() < (size_t)s + 1)
		_servers.push_back(server(host));
	else
		_servers[s].set_host(h);
}

webserver::~webserver()
{}
