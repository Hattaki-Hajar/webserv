// #include "Webserv.hpp"

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
// 		return (dir_remover(uri, path));
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