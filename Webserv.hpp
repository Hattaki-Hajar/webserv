#pragma once

#include "Server.hpp"
#include "Client.hpp"
#include "config_file_parsing/config_file_utils.hpp"
#include <fstream>
#include <vector>
#include <algorithm>


class Webserv
{
	int						_socket;
	int						_epfd;
	epoll_event				*_event;
	std::vector<Server *>	_Servers;
	std::vector<Client *>		_Clients;
public:
	/* constructors / destructors */
	Webserv();
	~Webserv();
	/* setters */
	void	set_socket(int);
	void	set_Server_socket(int, int);
	void	set_new_Server(size_t);
	void	set_port(const std::string &, int);
	void	set_host(const std::string &, int);
	void	set_name(const std::string &, int);
	void	set_body_size(size_t, int);
	void	set_error_page(int, std::string const&, int);
	void	set_Server(int );
	void	set_root(std::string const &, int);
	void	set_index(std::string const &, int);
	void	set_location(std::string const &, location &, int);
	/* getters */
	const std::string	&get_name(int);
	const Server		&get_Server(int) const;
	size_t				get_size() const;
	/* additional func */
	void	start();
	void	bind_Servers();
	void	new_connection(Server &);

	// void	listen_directive(std::string &, int, int);
};
epoll_event	*set_up_Server(Server &, int);
std::ostream& operator<<(std::ostream &os, const Webserv& s);