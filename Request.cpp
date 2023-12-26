#include "Request.hpp"
# include <cstdio>
/*  constructors destructors  */
Request::Request() {
	_size_read = 0;
	_chunk_read = 0;
	_first_chunk_size = 0;
	_chunks_size = 0;
	_remaining = NULL;
	_file.open("test.mp4", std::ios::out | std::ios::app);
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
		if (!_chunks_size) {
			if (_remaining)	{
				buffer = new char[strlen(_remaining) + bytesread];
				bzero(buffer, strlen(_remaining) + bytesread);
				strcat(buffer, _remaining);
				i = 0;
				bytesread += strlen(_remaining);
				delete [] _remaining;
				_remaining = NULL;
			}
			std::string line;
			std::istringstream ss(buffer + i);
			getline(ss, line);
			std::stringstream hex;
			hex << std::hex << line.substr(0, line.length() - 1);
			i += line.length() + 1;
			std::cout << "debug: buffer = " << buffer + 5 << std::endl;
			hex >> _chunks_size;
			std::cout << "debug: chunks_size = " << _chunks_size << std::endl;
			// if (_chunks_size == 0) {
			// 	_end_of_request = true;
			// 	_file.close();
			// 	return ;
			// }
			while (_chunk_read < _chunks_size && i < bytesread) {
				_file.put(buffer[i]);
				_file.flush();
				_chunk_read++;
				i++;
				std::cout << "debug in first while: _chunks_size =<" << _chunks_size << ">, chunk read<" << _chunk_read << ">" << std::endl;
			}
		}
		else if (_chunk_read < _chunks_size && i < bytesread) {
			while (_chunk_read < _chunks_size && i < bytesread) {
				_file.put(buffer[i]);
				_file.flush();
				_chunk_read++;
				std::cout << "debug in second while: _chunks_size =<" << _chunks_size << ">, chunk read<" << _chunk_read << ">" << std::endl;
				i++;
			}
		}
		if (_chunk_read == _chunks_size && i < bytesread) {
			i += 2;
			std::cout << "In the if statement" << std::endl;
			_chunk_read = 0;
			_chunks_size = 0;
			// if (_remaining)
			// 	delete [] _remaining;
			_remaining = new char[bytesread - i];
			int j = 0;
			while(i < bytesread) {
				_remaining[j] = buffer[i];
				j++;
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
