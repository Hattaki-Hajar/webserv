#include "webserver.hpp"

int main(int ac, char *av[])
{
	(void)av;
	if (ac != 2)
	{
		std::cerr << "Error: wrong number of arguments!" << std::endl;
		return (-1);
	}
	try 
	{
		webserver	w;
		config_parser(w, av[1]);
		w.bind_servers();
		w.start();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
