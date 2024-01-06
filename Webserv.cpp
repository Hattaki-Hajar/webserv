#include "Webserv.hpp"
#include <cstring>
#include <algorithm>
#include "response/response.hpp"

Webserv::Webserv()
{
	_event = new epoll_event;
}

void	Webserv::new_connection(Server &s)
{
	int i;
	_Clients.push_back(new Client(s));
	i = _Clients.size() - 1;
	int new_socket = accept(s.get_socket(), (struct sockaddr *)&_Clients[i]->_addr, &_Clients[i]->_addr_size);
	if (new_socket < 0)
	{
		perror("accept");
		throw std::runtime_error("failed to connect1!");
	}
	_Clients[i]->set_socket(new_socket);
	if (set_nonblocking(new_socket) < 0)
		throw std::runtime_error("set_nonblocking failed!");
	if (epoll_ctl(s.get_epfd(), EPOLL_CTL_ADD, _Clients[i]->get_socket(), _Clients[i]->get_event()) < 0)
	{
		perror("epoll_ctl");
		throw std::runtime_error("epoll_ctl failed! 2");
	}
	// std::cout << "fd: " << new_socket << std::endl;
}

int	is_server_socket(std::vector<Server *> &Servers, int socket)
{
	size_t i = 0;
	while (i < Servers.size())
	{
		if (Servers[i]->get_socket() == socket)
			return (i);
		i++;
	}
	return (-1);
}

void	set_up_webserv(std::vector<Server *> &Servers, int epfd)
{
	if (epfd < 0)
		throw std::runtime_error("epoll_create1 failed!");
	for (size_t i = 0; i < Servers.size(); i++)
		set_up_Server(*Servers[i], epfd);
}

int	find_client(std::vector<Client *> &Clients, int socket)
{
	size_t i = 0;
	while (i < Clients.size())
	{
		if (Clients[i]->get_socket() == socket)
			return (i);
		i++;
	}
	return (-1);
}

void	Webserv::check_cgi()
{
	double	time;
	clock_t	end;
	int		status;
	
	for (size_t i = 0; i < _Clients.size(); i++)
	{
		if (_Clients[i]->_cgi && _Clients[i]->_cgi->is_running)
		{
			end = clock();
			if (waitpid(_Clients[i]->_cgi->_pid, &status, WNOHANG) > 0)
			{
				if (WIFEXITED(status))
					_Clients[i]->_response->set_status_code(200);
				else
					_Clients[i]->_response->set_status_code(500);
				_Clients[i]->_cgi->is_running = false;
				_Clients[i]->_cgi->is_complete = true;
			}
			else
			{
				time = (double)(end - _Clients[i]->_cgi->_start) / CLOCKS_PER_SEC;
				if (time >= 30)
				{
					kill(_Clients[i]->_cgi->_pid, SIGKILL);
					_Clients[i]->_response->set_status_code(504);
					_Clients[i]->_cgi->is_running = false;
					_Clients[i]->_cgi->is_complete = true;
				}
			}
		}
	}
}

void	Webserv::start()
{
	int i = 0, client_nb = 0;
	ssize_t bytesread = 0;
	char buffer[BUFFER_SIZE + 1];
	bzero(buffer, BUFFER_SIZE + 1);
	int epfd = epoll_create1(0), event_nb = 0, fd;
	epoll_event	events[MAX_EVENTS];
	set_up_webserv(_Servers, epfd);
	while (1)
	{
		event_nb = epoll_wait(epfd, events, MAX_EVENTS, 0);
		if (event_nb < 0)
			throw std::runtime_error("epoll_wait failed!");
		for (int j = 0; j < event_nb; j++)
		{
			fd = events[j].data.fd;
			i = is_server_socket(_Servers, fd);
			if (i >= 0)
			{
				new_connection(*_Servers[i]);
				continue ;
			}
			client_nb = find_client(_Clients, fd);
			if (events[j].events & EPOLLIN)
			{
				bzero(_Clients[client_nb]->get_buffer(), BUFFER_SIZE + 1);
				bytesread = read(fd, _Clients[client_nb]->get_buffer(), BUFFER_SIZE);
				_Clients[client_nb]->set_bytesread(bytesread);
				if (bytesread <= 0) {
					continue ;
				}
				if (!_Clients[client_nb]->get_done_reading())
					_Clients[client_nb]->parse_request();
				if (_Clients[client_nb]->get_done_reading() && !_Clients[client_nb]->_response)
					_Clients[client_nb]->generateResponse();
				_Clients[client_nb]->clear_buffer();
			}
			if (events[j].events & EPOLLOUT && _Clients[client_nb]->get_done_reading() && !_Clients[client_nb]->_cgi->is_running)
			{
				write(fd, _Clients[client_nb]->_response->send(), _Clients[client_nb]->_response->getResponse_length());
				if (_Clients[client_nb]->_response->getIs_complete())
				{
					epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
					close(fd);
					delete _Clients[client_nb];
					_Clients.erase(_Clients.begin() + client_nb);
				}
			}
			this->check_cgi();
		}
	}
}

void	Webserv::bind_Servers()
{
	size_t i = 0, j, found;
	int port;
	std::string host, server_name;

	while (i < _Servers.size())
	{
		j = 0;
		found = 0;
		port = _Servers[i]->get_port();
		host = _Servers[i]->get_ip();
		server_name = _Servers[i]->get_name();
		while (j < i)
		{
			if (_Servers[j]->get_port() == port && _Servers[j]->get_ip() == host)
				throw std::runtime_error("Error: port and host are the same!");
			j++;
		}
		i++;
		if (found)
			continue ;
		_Servers[i - 1]->bind_Server();
	}
}
	/*  setters  */
void	Webserv::set_new_Server(size_t s)
{
	if (_Servers.size() < s + 1)
		_Servers.push_back(new Server());
}
void	Webserv::set_port(const std::string &port, int s)
{
	int p;
	int i = 0;
	std::string nb;

	if (port.empty())
		return ;
	while (port[i] && isdigit(port[i]))
		nb += port[i++];
	while (port[i] && isspace(port[i]))
		i++;
	p = atoi(nb.c_str());
	if (p < 0 || p > 65535)
		throw std::runtime_error("Error: port is not valid!");
	_Servers[s]->set_port(p);
}

void	Webserv::set_host(const std::string &h, int s)
{
	int i, nb, j = 0;
	if (h.empty())
		return ;
	std::string host = h;
	i = std::count(host.begin(), host.end(), '.');
	if (i != 3)
		throw std::runtime_error("Error: host is not valid!");
	if (h == "localhost")
	{
		_Servers[s]->set_host("127.0.0.1");
		return ;
	}
	char *str = strtok((char *)host.c_str(), ".");
	while (j < 4)
	{
		i = 0;
		while (isdigit(str[i]))
			i++;
		if (str[i])
			throw std::runtime_error("Error: host is not valid 1!");
		nb = atoi(str);
		if (nb < 0 || nb > 255)
			throw std::runtime_error("Error: host is not valid 2!");
		j++;
		str = strtok(0, ".");
	}
	if (j == 4 && str)
		throw std::runtime_error("Error: host is not valid 3!");
	_Servers[s]->set_host(h);
}

void	Webserv::set_name(const std::string &name, int Server)
{
	_Servers[Server]->set_name(name);
}

void	Webserv::set_error_page(int error, std::string const& path, int Server) {
	_Servers[Server]->set_error_page(error, path);
}

void	Webserv::set_body_size(size_t size, int Server) {
	_Servers[Server]->set_body_size(size);
}

void	Webserv::set_Server(int Server) {
	_Servers[Server]->set_addr(_Servers[Server]->get_port(), _Servers[Server]->get_ip());
	_Servers[Server]->set_len();
}

void	Webserv::set_Server_socket(int socket, int Server) {
	_Servers[Server]->set_socket(socket);
}

void	Webserv::set_root(std::string const &path, int Server) {
	_Servers[Server]->set_root(path);
}
void	Webserv::set_index(std::string const &path, int server) {
	_Servers[server]->set_index(path);
}
void	Webserv::set_location(std::string const &path, location &loc, int Server) {
	_Servers[Server]->set_location(path, loc);
}
	/*  getters */
const std::string	&Webserv::get_name(int Server) {
	return (_Servers[Server]->get_name());
}

const Server	&Webserv::get_Server(int Server) const {
	return (*_Servers[Server]);
}

size_t	Webserv::get_size() const {
	return (_Servers.size());
}
/*  << overload for Webserv  */
std::ostream& operator<<(std::ostream &os, const Webserv& s)
{
	size_t i = 0;
	while (i < s.get_size())
	{
		os << " ********* Server index: " << i << std::endl;
		os << s.get_Server(i);
		i++;
	}
	return os;
}

Webserv::~Webserv()
{
	// std::cout << "Webserv destructor called" << std::endl;
	for (size_t i = 0; i < _Servers.size(); i++)
		delete _Servers[i];
	for (size_t i = 0; i < _Clients.size(); i++)
		delete _Clients[i];
	delete _event;
	close(_epfd);
}
