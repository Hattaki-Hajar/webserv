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
	bool				_done_reading;
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
	void	set_reading_status(bool);
	// void	set_event_fd(int);
	/*  getters  */
	int					get_socket() const;
	epoll_event			*get_event() const;
	char				*get_buffer() const;
	int					get_bytesread() const;
	const Server		&get_server() const;
	bool				get_reading_status(void) const;
	/*  additional funcs  */
	void	clear_buffer();
	// void	acceptconnection(Server &w);
};

int set_nonblocking(int sockfd);
