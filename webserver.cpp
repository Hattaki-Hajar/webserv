#include "webserver.hpp"

webserver::webserver()
{}

void	webserver::start()
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0)
		throw std::runtime_error("socket failed!");
	int on = 1;
    setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    setsockopt(_socket, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
}

// void	webserver::set_config_file(std::string &name)
// {
// 	_config_file.open(name);
// 	if (!_config_file.good())
// 		throw std::runtime_error("config file failed to open!");
// }


webserver::~webserver()
{}
