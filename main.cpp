#include "webserver.hpp"

int main(int ac, char *av[])
{
	if (ac != 2)
	{
		std::cout << "Error: wrong number of arguments!" << std::endl;
		return (-1);
	}
	try 
	{
		webserver	w;
		config_parser(w, av[1]);
		// server	s("127.0.0.1", 1234);
		// s.start();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
