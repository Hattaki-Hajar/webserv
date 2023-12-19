#include "Webserv.hpp"

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
		Webserv	w;
		config_parser(w, av[1]);
		std::cerr << "here" << std::endl;
		w.bind_Servers();
		w.start();
		std::cerr << "here" << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}