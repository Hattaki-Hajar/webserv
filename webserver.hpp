#pragma once

#include "server.hpp"
#include "client.hpp"
#include "config_file_parsing/config_file_utils.hpp"
#include <fstream>
#include <vector>

class webserver
{
	int					_socket;
	int					_epfd;
	std::vector<server> _servers;
	std::vector<client> _clients;
public:
	/* constructors / destructors */
	webserver();
	~webserver();
	/* setters */
	void	set_socket(int);
	void	set_server_socket(int, int);
	void	set_new_server(size_t);
	void	set_port(const std::string &, int);
	void	set_host(const std::string &, int);
	void	set_name(const std::string &, int);
	void	set_body_size(size_t, int);
	void	set_error_page(int, std::string const&, int);
	void	set_server(int );
	void	set_root(std::string const &, int);
	void	set_location(std::string const &, location &, int);
	/* getters */
	const std::string	&get_name(int);
	const server		&get_server(int) const;
	size_t				get_size() const;
	/* additional func */
	void	start();
	void	bind_servers();

	// void	listen_directive(std::string &, int, int);
};
std::ostream& operator<<(std::ostream &os, const webserver& s);