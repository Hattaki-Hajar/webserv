# pragma once

/*
	An http_request structure:
		-> Request Line: Method SP Request-URI SP HTTP-Version CRLF

		-> Header Fields: Key: 
					-> Value CRLF
					-> Value CRLF
					-> Value \r\n
							 .
		\r\n

		-> An Optional Body


		GET\r\n
		\r\n
*/

# include <cstring>
# include <iostream>

# include "../../server.hpp"
# include "request_line.hpp"
# include "header_field.hpp"
# include "body.hpp"

# define CRLF "\r\n"
# define DCRLF "\r\n\r\n"
# define SP ' '
# define CR '\r'
# define LF '\n'

class HTTPRequest : public RequestLine, public HeaderField, public BODY {
private:
		std::string				_SocketBuffer;
		// server					_Server;
		// struct HTTPRequestStructure {
		// 	RequestLine	*request_line;
		// 	// HeaderField	header_fields;
		// 	// BODY		body;
	// };

public:
	HTTPRequest( std::string );
	~HTTPRequest();

	void	split( void );
	void	parse( void );

};
