#include "http_server.hpp"
# include "HTTPRequest.hpp"

int main()
{
	try 
	{
		server	s("127.0.0.1", 1234);
		s.start();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	// std::cout << "adf" << SP << "asdf";
}
