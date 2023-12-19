#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
// #include "Server.hpp"

class Client
{
	int					_socket;
	std::string			_message;
	struct epoll_event	*_event;
public:
	sockaddr_in			_addr;
	unsigned int		_addr_size;
	/*  constructer/destructer  */
	Client();
	Client(int);
	~Client();
	/*  setters  */
	void	set_socket(int);
	void	set_message(std::string const&);
	void	set_event_fd(int);
	/*  getters  */
	int		get_socket() const;
	const std::string	&get_message() const;
	/*  additional funcs  */
	// void	acceptconnection(Server &w);
};
