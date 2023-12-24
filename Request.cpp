#include "Request.hpp"
# include <cstdio>
/*  constructors destructors  */
Request::Request() {
	_size_read = 0;
	_chunk_read = 0;
	_first_chunk_size = 0;
	_chunks_size = 0;
	_file.open("test.jpeg", std::ios::out | std::ios::app);
	_headers_read = false;
}

Request::~Request() {
}

/*  setters  */
/*	getters	*/
std::map<std::string, std::string>	Request::get_headers() const {
	return (_headers);
}
const request_line	&Request::get_request_line() const {
	return (_request_line);
}
long	Request::get_size_read() const {
	return (_size_read);
}
std::fstream	&Request::get_file() {
	return (_file);
}
/*	additional functions	*/
void	Request::split_request(char *buffer, ssize_t bytesread) {
	ssize_t	i = 0;
	while (i < bytesread && !_headers_read)
	{
		if (buffer[i] == '\r')
			if (buffer[i + 1] == '\n')
				if (buffer[i + 2] == '\r')
					if (buffer[i + 3] == '\n')
					{
						_headers_read = true;
						i += 4;
						break ;
					}
		_request_headers += buffer[i];
		i++;
	}
	this->parse_request();
	if (!_file.good())
		std::cout << "debug: file not open!" << std::endl;
	if (_headers["Transfer-Encoding"] == "chunked") {
		if (!_first_chunk_size) {
			std::string line;
			std::istringstream ss(buffer + i);
			getline(ss, line);
			std::stringstream hex;
			hex << std::hex << line.substr(0, line.length() - 1);
			buffer += line.length() + 1;
			hex >> _first_chunk_size;
				std::cout << "debug: first chunk size: " << _first_chunk_size << std::endl;
			_chunks_size = _first_chunk_size;
			while (_chunk_read < _chunks_size && i < bytesread) {
				_file.put(buffer[i]);
				_file.flush();
				_chunk_read++;
				i++;
			}
		}
		else if (_chunk_read < _chunks_size && i < bytesread) {
			while (_chunk_read < _chunks_size && i < bytesread) {
				_file.put(buffer[i]);
				_file.flush();
				_chunk_read++;
				i++;
			}
		}
		if (_chunk_read == _chunks_size && i < bytesread) {
			std::string line;
			std::istringstream ss(buffer + i);
			getline(ss, line);
			std::stringstream hex;
			hex << std::hex << line.substr(0, line.length() - 1);
			buffer += line.length() + 1;
			hex >> _chunks_size;
			_chunk_read = 0;
			std::cout << "debug: chunk size: " << _chunks_size << std::endl;
			while (_chunk_read < _chunks_size && i < bytesread) {
				_file.put(buffer[i]);
				_file.flush();
				_chunk_read++;
				i++;
			}
		}
	}
	else {
		_file.write(buffer + i, bytesread - i);
		_file.flush();
		_size_read += bytesread - i;
	}
}
void	Request::parse_request() {
	if (_request_headers.empty())
		return ;
	std::istringstream ss(_request_headers);
	std::string line;
	getline(ss, line);
	line = line.substr(0, line.length() - 1);
	_request_line.method = line.substr(0, line.find(' '));
	line.erase(0, line.find(' ') + 1);
	_request_line.uri = line.substr(0, line.find(' '));
	line.erase(0, line.find(' ') + 1);
	_request_line.version = line.substr(0, line.find(' '));
	while (getline(ss, line))
	{
		line = line.substr(0, line.length() - 1);
		_headers[line.substr(0, line.find(':'))] = line.substr(line.find(':') + 2);
	}
	std::cout << "headers: " << std::endl;
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++)
		std::cout << it->first << ": " << it->second << std::endl;
	_request_headers.clear();
}
