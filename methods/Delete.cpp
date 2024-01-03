#include "../Webserv.hpp"
#include "../response/response.hpp"

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
			if (errno == EEXIST || errno == ENOTEMPTY)
			{
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
		if (std::remove(_uri.c_str()))
		{
			_status_code = 403;
			return ;
		}
	}
	else {
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
