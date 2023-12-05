# include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest( char* SocketBuffer, server Server)
	: _SocketBuffer( SocketBuffer ), _Server(Server) {
		std::cout << "Start of Request:" << std::endl;
		std::cout << _SocketBuffer << std::endl;
		std::cout << "End of Request.\n" << std::endl;
}

HTTPRequest::~HTTPRequest() {}

void	HTTPRequest::split( void ) {
	for ( int i = 0; _SocketBuffer[i]; i++ ) {
		
	}
}

void	HTTPRequest::parse( void ) {
	split();
	std::cout << RequestLine::_buffer << std::endl;
	std::cout << HeaderField::_buffer << std::endl;
}