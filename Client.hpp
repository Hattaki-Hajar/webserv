#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include "Server.hpp"

class Client
{
	int					_socket;
	int					_bytesread;
	std::string			_message;
	char				_buffer[BUFFER_SIZE + 1];
	Server				&_server;
	struct epoll_event	*_event;
	Client();
public:
	sockaddr_in			_addr;
	unsigned int		_addr_size;
	/*  constructer/destructer  */
	Client(Server &);
	~Client();
	/*  setters  */
	void	set_socket(int);
	void	set_bytesread(int);
	void	set_message(std::string const&);
	// void	set_event_fd(int);
	/*  getters  */
	int					get_socket() const;
	epoll_event			*get_event() const;
	const std::string	&get_message() const;
	char				*get_buffer() const;
	int					get_bytesread() const;
	/*  additional funcs  */
	void	clear_buffer();
	// void	acceptconnection(Server &w);
};

int set_nonblocking(int sockfd);
