#include "Client.hpp"
#include "Webserv.hpp"

Client::Client() {
	// std::cout << "Client constructor" << std::endl;
	_socket = -1;
	_addr.sin_family = AF_INET;
	_addr_size = sizeof(_addr);
	_event = new epoll_event();
	_event->events = EPOLLIN | EPOLLOUT;
}
Client::Client(int socket) {
	// std::cout << "Client constructor 2" << std::endl;
	_socket = socket;
	_addr.sin_family = AF_INET;
	_addr_size = sizeof(_addr);
	_event = new epoll_event();
	_event->events = EPOLLIN | EPOLLOUT;
}
	/*  Setters  */
void	Client::set_socket(int socket) {
	_socket = socket;
}

void	Client::set_message(std::string const& message) {
	_message = message;
}
void	Client::set_event_fd(int fd) {
	_socket = fd;
	_event->data.fd = fd;
}
	/*  Getters  */
int		Client::get_socket() const {
	return (_socket);
}
const std::string	&Client::get_message() const {
	return (_message);
}
epoll_event	*Client::get_event() const {
	return (_event);
}
	/*  Additional funcs  */
Client::~Client() {
	// std::cout << "Client destructor" << std::endl;
	close(_socket);
	delete _event;
}

