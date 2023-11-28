#pragma once

#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <exception>
#include <cstdlib>

#define BUFFER_SIZE 30720
#define	NO_NAME "no name"
#define	DEFAULT_SERVER "default"

class server
{
	int					_socket;
	int					_newSocket;
	std::string			_ip;
	int					_port;
	std::string			_message;
	sockaddr_in			_addr;
	unsigned int		_socketaddr_len;
	std::string 		_name;
public:
	/* constructors / destructors */
	server();
	server(int);
	server(const std::string &);
	~server();
	/* additional functions */
	void	bind_server();
	void	start_listen();
	void	acceptconnection(int &);
	/* setters */
	void	set_socket(int);
	void	set_port(int port);
	void	set_host(const std::string &);
	void	set_name(const std::string &);
	/* getters */
	std::string	&get_name();

	// void	send_response(int, std::string const&);
};