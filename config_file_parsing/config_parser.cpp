#include "../webserver.hpp"

void	get_block(std::ifstream &config_file, std::string &server)
{
	int open_b = 1, close_b = 0;
	size_t pos;
	std::string line;

	while (std::getline(config_file, line))
	{
		server += line;
		server += '\n';
		pos = line.find('{');
		if (pos != std::string::npos)
			open_b++;
		pos = line.find('}');
		if (pos != std::string::npos)
		{
			close_b++;
			if (open_b == close_b)
				break ;
		}
	}
	if (open_b != close_b)
		throw std::runtime_error("Error: config file is not valid 5!");
}

void	server_block_parser(std::ifstream &config_file, webserver &w, int server)
{
	std::string line, directive, block;
	size_t i;
	get_block(config_file, block);
	std::istringstream ss(block);
	w.set_new_server(server);
	while (std::getline(ss, line))
	{
		i = 0;
		while (line[i] && isspace(line[i]))
			i++;
		if (line[i] == '#')
			continue ;
		if (line[i])
		{
			while (line[i] && isspace(line[i]))
				i++;
			while (line[i] && !isspace(line[i]))
			{
				directive += line[i];
				i++;
			}
		}
		if (directive == "listen")
			listen_directive(line, i, w, server);
		if (directive == "server_name")
			server_name_directive(line, i, w, server);
		if (directive == "error_page")
			error_page_directive(line, i, w, server);
		if (directive == "client_max_body_size")
			client_max_body_size_directive(line, i, w, server);
		if (directive == "location")
			location_directive(ss, line, i, w, server);
		if (directive == "root")
			root_directive(line, i, w, server);
		directive.clear();
	}
	std::cout << w;
}


void	config_parser(webserver &w, const char *name)
{
	std::ifstream	config_file;
	config_file.open(name);
	if (!config_file.good())
		throw std::runtime_error("Error: failed to open config file!");
	std::string	line, block;
	int i, server_nb = 0;
	while (std::getline(config_file, line))
	{
		i = 0;
		while (line[i] && isspace(line[i]))
			i++;
		while (line[i] && !isspace(line[i]))
			block += line[i++];
		if (block.empty())
			continue ;
		if (block != "server")
			throw std::runtime_error("Error: config file is not valid 1!");
		server_nb++;
		while (line[i] && isspace(line[i]))
			i++;
		if (line[i] && line[i++] != '{')
			throw std::runtime_error("Error: config file is not valid 2!");
		while (line[i] && isspace(line[i]))
			i++;
		if (line[i] && line[i++] != '\n')
			throw std::runtime_error("Error: config file is not valid 3!");
		server_block_parser(config_file, w, server_nb - 1);
		block.clear();
	}
}
