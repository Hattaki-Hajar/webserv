#include "webserver.hpp"

void	config_parser(webserver &w, const char *name)
{
	(void)w;
	std::ifstream	config_file;
	config_file.open(name);
	if (!config_file.good())
		throw std::runtime_error("config file failed to open!");
	std::string	line, block;
	int i = 0;
	std::getline(config_file, line);
	while (isspace(line[i]))
		i++;
	while (!isspace(line[i]))
		block += line[i++];
	if (block != "server" && block != "http")
		throw std::runtime_error("config file is not valid!");
	while (isspace(line[i]))
		i++;
	if (line[i] != '{')
		throw std::runtime_error("config file is not valid!");
}