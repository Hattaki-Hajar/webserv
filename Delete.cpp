#include "Webserv.hpp"
#include "./response/response.hpp"

void	Response::clear_dir(const std::string &dir_path)
{
	DIR				*dir;
	struct dirent	*d;
	std::string		path, name;
	if (_del_error)
		return ;
	dir = opendir(dir_path.c_str());
	if (!dir)
		return ;
	d = readdir(dir);
	while (d)
	{
		name = d->d_name;
		path = dir_path + d->d_name;
		if (name == "." || name == "..")
		{
			d = readdir(dir);
			continue ;
		}
		if (std::remove(path.c_str()))
		{
			std::cout << "debug: errno = " << errno << std::endl;
			std::cout << "debug: path: " << path << std::endl;
			if (errno == EEXIST || errno == ENOTEMPTY)
			{
				std::cout << "debug: should be here once" << std::endl;
				clear_dir(path + "/");
				if (std::remove(path.c_str()))
				{
					_del_error = 403;
					return ;
				}
			}
			else if (errno == EACCES)
			{
				_del_error = 403;
				return ;
			}
			else
			{
				_del_error = 500;
				return ;
			}	
		}
		if (_del_error)
			return;
		d = readdir(dir);
	}
}

void	Response::delete_method()
{
	_status_code = 204;
	int type = get_resource_type();
	if (type == NOT_FOUND)
		_status_code = 404;
	else if (type == FILE) {
		std::cout << "debug: found file" << std::endl;
		if (std::remove(_uri.c_str()))
		{
			_status_code = 403;
			return ;
		}
	}
	else {
		std::cout << "debug: found dir" << std::endl;
		if (_uri[_uri.length() - 1] != '/')
		{
			_status_code = 409;
			return ;
		}
		clear_dir(_uri.c_str());
		if (_del_error)
		{
			_status_code = _del_error;
			return ;
		}
		if (std::remove(_uri.c_str()))
			_status_code = 403;
	}
}

// int	file_finder(const std::string &name, const std::string path)
// {
// 	DIR	*dir;
// 	struct dirent *d;
// 	std::string	file;
// 	dir = opendir(path.c_str());
// 	if (dir)
// 	{
// 		d = readdir(dir);
// 		while (d)
// 		{
// 			file = d->d_name;
// 			if (file == name)
// 			{
// 				closedir(dir);
// 				if (d->d_type == DT_DIR)
// 					return (2);
// 				else
// 					return (1);
// 				break;
// 			}
// 			d = readdir(dir);
// 		}
// 	}
// 	closedir(dir);
// 	return (0);
// }

// int	dir_remover(const std::string &uri, const std::string &path)
// {
// 	if (!uri.empty() && uri[uri.length() - 1] != '/')
// 		return 409;
// 	/* add cgi */
	
// }

// int	delete_method(const std::string &uri, location &loc, const std::string &location_path)
// {
// 	std::string path, temp;
// 	int	found = 0;
// 	if (!loc.root.empty())
// 		path += loc.root;
// 	temp = uri.substr(location_path.length() + 1, uri.length());
// 	if (uri[uri.length() - 1] == '/')
// 	{
// 		temp.erase(temp.length() - 1);
// 		std::cout << temp << std::endl;
// 	}
// 	found = file_finder(temp, path);
// 	if (!found)
// 		return 404;
// 	if (temp[0] && temp[0] != '/' && path[path.length() - 1] && path[path.length() - 1] != '/')
// 		path += "/";
// 	path += temp;
// 	if (found == 1) {
// 		/* add cgi */
// 		if (std::remove(path.c_str()))
// 			return 403;
// 		return 204;
// 	}
// 	else {
// 		if (std::remove(path.c_str()))
// 		{
// 			std::cout << "error" << std::endl;
// 			return -1;
// 		}
// 		// command = "rm -r " + path;
// 		// /* add cgi */
// 		// if (std::system(command.c_str()))
// 		// 	return 204;
// 	}
// 	return 0;
// }

// int main()
// {
// 	location loc;
// 	loc.root = "/nfs/homes/hhattaki/Desktop/webserv";
// 	std::cout << delete_method("/webserv/tist", loc, "/webserv") << std::endl;
// }