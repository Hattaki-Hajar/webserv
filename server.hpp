#pragma once

#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <exception>

#define BUFFER_SIZE 30720

class server
{
	int					_socket;
	int					_newSocket;
	int					_port;
	const std::string	&_ip;
	std::string			_message;
	sockaddr_in			_addr;
	unsigned int		_socketaddr_len;
	server();
public:
	/* constructors / destructors */
	server(std::string const&, int);
	~server();
	/* additional functions */
	void	bind_server();
	void	start_listen();
	void	acceptconnection(int &);
	/* setters */
	void	set_socket(int);

	// void	send_response(int, std::string const&);
};