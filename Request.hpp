#pragma once

#include <map>
#include <iostream>
#include <fstream>
#include "Server.hpp"
// #include <random>

typedef struct Request_line
{
	std::string	method;
	std::string	uri;
	std::string	version;
}	request_line;

class Request
{
	std::map<std::string, std::string>	_headers;
	Request_line						_request_line;
	long								_size_read;
	ssize_t								_chunk_read;
	ssize_t								_first_chunk_size;
	ssize_t								_chunks_size;
	std::string							_request_headers;
	char*								_remaining;
	std::fstream						_file;
	bool								_headers_read;
	bool								_end_of_request;
public:
	/*	constructor destructor	*/
	Request();
	// Request(Request const &);
	~Request();
	// Request &operator=(Request const &);
	/*	setters	*/
	/*	getters	*/
	std::map<std::string, std::string>	get_headers() const;
	const request_line					&get_request_line() const;
	long								get_size_read() const;
	std::fstream						&get_file();
	bool								get_end_of_request() const;
	/*	additional function	*/
	void	split_request(char *, ssize_t);
	void	parse_request();
};
