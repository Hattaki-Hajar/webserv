# include "../includes/body.hpp"

BODY::BODY( ) : _read(0) {}

BODY::~BODY() {
}

void    BODY::initFileStream( void ) {
    _buffer.open("body", std::ofstream::out | std::ofstream::trunc);
    if ( !_buffer.is_open() )
        std::cerr << "Failed to open the FileStream." << std::endl;
}
