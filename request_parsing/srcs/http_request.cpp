# include "../includes/http_request.hpp"

HTTPRequest::HTTPRequest( std::string SocketBuffer )
	: _SocketBuffer( SocketBuffer ) {
		std::ofstream	mhamed("dsf");
		mhamed << SocketBuffer;
}

HTTPRequest::~HTTPRequest() {}

void	HTTPRequest::split( void ) {

	std::size_t	__HalfPos = _SocketBuffer.find(DCRLF);
	if ( __HalfPos == std::string::npos ) {
		std::cerr << "400 Bad request." << std::endl;
		return;
	}
	std::string	__requestHalf = _SocketBuffer.substr(0, __HalfPos);

	std::size_t	__ifPos = __requestHalf.find(CRLF);
	RequestLine::_buffer = __requestHalf.substr(0, __ifPos);
	HeaderField::_buffer = __requestHalf.substr(__ifPos + 2, __HalfPos);

	std::string	__ifBody = _SocketBuffer.c_str() + __HalfPos + 4;

	if ( !__ifBody.empty() ) {
		BODY::initFileStream();
		for ( int i = 0; __ifBody[i]; i++ ) {
			BODY::_buffer << __ifBody[i];
			BODY::_read++;
		}
	}
}

void	HTTPRequest::parse( void ) {
	split();

	// std::cout << std::endl << "The request_line<" << RequestLine::_buffer << ">" << std::endl;
	// std::cout << std::endl << "The header_fields<" << HeaderField::_buffer << ">" << std::endl;
	// std::cout << std::endl << "The body size<" << BODY::_read << ">" << std::endl;

}