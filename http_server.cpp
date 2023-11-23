#include "http_server.hpp"

server::server(std::string const& ip, int port):_ip(ip)
{
	_port = port;
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(_port);
	_addr.sin_addr.s_addr = inet_addr(_ip.c_str());
	_socketaddr_len = sizeof(_addr);
}
server::~server()
{
	close(_socket);
}

void	server::start()
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0)
		throw std::runtime_error("socket failed!");
	int on = 1;
    setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    setsockopt(_socket, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
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
