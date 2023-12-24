#pragma once

#include "../Server.hpp"

class Webserv;

void	config_parser(Webserv &w, const char *name);
void	listen_directive(std::string &, Webserv &, int);
void	Server_name_directive(const std::string &, Webserv &, int);
void	error_page_directive(std::string &, Webserv &, int);
void	Client_max_body_size_directive(std::string &, Webserv &, int);
void	location_directive(std::istringstream &, std::string &, Webserv &, int);
void	root_directive(std::string &, Webserv &, int);
void	index_directive(std::string &, Webserv &, int);
