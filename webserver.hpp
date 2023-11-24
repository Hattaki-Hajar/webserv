#pragma once

#include "server.hpp"
#include "utils.hpp"
#include <fstream>
#include <vector>

class webserver
{
	int					_socket;
	std::vector<server> _servers;
	// std::ifstream		_config_file;
public:
	/* constructors / destructors */
	webserver();
	~webserver();
	/* setters */
	void	set_socket(int);
	// void	set_config_file(std::string &);
	/* additional func */
	void	start();
};
