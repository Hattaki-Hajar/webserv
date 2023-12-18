#include "../webserver.hpp"

void	check_line(std::string &line, size_t i, std::string &path)
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

void	allow_directive(std::string &line, size_t i, location &loc)
{
	std::string method;

	while (line[i] && isspace(line[i]))
		i++;
	while (line[i] && !isspace(line[i]) && line[i] != ';')
		method += line[i++];
	if (method != "GET" && method != "POST" && method != "DELETE")
		throw std::runtime_error("Error: config file is not valid location4!");
	loc.methods.push_back(method);
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i] != ';')
		throw std::runtime_error("Error: config file is not valid location5!");
}

void	root_dir(std::string &line, size_t i, location &loc)
{
	std::string path;

	while (line[i] && isspace(line[i]))
		i++;
	while (line[i] && !isspace(line[i]) && line[i] != ';')
		path += line[i++];
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i] != ';')
		throw std::runtime_error("Error: config file is not valid loc_root2!");
	loc.root = path;
}

void	autoindex_directive(std::string &line, size_t i, location &loc)
{
	std::string autoindex;

	while (line[i] && isspace(line[i]))
		i++;
	while (line[i] && !isspace(line[i]) && line[i] != ';')
		autoindex += line[i++];
	if (autoindex != "on" && autoindex != "off")
		throw std::runtime_error("Error: config file is not valid loc_autoindex!");
	if (autoindex == "on")
		loc.autoindex = true;
	else
		loc.autoindex = false;
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i] != ';')
		throw std::runtime_error("Error: config file is not valid loc_autoindex2!");
}

void	index_directive(std::string &line, size_t i, location &loc)
{
	std::string index;

	while (line[i] && isspace(line[i]))
		i++;
	while (line[i] && !isspace(line[i]) && line[i] != ';')
		index += line[i++];
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i] != ';')
		throw std::runtime_error("Error: config file is not valid loc_index2!");
	loc.index = index;
}

void	return_directive(std::string &line, size_t i, location &loc)
{
	std::string return_code;
	int j = 0;

	while (line[i] && isspace(line[i]))
		i++;
	while (line[i] && !isspace(line[i]))
		return_code += line[i++];
	while (line[i] && isspace(line[i]))
		i++;
	while (j < 3 && return_code[j] && isdigit(return_code[j]))
		j++;
	if (return_code[j])
		throw std::runtime_error("Error: config file is not valid loc_return1!");
	loc.return_code = atoi(return_code.c_str());
	if (loc.return_code < 300 || loc.return_code > 308)
		throw std::runtime_error("Error: config file is not valid loc_return2!");
	while (line[i] && !isspace(line[i]))
		loc.return_path += line[i++];
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i] != ';')
		throw std::runtime_error("Error: config file is not valid loc_return3!");
}

void	loc_body_size_directive(std::string &line, size_t i, location &loc)
{
	std::string size;
	int j = 0, r_size;

	while (line[i] && isspace(line[i]))
		i++;
	while (line[i] && isdigit(line[i]))
		size += line[i++];
	if ((line[i] != 'k' && line[i] != 'K' && line[i] != 'm' && line[i]!= 'M'
		&& line[i] != 'g' && line[i] != 'G' && line[i] != ';') || size.empty())
		throw std::runtime_error("Error: config file is not valid size1!");
	while (size[j] && isdigit(size[j]))
		j++;
	if (size[j])
		throw std::runtime_error("Error: config file is not valid size2!");
	r_size = atoi(size.c_str());
	if (line[i] == 'k' || line[i] == 'K')
		r_size *= 1024;
	else if (line[i] == 'm' || line[i] == 'M')
		r_size *= 1024 * 1024;
	else if (line[i] == 'g' || line[i] == 'G')
		r_size *= 1024 * 1024 * 1024;
	else
		r_size *= 1;
	i++;
	loc.max_body_size = r_size;
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i] && line[i] != ';')
		throw std::runtime_error("Error: config file is not valid size3!");
}

void	location_directive(std::istringstream &ss, std::string &line, webserver &w, int server)
{
	std::string path, directive;
	location loc;
	int open_bracket = 0, close_bracket = 0, i = 0;

	check_line(line, i, path);
	loc.max_body_size = -1;
	loc.return_code = -1;
	loc.autoindex = false;
	loc.root = "";
	loc.index = "";
	loc.return_path = "";
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
		while (line[i] && !isspace(line[i]))
			directive += line[i++];
		if (directive == "allow")
			allow_directive(line, i, loc);
		if (directive == "root")
			root_dir(line, i, loc);
		if (directive == "autoindex")
			autoindex_directive(line, i, loc);
		if (directive == "index")
			index_directive(line, i, loc);
		if (directive == "return")
			return_directive(line, i, loc);
		if (directive == "client_max_body_size")
			loc_body_size_directive(line, i, loc);
		directive.clear();
	}
	w.set_location(path, loc, server);
}