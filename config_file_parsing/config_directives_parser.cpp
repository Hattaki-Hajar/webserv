#include "../webserver.hpp"

void	listen_directive(std::string &line, webserver &w, int server)
{
	std::string host_port, port, host;
	size_t pos, i = 0;
	std::cout << "line = " << line << std::endl;
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i] && !isdigit(line[i]))
		throw std::runtime_error("Error: config file is not valid! listen");
	while (line[i] && !isspace(line[i]) && line[i] != ';')
		host_port += line[i++];
	pos = host_port.find(':');
	if (pos == std::string::npos)
	{
		i = 0;
		host = "";
		port = "";
		pos = host_port.find('.');
		if (pos == std::string::npos)
		{
			while (host_port[i])
				port += host_port[i++];
		}
		else
			while (host_port[i])
				host += host_port[i++];
	}
	else
	{
		host = host_port.substr(0, pos);
		port = host_port.substr(pos + 1, host_port.size() - pos - 1);
	}
	std::cout << "host = " << host << std::endl;
	std::cout << "port = " << port << std::endl;
	w.set_port(port, server);
	w.set_host(host, server);
}

void	server_name_directive(const std::string &line, webserver &w, int server)
{
	std::string name;
	int s = 0, i = 0;

	while (line[i] && isspace(line[i]))
		i++;
	while (line[i] && !isspace(line[i]) && line[i] != ';')
		name += line[i++];
	while (line[i] && isspace(line[i]))
		i++;
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

void	error_page_directive(std::string &line, webserver &w, int server)
{
	std::string error, path;
	size_t pos, j, i = 0;
	int error_code;

	pos = line.find('/');
	if (pos == std::string::npos)
		throw std::runtime_error("Error: config file is not valid!");
	while (pos > 0 && !isspace(line[pos - 1]))
		pos--;
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
			throw std::runtime_error("Error: config file is not valid 1-!");
		error_code = atoi(error.c_str());
		w.set_error_page(error_code, path, server);
		error.clear();
		i++;
	}
}

void	client_max_body_size_directive(std::string &line, webserver &w, int server)
{
	std::string size;
	int j = 0, r_size, i = 0;

	while (line[i] && isspace(line[i]))
		i++;
	while (line[i] && isdigit(line[i]) && line[i] != ';')
		size += line[i++];
	// std::cout << "line = " << line << std::endl;
	if ((line[i] && line[i] != 'k' && line[i] != 'K' && line[i] != 'm' && line[i]!= 'M'
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
	w.set_body_size(r_size, server);
	while (line[i] && isspace(line[i]))
		i++;
}

void	root_directive(std::string &line, webserver &w, int server)
{
	std::string path;
	size_t i = 0;

	while (line[i] && isspace(line[i]))
		i++;
	while (line[i] && !isspace(line[i]) && line[i] != ';')
		path += line[i++];
	while (line[i] && isspace(line[i]))
		i++;
	w.set_root(path, server);
}
