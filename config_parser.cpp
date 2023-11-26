#include "webserver.hpp"

void	server_block_parser(std::ifstream &config_file, webserver &w, int server)
{
	std::string	line, directive;
	int i = 0;
	std::getline(config_file, line);
	while (line[i] && isspace(line[i]))
		i++;
	while (line[i])
	{
		while (line[i] && isspace(line[i]))
			i++;
		while (line[i] && !isspace(line[i]))
			directive += line[i++];
	}
	if (directive.empty())
	{
		i = 0;
		std::getline(config_file, line);
		while (line[i] && isspace(line[i]))
			i++;
		while (line[i] && !isspace(line[i]))
			directive += line[i++];
	}
	if (directive == "listen")
		listen_directive(line, i, w, server);
	// std::cout << directive << "*" << std::endl;
}

void	config_parser(webserver &w, const char *name)
{
	std::ifstream	config_file;
	config_file.open(name);
	if (!config_file.good())
		throw std::runtime_error("Error: failed to open config file!");
	std::string	line, block;
	int i = 0, server_nb = 0;
	while (std::getline(config_file, line))
	{
		while (isspace(line[i]))
			i++;
		while (!isspace(line[i]))
			block += line[i++];
		if (block != "server")
			throw std::runtime_error("Error: config file is not valid 1!");
		server_nb++;
		while (isspace(line[i]))
			i++;
		if (line[i++] != '{')
			throw std::runtime_error("Error: config file is not valid 2!");
		while (isspace(line[i]) && line[i] != '\n')
			i++;
		if (line[i] && line[i++] != '\n')
			throw std::runtime_error("Error: config file is not valid 3!");
		server_block_parser(config_file, w, server_nb - 1);
	}
}