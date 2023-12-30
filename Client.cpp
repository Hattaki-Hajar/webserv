#include "Client.hpp"
#include "Webserv.hpp"

// Client::Client() {
// 	// std::cout << "Client constructor" << std::endl;
// 	_socket = -1;
// 	_addr.sin_family = AF_INET;
// 	_addr_size = sizeof(_addr);
// 	_event = new epoll_event();
// 	_event->events = EPOLLIN | EPOLLOUT;
// }
Client::Client(Server &s):_server(s) {
	_addr.sin_family = AF_INET;
	_addr_size = sizeof(_addr);
	_event = new epoll_event();
	_event->events = EPOLLIN | EPOLLOUT;
	_bytesread = -2;
	bzero(_buffer, BUFFER_SIZE + 1);
	_done_reading = false;
	_status_code = 0;
	// std::cout << "debug: should be here once!" << std::endl;
	_request = new Request();
}
	/*  Setters  */
void	Client::set_socket(int socket) {
	_event->data.fd = socket;
	_event->events = EPOLLIN | EPOLLOUT;
	_socket = socket;
}
void	Client::set_bytesread(ssize_t bytesread) {
	_bytesread = bytesread;
}
void	Client::set_reading_status(bool status) {
	_done_reading = status;
}
	/*  Getters  */
int		Client::get_socket() const {
	return (_socket);
}
ssize_t	Client::get_bytesread() const {
	return (_bytesread);
}
epoll_event	*Client::get_event() const {
	return (_event);
}
const Server	&Client::get_server() const {
	return (_server);
}
char	*Client::get_buffer(void) const {
	return ((char *)_buffer);
}
bool	Client::get_reading_status(void) const {
	return (_done_reading);
}
const Request	*Client::get_request() const {
	return (_request);
}
bool	Client::get_done_reading() const {
	return (_done_reading);
}
	/*  Additional funcs  */
void	Client::clear_buffer() {
	bzero(_buffer, BUFFER_SIZE + 1);
}
void	Client::parse_request() {
	_request->split_request(_buffer, _bytesread);


	if (_request->get_end_of_request()) {
		_done_reading = true;
		_status_code = _request->get_status_code();
	}

	// check if the request body is larger than the max body size.
	if (_request->get_size_read() > get_server().get_max_body_size()) {
		_done_reading = true;
		_status_code = 413;
	}
}
	/*  Destructor  */
Client::~Client() {
	// std::cout << "Client destructor" << std::endl;
	close(_socket);
	delete _event;
	delete _request;
}

