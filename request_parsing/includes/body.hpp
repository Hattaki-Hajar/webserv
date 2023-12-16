# pragma once

# include <string>
# include <fstream>
# include <iostream>

class BODY {
protected:
	std::ofstream	_buffer;
	size_t			_read;
public:
	BODY();
	~BODY();

	void	initFileStream( void );
};