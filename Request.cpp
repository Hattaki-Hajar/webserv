#include "Request.hpp"
# include <cstdio>
/*  constructors destructors  */
Request::Request() {
	_size_read = 0;
	_chunk_read = 0;
	_first_chunk_size = 0;
	_chunks_size = 0;
	_remaining = NULL;
	_remaining_size = 0;
	_file.open("original.mp4", std::ios::out | std::ios::app);
	_headers_read = false;
	_end_of_request = false;
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
bool	Request::get_end_of_request() const {
	return (_end_of_request);
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
						this->parse_request();
						i += 4;
						break ;
					}
		_request_headers += buffer[i];
		i++;
	}
	if (!_file.good())
		std::cout << "debug: file not open!" << std::endl;
	if (get_headers().find("Transfer-Encoding") != get_headers().end() && get_headers()["Transfer-Encoding"] == "chunked") {
		if (!_chunks_size) {
			if (_remaining)	{
				char *tmp = new char[bytesread - i];
 				ssize_t	j = 0;
				for (; j < bytesread - i; j++)
					tmp[j] = buffer[i + j];
				buffer = new char[_remaining_size + bytesread - i];
				j = 0;
				while (j < _remaining_size) {
					buffer[j] = _remaining[j];
					j++;
				}
				ssize_t k = 0;
				while (k < bytesread - i) {
					buffer[j] = tmp[k];
					j++;
					k++;
				}
				bytesread += _remaining_size;
				i = 0;
				if (buffer[i] == '\r' && buffer[i + 1] == '\n') {
					i += 2;
				}
				if (_remaining[i] == '0' && _remaining[i + 1] == '\r' && _remaining[i + 2] == '\n') {
					_end_of_request = true;
					std::cout << "end of request" << std::endl;
					return ;
				}
				delete [] tmp;
				delete [] _remaining;
				_remaining = NULL;
			}
			char	line[16];
			bzero(line, 16);
			ssize_t k = 0;
			for (; k < 15; i++, k++) {
				if (buffer[i] == '\r' && buffer[i + 1] == '\n') {
					i += 2;
					break;
				}
				line[k] = buffer[i];
			}
			std::stringstream hex;
			hex << std::hex << line;
			hex >> _chunks_size;
			if (_chunks_size == 0) {
				std::cout << "end of request" << std::endl;
				_end_of_request = true;
				return ;
			}
		}
		while (_chunk_read < _chunks_size && i < bytesread) {
				_file.put(buffer[i]);
				// if (buffer[i])
				_file.flush();
				_chunk_read++;
				i++;
		}
		if (_chunk_read  == _chunks_size && i < bytesread) {
			// i++;
			// std::cout << "READ" << std::endl;
			_chunk_read = 0;
			_chunks_size = 0;
			_remaining = new char[bytesread - i];
			_remaining_size = bytesread - i;
			int j = 0;
			while(i < bytesread) {
				_remaining[j] = buffer[i];
				std::cout << "remaining[" << j << "]:" << _remaining[j] << std::endl;
				std::cout << "buffer[" << i << "]:" << buffer[i] << std::endl;
				j++;
				i++;
			}
			if (_remaining_size >= 5) {
				if (_remaining[0] == '\r' && _remaining[1] == '\n' && _remaining[2] == '0' && _remaining[3] == '\r' && _remaining[4] == '\n') {
					_end_of_request = true;
					std::cout << "end of request" << std::endl;
					return ;
				}
			}
		}
	}
	else {
		_file.write(buffer + i, bytesread - i);
		_file.flush();
		_size_read += bytesread - i;
		if (get_size_read() == atol(get_headers()["Content-Length"].c_str())) {
			_end_of_request = true;
			std::cout << "end of request" << std::endl;
			return ;
		}
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
