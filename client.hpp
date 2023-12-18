#pragma once

#include "server.hpp"

class client
{
	int				_socket;
	sockaddr_in		_addr;
	unsigned int	_addr_size;
	std::string		_message;
public:
	/*  constructer/destructer  */
	client();
	~client();
	/*  setters  */
	void	set_socket(int);
	void	set_message(std::string const&);
	/*  getters  */
	int		get_socket() const;
	const std::string	&get_message() const;
	/*  additional funcs  */
	void	acceptconnection(server &w);
};
