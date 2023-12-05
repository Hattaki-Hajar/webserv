#pragma once

#include "server.hpp"
#include "config_file_utils.hpp"
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
	void	set_body_size(size_t, int);
	void	set_error_page(int, std::string const&, int);
	void	set_server(int socket, int port, std::string const &ip);
	/* getters */
	std::string	&get_name(int);
	/* additional func */
	void	start(int, std::string const &);

	// void	listen_directive(std::string &, int, int);
};