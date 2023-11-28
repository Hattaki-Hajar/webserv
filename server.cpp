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
{}

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
