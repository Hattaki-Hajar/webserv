#include "../Webserv.hpp"

void	check_line(std::string &line, int i, std::string &path)
{
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i] == '=')
	{
		i++;
		while (line[i] && isspace(line[i]))
			i++;
	}
	if (line[i] != '/')
		throw std::runtime_error("Error: config file is not valid location1!");
	while (line[i] && !isspace(line[i]))
		path += line[i++];
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i++] != '{')
		throw std::runtime_error("Error: config file is not valid location2!");
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i])
		throw std::runtime_error("Error: config file is not valid location3!");
}

void	allow_directive(std::string &option, location &loc)
{
	std::string method;
	size_t i = 0;

	while (option[i] && !isspace(option[i]))
		method += option[i++];
	if (method != "GET" && method != "POST" && method != "DELETE")
		throw std::runtime_error("Error: config file is not valid location4!");
	while (option[i] && isspace(option[i]))
		i++;
	if (option[i])
		throw std::runtime_error("Error: config file is not valid location5!");
	loc.methods.push_back(method);	
}

void	root_dir(std::string &option, location &loc)
{
	std::string path;
	size_t i = 0;

	while (option[i] && !isspace(option[i]))
		path += option[i++];
	while (option[i] && isspace(option[i]))
		i++;
	if (option[i])
		throw std::runtime_error("Error: config file is not valid loc_root2!");
	loc.root = path;
}

void	autoindex_directive(std::string &option, location &loc)
{
	std::string autoindex;
	size_t i = 0;

	while (option[i] && !isspace(option[i]))
		autoindex += option[i++];
	if (autoindex != "on" && autoindex != "off")
		throw std::runtime_error("Error: config file is not valid loc_autoindex!");
	if (autoindex == "on")
		loc.autoindex = true;
	else
		loc.autoindex = false;
	while (option[i] && isspace(option[i]))
		i++;
	if (option[i])
		throw std::runtime_error("Error: config file is not valid loc_autoindex2!");
}

void	index_directive(std::string &option, location &loc)
{
	std::string index;
	size_t i = 0;

	while (option[i] && !isspace(option[i]))
		index += option[i++];
	while (option[i] && isspace(option[i]))
		i++;
	if (option[i])
		throw std::runtime_error("Error: config file is not valid loc_index2!");
	loc.index = index;
}

void	return_directive(std::string &option, location &loc)
{
	std::string return_code;
	size_t i = 0, j = 0;

	while (option[i] && !isspace(option[i]))
		return_code += option[i++];
	while (option[i] && isspace(option[i]))
		i++;
	while (j < 3 && return_code[j] && isdigit(return_code[j]))
		j++;
	if (return_code[j] || return_code.empty() || !option[i])
		throw std::runtime_error("Error: config file is not valid loc_return1!");
	loc.return_code = atoi(return_code.c_str());
	if (loc.return_code < 200 || loc.return_code > 307 || (loc.return_code > 206 && loc.return_code < 300))
		throw std::runtime_error("Error: config file is not valid loc_return2!");
	while (option[i] && !isspace(option[i]))
		loc.return_path += option[i++];
	while (option[i] && isspace(option[i]))
		i++;
	if (option[i])
		throw std::runtime_error("Error: config file is not valid loc_return3!");	
}

void	loc_body_size_directive(std::string &option, location &loc)
{
	std::string size;
	int r_size;
	size_t i = 0, j = 0;

	while (option[i] && isdigit(option[i]))
		size += option[i++];
	if ((option[i] != 'k' && option[i] != 'K' && option[i] != 'm' && option[i]!= 'M'
		&& option[i] != 'g' && option[i] != 'G' && option[i]) || size.empty())
		throw std::runtime_error("Error: config file is not valid size1!");
	while (size[j] && isdigit(size[j]))
		j++;
	if (size[j])
		throw std::runtime_error("Error: config file is not valid size2!");
	r_size = atoi(size.c_str());
	if (option[i] == 'k' || option[i] == 'K')
	{
		r_size *= 1024;
		i++;
	}
	else if (option[i] == 'm' || option[i] == 'M')
	{
		r_size *= 1024 * 1024;
		i++;
	}
	else if (option[i] == 'g' || option[i] == 'G')
	{
		r_size *= 1024 * 1024 * 1024;
		i++;
	}
	else
		r_size *= 1;
	loc.max_body_size = r_size;
	while (option[i] && isspace(option[i]))
		i++;
	if (option[i])
		throw std::runtime_error("Error: config file is not valid size3!");
}

void	init_location(location &loc)
{
	loc.max_body_size = -1;
	loc.return_code = -1;
	loc.autoindex = false;
	loc.root = "";
	loc.index = "";
	loc.return_path = "";
}

void	get_option(std::string &line, size_t i, std::string &option)
{
	while (line[i] && isspace(line[i]))
		i++;
	while (line[i] && line[i] != ';')
		option += line[i++];
	if (!line[i] || line[i++] != ';')
		throw std::runtime_error("Error: config file is not valid location!");
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i])
		throw std::runtime_error("Error: config file is not valid location!");
}

void	find_directive(std::string & directive, std::string &option, location &loc)
{
	if (directive == "allow")
		allow_directive(option, loc);
	else if (directive == "root")
		root_dir(option, loc);
	else if (directive == "autoindex")
		autoindex_directive(option, loc);
	else if (directive == "index")
		index_directive(option, loc);
	else if (directive == "return")
		return_directive(option, loc);
	else if (directive == "client_max_body_size")
		loc_body_size_directive(option, loc);
	else if (!directive.empty())
		throw std::runtime_error("Error: config file is not valid location: unknown directive!");
}

void	location_directive(std::istringstream &ss, std::string &line, Webserv &w, int Server)
{
	std::string path, directive, option;
	location loc;
	int open_bracket = 0, close_bracket = 0, i = 0;

	check_line(line, i, path);
	init_location(loc);
	if (line.find("}") != std::string::npos)
		close_bracket++;
	if (line.find("{") != std::string::npos)
		open_bracket++;
	while (std::getline(ss, line))
	{
		i = 0;
		if (line.find("}") != std::string::npos)
		{
			close_bracket++;
			if (open_bracket == close_bracket)
				break ;
		}
		if (line.find("{") != std::string::npos)
			open_bracket++;
		while (line[i] && isspace(line[i]))
			i++;
		if (line[i] == '#')
			continue ;
		while (line[i] && !isspace(line[i]))
			directive += line[i++];
		get_option(line, i, option);
		find_directive(directive, option, loc);
		directive.clear();
		option.clear();
	}
	w.set_location(path, loc, Server);
}