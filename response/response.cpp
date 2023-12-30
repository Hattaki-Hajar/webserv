/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharrass <aharrass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 15:34:16 by aharrass          #+#    #+#             */
/*   Updated: 2023/12/30 16:34:05 by aharrass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

Response::Response()    {
}

Response::Response(int status_code, Client &client, std::string file_path)
: _status_code(status_code), _file_path(file_path)   {
    _client = &client;
    _found_location = false;
    _headers = _client->get_request()->get_headers();
    _uri = _client->get_request()->get_request_line().uri;
    pars_uri();
    _old_uri = _uri;
    match_uri();
    // std::cout << "old uri = " << _old_uri << std::endl;
    // std::cout << "new uri = " << _uri << std::endl;
    _server_root_path = _client->get_server().get_root();
    _server_index_path = _client->get_server().get_index();
    _server_error_pages = _client->get_server().get_error_pages();
    _request_line = _client->get_request()->get_request_line();
    
    _error_page = "<!DOCTYPE html>\n<html>\n<head>\n<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\"\n>";
    _error_page += "<title>Error</title>\n<style>\n@import url('https://fonts.googleapis.com/css?family=Press Start 2P');\n";
    _error_page += "*   {\nbackground: black;\n}\n.main-box {\npadding: 0%;\nbox-sizing: border-box;\ndisplay: flex;\n";
    _error_page += "font-family: 'Press Start 2P', cursive;\nheight: 100vh;\nbackground: rgb(0, 0, 0);\njustify-content: center;\n";
    _error_page += "flex-direction: column;\ntext-align: center;\nalign-items: center;\nfont-size: 2rem;\ncolor: #54FE55;\n}\n</style>\n";
    _error_page += "</head>\n<body>\n<div class=\"main-box\">\n...\n</div>\n</body>\n</html>";
	_del_error = 0;
}

Response::~Response()   {
}

int Response::get_resource_type()    {
    if (!check_location(_uri))
        return NOT_FOUND;
    DIR *dir = opendir(_uri.c_str());
    if (dir)    {
        closedir(dir);
        return DIREC;
    }
    std::fstream file(_uri.c_str(), std::ios::in);
    if (!file.fail()){
        file.close();   
        return FILE;
    }
    return NOT_FOUND;
}

void   Response::pars_uri()   {
    std::string prefix = "http://";
    if (_uri.compare(0, prefix.length(), prefix) == 0)   {
        _uri.erase(0, prefix.length());
        size_t p = _uri.find('/', 0);
        if (p == std::string::npos){
            _uri = "/";
        }
        else    {
            _uri.erase(0, p);
        }
    }
    else if (_uri[0] != '/') {
        _status_code = 301;
    }
}

void Response::get()  {
    int type = get_resource_type();
    if (type == NOT_FOUND)   {
        _status_code = 404;
    }
    else if (type == DIREC) {
        if (_uri[_uri.length() - 1] != '/') {
            _old_uri += "/";
            _status_code = 301;
            return;
        }
        else{
            if (!_location.index.empty() || !_server_index_path.empty())   {
                if (!_location.index.empty())
                    _uri += "/" + _location.index;
                else if (!_server_index_path.empty())
                    _uri += "/" + _server_index_path;
                std::cout << _uri << std::endl;
                std::ifstream file(_uri.c_str());
                if (file.fail())    {
                    _status_code = 404;
                    return;
                }
                getline(file, _response_body, '\0');
                _content_type = "text/html";
                return ;
            }
            else  {
                if (_location.autoindex)   {
                    find_files();
                    return ;
                }
            }
        }
    }
}  

// void    Response::post()    {
//     std::string path = _location.upload_path;
//     std::string file_name = _client->get_request()->get_headers()["File-Name"];
//     std::string file_path = path + "/" + file_name;
//     std::string file_content = _client->get_request()->get_body();
//     std::ofstream file(file_path.c_str());
//     if (file.fail())    {
//         _status_code = 409;
//         return ;
//     }
//     file << file_content;
//     file.close();
//     _status_code = 204;
// }

void    Response::post() {
    if (!_found_location) {
        _status_code = 404;
        return ;
    }
    if (!_location.upload_path.empty()) {
        std::string path = _location.upload_path + "/" + _file_path.substr(_file_path.find("/") + 1);
        std::rename(_file_path.c_str(), path.c_str());
        _status_code = 201;
        return;
    }
    int type = get_resource_type();
    if (type == NOT_FOUND) {
        _status_code = 404;
        return;
    }
    if (type == FILE) {
        if (!_location.Cgi.path.empty()) {
            // run cgi on requested file with POST.
            // return _status_code of the cgi;
        }
        else {
            _status_code = 403;
            return;
        }
    } else if (type == DIREC) {
        if (_uri[_uri.length() - 1] != '/') {
            _old_uri += "/";
            _status_code = 301;
            return;
        }
        if (!_location.index.empty()) {
            _status_code = 403;
            return;
        }
        if (!_location.Cgi.path.empty()) {
            // run cgi on requested file with POST.
            // return _status_code of the cgi;
        }
        else {
            _status_code = 403;
            return;
        }
    }
}

void    Response::responde()    {
    if (_status_code == 200)    {
        if (!_found_location && _server_root_path.empty())
            _status_code = 404;
        else if (_found_location)   {
            std::vector<std::string>::iterator it = std::find(_location.methods.begin(), _location.methods.end(), _request_line.method);
            
            if (it == _location.methods.end())
                _status_code = 405;
            else if (_location.return_code > 299 && _location.return_code < 308 )    {
                _old_uri = _location.return_path;
                _status_code = 301;
            }   
            else    {
                if (_request_line.method == "GET")
                    get();
                if (_request_line.method == "POST")
                    post();
            }
        }
    }
    setResponse();
}

void Response::match_uri()  {
    std::map<std::string, location> locations = _client->get_server().get_locations();
    int diff = 0;
    int diff_tmp = 0;
    int comp = 0;
    
    if (_status_code != 200)
        return ;
    std::map<std::string, location>::const_iterator it_tmp = locations.end();
    for(std::map<std::string, location>::const_iterator it = locations.begin(); it != locations.end(); it++)    {
        if (it->first.length() <= _uri.length())    {
            comp = it->first.compare(_uri.substr(0, it->first.length()));
            if (comp)
                continue;
            diff = _uri.length() - it->first.length();
            if (diff == 0)  {
                it_tmp = it;
                diff_tmp = diff;
                break;
            }
            if (it != locations.begin() && comp == 0)    {
                if (diff < diff_tmp)   {
                    it_tmp = it;
                    diff_tmp = diff;
                }
            }
            else
                diff_tmp = diff;
        }
    }
    if (it_tmp != locations.end())  {
        _location = it_tmp->second;
        _found_location = true;
        if (_uri.length() >= it_tmp->first.length() && !_location.root.empty()) {
            std::string tt = _uri.substr(0, it_tmp->first.length());
            _uri.erase(0, it_tmp->first.length());
            _uri = _location.root + tt + _uri;
        } 
    }
    else    {
        _uri = _client->get_server().get_root() + _uri;
    }
}

void Response::setResponse()    {
    if (_status_code == 200)    {
        _status_line = "HTTP/1.1 200 OK\r\n";
        _response_header = "Content-Type: " + _content_type + "\r\n";
    }
    else    {
        int pos;
        std::string tmp = "...";
        pos = _error_page.find(tmp, 0);
        _error_page.erase(pos, 3);
        if (_status_code == 400)    {
            _status_line = "HTTP/1.1 400 Bad Request\r\n";
            _content_type = "text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            _error_page.insert(pos, "<p>Error 400!<br>Bad Request</p>");
            _response_body = _error_page;
        }
        else if (_status_code == 501)   {
            _status_line = "HTTP/1.1 501 Not Implemented\r\n";
            _content_type = "text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            _error_page.insert(pos, "<p>Error 501!<br>Not Implemented</p>");
            _response_body = _error_page;
        }
        else if (_status_code == 414)   {
            _status_line = "HTTP/1.1 414 Request-URI Too Long\r\n";
            _content_type = "text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            _error_page.insert(pos, "<p>Error 414!<br>Request-URI Too Long</p>");
            _response_body = _error_page;
        }
        else if (_status_code == 413)   {
            _status_line = "HTTP/1.1 413 Request Entity Too Large\r\n";
            _content_type = "text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            _error_page.insert(pos, "<p>Error 413!<br>Request Entity Too Large</p>");
            _response_body = _error_page;
        }
        else if (_status_code == 404)   {
            _status_line = "HTTP/1.1 404 Not Found\r\n";
            _content_type = "text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            _error_page.insert(pos, "<p>Error 404!<br>Not Found</p>");
            _response_body = _error_page;
        }
        else if (_status_code == 301)   {
            _status_line = "HTTP/1.1 301 Moved Permanently\r\n";
            _response_header = "Location: " + _old_uri + "\r\n";
        }
        else if (_status_code == 405)   {
            _status_line = "HTTP/1.1 405 Method Not Allowed\r\n";
            _content_type = "text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            _error_page.insert(pos, "<p>Error 405!<br>Method Not Allowed</p>");
            _response_body = _error_page;
        }
        else if (_status_code == 403)   {
            _status_line = "HTTP/1.1 403 Forbidden\r\n";
            _content_type = "text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            _error_page.insert(pos, "<p>Error 403!<br>Forbidden</p>");
            _response_body = _error_page;
        }
        else if (_status_code == 409)   {
            _status_line = "HTTP/1.1 409 Conflict\r\n";
            _content_type = "text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            _error_page.insert(pos, "<p>Error 409!<br>Conflict</p>");
            _response_body = _error_page;
        }
        else if (_status_code == 204)   {
            _status_line = "HTTP/1.1 204 No Content\r\n";
            _content_type = "text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            _error_page.insert(pos, "<p>Error 204!<br>No Content</p>");
            _response_body = _error_page;
        }
        else if (_status_code == 500)   {
            _status_line = "HTTP/1.1 500 Internal Server Error\r\n";
            _content_type = "text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            _error_page.insert(pos, "<p>Error 500!<br>Internal Server Error</p>");
            _response_body = _error_page;
        }
    }
    // std::cerr << "test" << std::endl;
    _response = _status_line + _response_header + "\r\n" + _response_body;
    // std::cerr << _response << std::endl;
    write(_client->get_socket(), _response.c_str(), _response.length());
}

void Response::find_files() {
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
			stat(path.c_str(), &s);
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
    _response_body = file;
}

