#include "../Webserv.hpp"

std::string	get_option(const std::string &line)
{
	std::string option;
	size_t i = 0;
	while (line[i] && !isspace(line[i]) && line[i] != ';')
		option += line[i++];
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i++] != ';')
		throw std::runtime_error("Error: config file is not valid 4!");
	while (line[i])
		if (!isspace(line[i++]))
			throw std::runtime_error("Error: config file is not valid 5!");
	return (option);
}

void	listen_directive(std::string &line, Webserv &w, int Server)
{
	std::string host_port, port, host;
	size_t pos, i;
	host_port = get_option(line);
	if (host_port.empty())
		throw std::runtime_error("Error: config file is not valid");
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
	w.set_port(port, Server);
	w.set_host(host, Server);
}

void	Server_name_directive(const std::string &line, Webserv &w, int Server)
{
	std::string name;
	int s = 0;

	name = get_option(line);
	// if (name.empty())
	// 	std::cout << 
	if (name.empty() || name == """")
	{
		w.set_name(DEFAULT_Server, Server);
		while (s <= Server)
		{
			if (w.get_name(s) == DEFAULT_Server)
			{
				w.set_name(NO_NAME, Server);
				return ;
			}
			s++;
		}
		return ;
	}
	w.set_name(name, Server);
}

void	error_page_directive(std::string &line, Webserv &w, int Server)
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
		w.set_error_page(error_code, path, Server);
		error.clear();
		i++;
	}
}

void	Client_max_body_size_directive(std::string &line, Webserv &w, int Server)
{
	std::string size;
	int j = 0, r_size, i = 0;

	while (line[i] && isdigit(line[i]) && line[i] != ';')
		size += line[i++];
	if ((line[i] && line[i] != 'k' && line[i] != 'K' && line[i] != 'm' && line[i]!= 'M'
		&& line[i] != 'g' && line[i] != 'G' && line[i] != ';') || size.empty())
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
	w.set_body_size(r_size, Server);
	while (line[i] && isspace(line[i]))
		i++;
}

void	root_directive(std::string &line, Webserv &w, int Server)
{
	std::string path = get_option(line);
	w.set_root(path, Server);
}

void	index_directive(std::string &line, Webserv &w, int Server)
{
	std::string path = get_option(line);
	w.set_index(path, Server);
}
