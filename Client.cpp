#include "Client.hpp"
#include "Webserv.hpp"

Client::Client(Server &s):_server(s) {
	_addr.sin_family = AF_INET;
	_addr_size = sizeof(_addr);
	_event = new epoll_event();
	_event->events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP;
	_bytesread = -2;
	bzero(_buffer, BUFFER_SIZE + 1);
	_done_reading = false;
	_request = new Request();
	_request->_max_body_size = s.get_max_body_size();
	this->start = clock();
	_request->set_time_start(&start);
	_cgi = 0;
	_EPOLL = false;

	this->timeout = false;
	this->_response = 0;
}

void Client::generateResponse() {
	if (this->timeout)
		_request->_status_code = 408;
	// std::cout << "client status code = " << _request->_status_code << std::endl;
	this->_response = new Response(_request->_status_code, *this);
	this->_cgi = new Cgi(_request->get_headers(), this->_response);
	this->_response->set_cgi(this->_cgi);
	this->_response->set_file_path(this->_request->get_file_path());
	this->_response->responde();
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
	if (_bytesread <= 0)
		return ;
	*(this->_request->time_start) = clock();

	_request->split_request(_buffer, _bytesread);

	if (_request->get_end_of_request())
		_done_reading = true;
}
	/*  Destructor  */
Client::~Client() {
	// std::cout << "Client destructor" << std::endl;
	close(_socket);
	delete _event;
	delete _request;
	// if (_cgi)
		delete _cgi;
	delete _response;
}

