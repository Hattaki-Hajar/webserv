/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharrass <aharrass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 15:34:16 by aharrass          #+#    #+#             */
/*   Updated: 2023/12/31 22:15:18 by aharrass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

Response::Response()    {
}

Response::Response(unsigned int status_code, Client &client)
: _status_code(status_code)   {
    _client = &client;
    is_complete = false;
    is_header = false;
    _found_location = false;
    _headers = _client->get_request()->get_headers();
    _uri = _client->get_request()->get_request_line().uri;
    pars_uri();
    bzero(_response_buffer, BUFFER_SIZE);
    _old_uri = _uri;
    _response_length = 1;
    _server_root_path = _client->get_server().get_root();
    _server_index_path = _client->get_server().get_index();
    _server_error_pages = _client->get_server().get_error_pages();
    match_uri();
    std::cout << "old uri = " << _old_uri << std::endl;
    std::cout << "new uri = " << _uri << std::endl;
    _request_line = _client->get_request()->get_request_line();
    
    _error_page = "<!DOCTYPE html>\n<html>\n<head>\n<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\"\n>";
    _error_page += "<title>Error</title>\n<style>\n@import url('https://fonts.googleapis.com/css?family=Press Start 2P');\n";
    _error_page += "*   {\nbackground: black;\n}\n.main-box {\npadding: 0%;\nbox-sizing: border-box;\ndisplay: flex;\n";
    _error_page += "font-family: 'Press Start 2P', cursive;\nheight: 100vh;\nbackground: rgb(0, 0, 0);\njustify-content: center;\n";
    _error_page += "flex-direction: column;\ntext-align: center;\nalign-items: center;\nfont-size: 2rem;\ncolor: #54FE55;\n}\n</style>\n";
    _error_page += "</head>\n<body>\n<div class=\"main-box\">\n...\n</div>\n</body>\n</html>";
}

Response::~Response()   {
}

bool    Response::getIs_complete() const {
    return is_complete;
}

bool    Response::getIs_header() const {
    return is_header;
}

const int& Response::getResponse_length() const {
    return _response_length;
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
                else if (_request_line.method == "DELETE")
                    delete_method();
            }
        }
    }
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
            else    {
                diff_tmp = diff;
                it_tmp = it;
            }
                
        }
    }
    if (it_tmp != locations.end())  {
        _location = it_tmp->second;
        _found_location = true;

        if (!_location.root.empty()) {
            std::string tt = _uri.substr(0, it_tmp->first.length());
            _uri.erase(0, it_tmp->first.length());
            _uri = _location.root + tt + _uri;
        } 
        else if (!_server_root_path.empty()) {
            std::string tt = _uri.substr(0, it_tmp->first.length());
            _uri.erase(0, it_tmp->first.length());
            _uri = _server_root_path + tt + _uri;
        }
        
    }
    else    {
        _uri = _server_root_path + _uri;
    }
}

char* Response::send()    {
    if (!is_header){
        set_headers();
    }
    else {
        set_body();
    }
    return _response_buffer;
}

void    Response::set_body()    {
    if (_status_code != 200)    {
        bzero(_response_buffer, BUFFER_SIZE);
        strcpy(_response_buffer, _error_page.c_str());
        _response_length = _error_page.length();
        is_complete = true;
        // close(_file);
        _file.close();
    }
    else {
        if (_response_length > 0)    {
            bzero(_response_buffer, BUFFER_SIZE);
            _file.read(_response_buffer, BUFFER_SIZE);
            _response_length = _file.gcount();
            // _response_length = read(_file, _response_buffer, BUFFER_SIZE);
            std::cout << "length = " << _response_length << std::endl;
            if (_response_length == 0)  {
                is_complete = true;
                _file.close();
                // close(_file);
            }
        }

        
    }

}

void   Response::set_headers()    {
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
        }
        else if (_status_code == 501)   {
            _status_line = "HTTP/1.1 501 Not Implemented\r\n";
            _content_type = "text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            _error_page.insert(pos, "<p>Error 501!<br>Not Implemented</p>");
        }
        else if (_status_code == 414)   {
            _status_line = "HTTP/1.1 414 Request-URI Too Long\r\n";
            _content_type = "text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            _error_page.insert(pos, "<p>Error 414!<br>Request-URI Too Long</p>");
        }
        else if (_status_code == 413)   {
            _status_line = "HTTP/1.1 413 Request Entity Too Large\r\n";
            _content_type = "text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            _error_page.insert(pos, "<p>Error 413!<br>Request Entity Too Large</p>");
        }
        else if (_status_code == 404)   {
            _status_line = "HTTP/1.1 404 Not Found\r\n";
            _content_type = "text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            _error_page.insert(pos, "<p>Error 404!<br>Not Found</p>");
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
        }
        else if (_status_code == 403)   {
            _status_line = "HTTP/1.1 403 Forbidden\r\n";
            _content_type = "text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            _error_page.insert(pos, "<p>Error 403!<br>Forbidden</p>");
        }
        else if (_status_code == 409)   {
            _status_line = "HTTP/1.1 409 Conflict\r\n";
            _content_type = "text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            _error_page.insert(pos, "<p>Error 409!<br>Conflict</p>");
        }
        else if (_status_code == 204)   {
            _status_line = "HTTP/1.1 204 No Content\r\n";
            _content_type = "text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            _error_page.insert(pos, "<p>Error 204!<br>No Content</p>");
        }
        else if (_status_code == 500)   {
            _status_line = "HTTP/1.1 500 Internal Server Error\r\n";
            _content_type = "text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            _error_page.insert(pos, "<p>Error 500!<br>Internal Server Error</p>");
        }
    }
    // std::cerr << "test" << std::endl;
    _response = _status_line + _response_header + "\r\n";
    bzero(_response_buffer, BUFFER_SIZE);
    strcpy(_response_buffer, _response.c_str());
    is_header = true;
    // std::cerr << _response << std::endl;
    _response_length = _response.length();
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
	file += _old_uri + "</h1>\n<hr>\n<pre>\n<table>\n<tbody>\n";
	dir = opendir(_uri.c_str());
    std::cout << _uri.c_str();
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
			file += "<tr>\n<td><a href=" + name;
			if (d->d_type == DT_DIR)
				file += "/>" + name + "/</a></td>\n";
			else
				file += ">" + name + "</a></td>\n";
			strftime(buff, sizeof(buff), "%d-%b-%Y %H:%M", gmtime(&s.st_mtim.tv_sec));
			file += "<td>";
			file += buff;
			file += "</td>";
            file += "<td>";
            std::stringstream ss;
            std::string tmp;
            ss << s.st_size;
            ss >> tmp;
            if (d->d_type == DT_DIR)
                file += "-";
            else
                file += tmp;
            file += "</td></tr>\n";
			d = readdir(dir);
		}
		file += "</tbody>\n</table>\n<hr>\n</pre>\n</body>\n</html>";
	}
	closedir(dir);
    
}

