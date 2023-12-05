# pragma once

# include <string>

enum	HTTPMethod {
	GET,
	POST,
	DELETE
};


class RequestLine {
protected:
		std::string	_buffer;
		// int	_Method;
		// std::string	_URI;
		// std::string	_Version;
public:
	RequestLine(/* args */);
	~RequestLine();
};
