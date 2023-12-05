#include "server.hpp"

server::server(int port)
{
	_port = port;
	_name = "no_name";
	_addr.sin_family = AF_INET;
	// _addr.sin_port = htons(_port);
	// _addr.sin_addr.s_addr = inet_addr(_ip.c_str());
	_socketaddr_len = sizeof(_addr);
}

server::server(const std::string &name)
{
	// _port = 80;
	_name = name;
	_addr.sin_family = AF_INET;
	// _addr.sin_port = htons(_port);
	// _addr.sin_addr.s_addr = inet_addr(host.c_str());
	_socketaddr_len = sizeof(_addr);
}

server::server()
{

}

server::~server()
{
	close(_socket);
}

void	server::bind_server()
{
	if (bind(_socket,(sockaddr *)&_addr, _socketaddr_len) < 0)
		throw std::runtime_error("bind failed!");
	start_listen();
	acceptconnection(_newSocket);
	char	buffer[BUFFER_SIZE] = {0};
	ssize_t	bytesReceived = read(_newSocket, buffer, BUFFER_SIZE);
	if (bytesReceived < 0)
		throw std::runtime_error("read failed!");
	std::cout << buffer << std::endl;
	long unsigned int bytesSent;
	bytesSent = write(_newSocket, _message.c_str(), _message.size());
	if (bytesSent != _message.size())
		throw std::runtime_error("write failed!");
}

void	server::start_listen()
{
	std::cout << "socket: " << _socket << std::endl;
	if (listen(_socket, SOMAXCONN) < 0)
		throw std::runtime_error("listen failed!");
	std::ostringstream ss;
    ss << "\n*** Listening on ADDRESS: " 
        << inet_ntoa(_addr.sin_addr) 
        << " PORT: " << ntohs(_addr.sin_port) 
        << " ***\n\n";
    std::cout << ss.str();
}

void	server::acceptconnection(int &new_socket)
{
	new_socket = accept(_socket, (sockaddr *)&_addr, 
                        &_socketaddr_len);
    if (new_socket < 0)
    {
        std::ostringstream ss;
        ss << 
        "Server failed to accept incoming connection from ADDRESS: " 
        << inet_ntoa(_addr.sin_addr) << "; PORT: " 
        << ntohs(_addr.sin_port);
        throw std::runtime_error("failed to connect!");
    }
}

void	server::set_port(int port)
{
	_port = port;
}

void	server::set_host(const std::string &host)
{
	_ip = host;
}

void	server::set_name(const std::string &name)
{
	_name = name;
}

std::string	&server::get_name(void)
{
	return (_name);
}

void	server::set_error_page(int error, std::string const& path)
{
	_error_pages[error] = path;
}

void	server::set_body_size(size_t size)
{
	_max_body_size = size;
}

unsigned int	&server::get_socketaddr_len()
{
	return (_socketaddr_len);
}
sockaddr_in		&server::get_addr()
{
	return (_addr);
}
int				&server::get_new_socket()
{
	return (_newSocket);
}

void	server::set_new_socket(int socket)
{
	_newSocket = socket;
}

void	server::set_socket(int socket)
{
	_socket = socket;
}

void	server::set_len(void)
{
	_socketaddr_len = sizeof(_addr);
}

void	server::set_addr(int port, std::string const& ip)
{
	_port = port;
	_ip = ip;
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = inet_addr(_ip.c_str());
	_addr.sin_port = htons(_port);
}
