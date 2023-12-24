#include "Request.hpp"
/*  constructors destructors  */
Request::Request() {
	// std::string name;
	_size_read = 0;
	_file.open("test.txt", std::ios::out | std::ios::app);
	// if (!_file.is_open())
	// 	std::cout << "debug: file not open!" << std::endl;
	// std::cout << "********* debug: should be here once!" << std::endl;
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
	std::cout << "debug: i = " << i << " bytesread = " << bytesread << std::endl;
	if (!_file.good())
		std::cout << "debug: file not open!" << std::endl;
	_file << buffer + i;
	_file.flush();
	_size_read += bytesread - i;
}
void	Request::parse_request() {
	if (_request_headers.empty())
		return ;
	std::istringstream ss(_request_headers);
	std::string line;
	getline(ss, line);
	_request_line.method = line.substr(0, line.find(' '));
	line.erase(0, line.find(' ') + 1);
	_request_line.uri = line.substr(0, line.find(' '));
	line.erase(0, line.find(' ') + 1);
	_request_line.version = line.substr(0, line.find(' '));
	while (getline(ss, line))
	{
		_headers[line.substr(0, line.find(':'))] = line.substr(line.find(':') + 2);
	}
	_request_headers.clear();
}
