#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <exception>

#define BUFFER_SIZE 30720

// struct in_addr {
//     unsigned long s_addr;
// };

// struct sockaddr_in {
// 	short			sin_family;
// 	unsigned short	sin_port;
// 	struct in_addr	sin_addr;
// 	char			sin_zero[8];
// };

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
	void	start();
	void	start_listen();
	void	acceptconnection(int &);
	// void	send_response(int, std::string const&);
};