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

void	no_name_server(webserver &w, int server)
{
	int s = 0;

	w.set_name(DEFAULT_SERVER, server);
	while (s <= server)
	{
		if (w.get_name(s) == DEFAULT_SERVER)
		{
			w.set_name(NO_NAME, server);
			return ;
		}
		s++;
	}
}

void	server_name_directive(const std::string &line, int i, webserver &w, int server)
{
	std::string name;
	while (line[i] && isspace(line[i]))
		i++;
	while (line[i] && !isspace(line[i]))
		name += line[i++];
	if (name.empty() || name == """")
		return (no_name_server(w, server));
	w.set_name(name, server);
}
