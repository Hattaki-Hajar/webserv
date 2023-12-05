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

// void	no_name_server(webserver &w, int server)
// {
// 	int s = 0;

// 	w.set_name(DEFAULT_SERVER, server);
// 	while (s <= server)
// 	{
// 		if (w.get_name(s) == DEFAULT_SERVER)
// 		{
// 			w.set_name(NO_NAME, server);
// 			return ;
// 		}
// 		s++;
// 	}
// }

void	server_name_directive(const std::string &line, int i, webserver &w, int server)
{
	std::string name;
	int s = 0;

	while (line[i] && isspace(line[i]))
		i++;
	while (line[i] && !isspace(line[i]))
		name += line[i++];
	if (name.empty() || name == """")
	{
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
		return ;
	}
	w.set_name(name, server);
}

void	error_page_directive(std::string &line, size_t i, webserver &w, int server)
{
	std::string error, path;
	size_t pos, j;
	int error_code;

	pos = line.find('/');
	if (pos == std::string::npos)
		throw std::runtime_error("Error: config file is not valid!");
	j = pos;
	while (line[j] && !isspace(line[j]) && line[j] != ';')
		j++;
	while (line[j] && isspace(line[j]) && line[j] != ';')
		j++;
	if (line[j] && line[j] != ';')
		throw std::runtime_error("Error: config file is not valid 0!");
	path = line.substr(pos, j - pos);
	while (i < pos)
	{
		j = 0;
		while (line[i] && isspace(line[i]))
			i++;
		while (line[i] && !isspace(line[i]))
			error += line[i++];
		while (error[j] && isdigit(error[j]))
			j++;
		if (error[j] && !isspace(error[j]))
			throw std::runtime_error("Error: config file is not valid 1!");
		error_code = atoi(error.c_str());
		w.set_error_page(error_code, path, server);
		error.clear();
		i++;
	}
}

void	client_max_body_size_directive(std::string &line, size_t i, webserver &w, int server)
{
	std::string size;
	int j = 0, r_size;

	while (line[i] && isspace(line[i]))
		i++;
	while (line[i] && isdigit(line[i]))
		size += line[i++];
	if ((line[i] != 'k' && line[i] != 'K' && line[i] != 'm' && line[i]!= 'M'
		&& line[i] != 'g' && line[i] != 'G') || size.empty())
		throw std::runtime_error("Error: config file is not valid size1!");
	while (size[j] && isdigit(size[j]))
		j++;
	if (size[j])
		throw std::runtime_error("Error: config file is not valid size2!");
	r_size = atoi(size.c_str());
	if (line[i] == 'k' || line[i] == 'K')
		r_size *= 1024;
	else if (line[i] == 'm' || line[i] == 'M')
		r_size *= 1024 * 1024;
	else if (line[i] == 'g' || line[i] == 'G')
		r_size *= 1024 * 1024 * 1024;
	else
		r_size *= 1;
	i++;
	w.set_body_size(r_size, server);
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i] != ';')
		throw std::runtime_error("Error: config file is not valid size3!");
}

void	location_directive(std::string &line, size_t i, webserver &w, int server)
{
	(void)w;
	(void)server;
	std::string path;

	while (line[i] && isspace(line[i]))
		i++;
	if (line[i] == '=')
	{
		i++;
		while (line[i] && isspace(line[i]))
			i++;
	}
	if (line[i] != '/')
		throw std::runtime_error("Error: config file is not valid location1!");
	while (line[i] && !isspace(line[i]))
		path += line[i++];
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i] != '{')
		throw std::runtime_error("Error: config file is not valid location2!");
	
}
