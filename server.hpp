#pragma once

#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <exception>
#include <cstdlib>
#include <map>
#include <vector>

#define BUFFER_SIZE 30720
#define	NO_NAME "no name"
#define	DEFAULT_SERVER "default"

typedef struct error_page
{
	int			error;
	std::string	path;
}				error_page;

typedef struct location
{
	std::string root;
	bool		autoindex;
	std::string	index;
	int			max_body_size;
	int			return_code;
	std::string	return_path;
	std::vector<std::string>	methods;
}	location;

class server
{
	int						_socket;
	int						_newSocket;
	std::string				_ip;
	int						_port;
	std::string				_message;
	sockaddr_in				_addr;
	unsigned int			_socketaddr_len;
	std::string 			_name;
	size_t					_max_body_size;
	std::string				_root_path;
	std::map<int, std::string>	_error_pages;
	std::map<std::string, location>	_locations;
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
	void	set_body_size(size_t);
	void	set_port(int port);
	void	set_host(const std::string &);
	void	set_name(const std::string &);
	void	set_error_page(int, std::string const&);
	void	set_root(std::string const&);
	void	set_new_socket(int socket);
	void	set_len();
	void	set_addr(int, std::string const&);
	void	set_location(std::string const &, location &);
	/* getters */
	const std::string		&get_name() const;
	unsigned int			get_socketaddr_len() const;
	const sockaddr_in		&get_addr() const;
	int						&get_new_socket();
	const std::string		&get_root() const;
	const std::string		&get_ip() const;
	int						get_port() const;
	const std::map<std::string, location>::const_iterator		get_location_begin_iter() const;
	const std::map<std::string, location>::const_iterator		get_location_end_iter() const;

	// void	send_response(int, std::string const&);
};

std::ostream& operator<<(std::ostream &os, const server& s);