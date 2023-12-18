#include "client.hpp"
#include "webserver.hpp"

client::client() {
	_socket = -1;
	_addr.sin_family = AF_INET;
	_addr_size = sizeof(_addr);
}
	/*  Setters  */
void	client::set_socket(int socket) {
	_socket = socket;
}

void	client::set_message(std::string const& message) {
	_message = message;
}
	/*  Getters  */
int		client::get_socket() const {
	return (_socket);
}

const std::string	&client::get_message() const {
	return (_message);
}
/*  Additional funcs  */
void	client::acceptconnection(server &s) {
	_socket = accept(s.get_socket(), (struct sockaddr *)&_addr, &_addr_size);
	if (_socket < 0)
		throw std::runtime_error("Error: accept failed!");
	// set_socket(_socket);
}

client::~client() {
	close(_socket);
}

