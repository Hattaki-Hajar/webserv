#include "../response/response.hpp"
#include "../Webserv.hpp"

void	get_block(std::ifstream &config_file, std::string &Server)
{
	int open_b = 1, close_b = 0;
	size_t pos;
	std::string line;

	while (std::getline(config_file, line))
	{
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
		Server += line;
		Server += '\n';
	}
	if (open_b != close_b)
		throw std::runtime_error("Error: config file is not valid 5!");
}

void	Server_block_parser(std::ifstream &config_file, Webserv &w, int Server)
{
	std::string line, directive, block, options;
	size_t i, found = 0;
	get_block(config_file, block);
	std::istringstream ss(block);
	w.set_new_Server(Server);
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
			while (line[i] && isspace(line[i]))
				i++;
			while (line[i])
				options += line[i++];
		}
		if (directive == "listen")
			listen_directive(options, w, Server);
		else if (directive == "server_name")
			Server_name_directive(options, w, Server);
		else if (directive == "error_page")
			error_page_directive(options, w, Server);
		else if (directive == "client_max_body_size")
			Client_max_body_size_directive(options, w, Server);
		else if (directive == "location")
			location_directive(ss, options, w, Server);
		else if (directive == "root")
			root_directive(options, w, Server);
		else if (directive == "index")
			index_directive(options, w, Server);
		else if (!directive.empty())
			throw std::runtime_error("Error: config file is not valid: unknown directive!");
		else
		{
			directive.clear();
			options.clear();
			continue ;
		}
		found++;
		directive.clear();
		options.clear();
	}
	if (!found)
		throw std::runtime_error("Error: empty server");
	w.set_Server(Server);
}


void	config_parser(Webserv &w, const char *name)
{
	std::ifstream	config_file;
	config_file.open(name);
	if (!config_file.good())
		throw std::runtime_error("Error: failed to open config file!");
	std::string	line, block;
	int i, Server_nb = 0;
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
		Server_nb++;
		while (line[i] && isspace(line[i]))
			i++;
		if (line[i] && line[i++] != '{')
			throw std::runtime_error("Error: config file is not valid 2!");
		while (line[i] && isspace(line[i]))
			i++;
		if (line[i] && line[i++] != '\n')
			throw std::runtime_error("Error: config file is not valid 3!");
		// std::cout << "here" << std::endl;
		Server_block_parser(config_file, w, Server_nb - 1);
		block.clear();
	}
	if (!Server_nb)
	{
		throw std::runtime_error("Error: config file has no servers!");
	}
}
