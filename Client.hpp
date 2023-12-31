#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include "Request.hpp"
#include "Server.hpp"
class Response;
class Client
{
	int					_socket;
	ssize_t				_bytesread;
	std::string			_message;
	char				_buffer[BUFFER_SIZE + 1];
	Server				&_server;
	struct epoll_event	*_event;
	bool				_done_reading;
	Request 			*_request;
	unsigned int		_status_code;
	Client();
public:
	Response			*_response;
	sockaddr_in			_addr;
	unsigned int		_addr_size;
	/*  constructer/destructer  */
	Client(Server &);
	~Client();
	/*  setters  */
	void	set_socket(int);
	void	set_bytesread(ssize_t);
	void	set_reading_status(bool);
	// void	set_event_fd(int);
	/*  getters  */
	int					get_socket() const;
	epoll_event			*get_event() const;
	char				*get_buffer() const;
	ssize_t				get_bytesread() const;
	const Server		&get_server() const;
	bool				get_reading_status(void) const;
	const Request		*get_request() const;
	bool				get_done_reading() const;
	/*  additional funcs  */
	void	clear_buffer();
	void	parse_request();
	void 	generateResponse();
	// void	acceptconnection(Server &w);
};

#include "response/response.hpp"
int set_nonblocking(int sockfd);
