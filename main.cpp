#include "Webserv.hpp"
#include <signal.h>

int main(int ac, char *av[])
{
	(void)av;
	std::string file;
	// signal(SIGPIPE, SIG_IGN);
	if (ac > 2)
	{
		std::cerr << "Error: wrong number of arguments!" << std::endl;
		return (-1);
	}
	else if (ac == 1) 
		file = "./config";
	else
		file = av[1];
	try 
	{
		Webserv	w;
		config_parser(w, file.c_str());
		// std::cout << w;
		w.bind_Servers();
		w.start();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}