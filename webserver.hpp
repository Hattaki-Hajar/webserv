#pragma once

#include "server.hpp"
#include "utils.hpp"
#include <fstream>
#include <vector>

class webserver
{
	int					_socket;
	std::vector<server> _servers;
public:
	/* constructors / destructors */
	webserver();
	~webserver();
	/* setters */
	void	set_socket(int);
	void	set_port(const std::string &, int);
	void	set_host(const std::string &, int);
	void	set_name(const std::string &, int);
	/* getters */
	std::string	&get_name(int);
	/* additional func */
	void	start();

	// void	listen_directive(std::string &, int, int);
};
