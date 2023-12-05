# pragma once

/*
	An http_request structure:
		-> Request Line: Method SP Request-URI SP HTTP-Version CRLF

		-> Header Fields: Key: Value CRLF
		CRLF

		-> An Optional Body
*/

# include <cstring>
# include <iostream>

# include "http_server.hpp"
# include "RequestLine.hpp"
# include "HeaderField.hpp"
# include "BODY.hpp"

#if ! defined SP
# define SP " "
# endif

# if ! defined CRLF
# define CRLF "\r\n"
# endif

# if ! defined CR
# define CR '\r'
# endif

# if ! defined LF
# define LF '\n'
# endif

class HTTPRequest : public RequestLine, public HeaderField, public BODY {
private:
		char					*_SocketBuffer;
		server					_Server;
		// struct HTTPRequestStructure {
		// 	RequestLine	*request_line;
		// 	// HeaderField	header_fields;
		// 	// BODY		body;
	// };

public:
	HTTPRequest( char*, server );
	~HTTPRequest();

	void	split( void );
	void	parse( void );

};
