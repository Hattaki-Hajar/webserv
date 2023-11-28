#include "webserver.hpp"

void	listen_directive(std::string &line, int i, webserver &w, int server)
{
	std::string host_port, port, host;
	size_t pos;
	while (line[i] && isspace(line[i]))
		i++;
	while (line[i] && !isspace(line[i]))
		host_port += line[i++];
	pos = host_port.find(':');
	if (pos == std::string::npos)
		throw std::runtime_error("Error: config file is not valid 4!");
	host = host_port.substr(0, pos);
	port = host_port.substr(pos + 1, host_port.size() - pos - 1);
	w.set_port(port, server);
	w.set_host(host, server);
}
