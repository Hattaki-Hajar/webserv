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
#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>

#define BUFFER_SIZE 3072
#define	NO_NAME "no name"
#define	DEFAULT_Server "default"
#define MAX_EVENTS 500
#define USER "ohalim"

typedef struct location
{
	std::string root;
	bool		autoindex;
	std::string	index;
	size_t			max_body_size;
	int			return_code;
	std::map<std::string, std::string> cgi;
	std::string	return_path;
	std::string	upload_path;
	std::vector<std::string>	methods;
}	location;

class Server
{
	int								_socket;
	int								_epfd;
	std::string						_ip;
	int								_port;
	sockaddr_in						_addr;
	unsigned int					_socketaddr_len;
	std::string 					_name;
	long							_max_body_size;
	std::string						_root_path;
	std::string						_index_path;
	std::map<int, std::string>		_error_pages;
	std::map<std::string, location>	_locations;
	struct epoll_event				*_event;
public:
	bool							_is_bound;
	/* constructors / destructors */
	Server();
	Server(Server const &);
	Server(int);
	~Server();
	/* additional functions */
	void	bind_Server();
	void	start();
	void	ip_to_in_addr_t();
	/* setters */
	void	set_socket(int);
	void	set_epoll_fd(int);
	void	set_body_size(size_t);
	void	set_port(int port);
	void	set_host(const std::string &);
	void	set_name(const std::string &);
	void	set_error_page(int, std::string const&);
	void	set_root(std::string const&);
	void	set_index(std::string const&);
	void	set_len();
	void	set_addr(int, std::string const&);
	void	set_location(std::string const &, location &);
	/* getters */
	const std::string		&get_name() const;
	unsigned int			&get_socketaddr_len();
	const sockaddr_in		&get_addr() const;
	int						get_socket();
	int						get_epfd() const;
	int						get_body_size() const;
	const std::string		&get_root() const;
	const std::string		&get_index() const;
	const std::string		&get_ip() const;
	int						get_port() const;
	const std::map<std::string, location>						&get_location() const;
	epoll_event				*get_event();
	long					get_max_body_size() const;
	const std::map<std::string, location>::const_iterator		get_location_begin_iter() const;
	const std::map<std::string, location>::const_iterator		get_location_end_iter() const;
	const std::map<std::string, location>						&get_locations() const;
	const std::map<int, std::string>							&get_error_pages() const;
	// void	send_response(int, std::string const&);
};

std::ostream& operator<<(std::ostream &os, const Server& s);