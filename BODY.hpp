# pragma once

# include <string>
# include <fstream>

class BODY {
protected:
	std::ofstream	_buffer;
public:
	BODY(/* args */);
	~BODY();
};