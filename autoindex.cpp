#include "Webserv.hpp"

void find_files(const std::string &_uri)
{
	DIR *dir;
	struct dirent *d;
	struct stat s;
	char buff[18];
	std::string file, name, path;
	file = "<!DOCTYPE html>\n<html>\n<head>\n<title>Index of ";
	file += _uri;
	file += "</title>\n</head>\n<body>\n<h1>Index of ";
	file += _uri + "</h1>\n<hr>\n<pre>\n";
	dir = opendir(_uri.c_str());
	if (dir)
	{
		d = readdir(dir);
		while (d)
		{
			name = d->d_name;
			if (name == ".") {
				d = readdir(dir);
				continue ;
			}
			path = _uri + "/" + name;
			if (stat(path.c_str(), &s) != 0) {
				std::cout << "fail" << std::endl;
				closedir(dir);
				return ;
			}
			file += "<a href=" + name;
			if (d->d_type == DT_DIR)
				file += "/>" + name + "/</a>";
			else
				file += ">" + name + "</a>";
			strftime(buff, sizeof(buff), "%d-%b-%Y %H:%M", gmtime(&s.st_mtim.tv_sec));
			file += "    ";
			file += buff;
			file += "\n";
			d = readdir(dir);
		}
		file += "</pre>\n<hr>\n</body>\n</html>";
	}
	closedir(dir);
}