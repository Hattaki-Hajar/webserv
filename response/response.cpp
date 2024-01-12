/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharrass <aharrass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 15:34:16 by aharrass          #+#    #+#             */
/*   Updated: 2024/01/12 03:13:33 by aharrass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

Response::Response()    {
}

Response::Response(unsigned int status_code, Client &client)
: _status_code(status_code)   {
    _client = &client;
    _total_response_body_length = 0;
    _error_file_good = 0;
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
    // std::cout << "---------------------------------------" << std::endl;
    // std::cout << "old uri = " << _old_uri << std::endl;
    // std::cout << "new uri = " << _uri << std::endl;
    // std::cout << "---------------------------------------" << std::endl;
    _request_line = _client->get_request()->get_request_line();
    
    fill_extentions();
    fill_error_line();
    
    _cgi = NULL;
    _error_page = "<!DOCTYPE html>\n<html>\n<head>\n<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\"\n>";
    _error_page += "<title>Error</title>\n<style>\n@import url('https://fonts.googleapis.com/css?family=Press Start 2P');\n";
    _error_page += "*   {\nbackground: black;\n}\n.main-box {\npadding: 0%;\nbox-sizing: border-box;\ndisplay: flex;\n";
    _error_page += "font-family: 'Press Start 2P', cursive;\nheight: 100vh;\nbackground: rgb(0, 0, 0);\njustify-content: center;\n";
    _error_page += "flex-direction: column;\ntext-align: center;\nalign-items: center;\nfont-size: 2rem;\ncolor: #54FE55;\n}\n</style>\n";
    _error_page += "</head>\n<body>\n<div class=\"main-box\">\n...\n</div>\n</body>\n</html>";
	// set_cgi();
}

Response::~Response()   {
	// delete _cgi;
}

// void    Response::set_cgi() {
//     // for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)    {
//     //     std::cout << it->first + " = " + it->second << std::endl;
//     // }
// 	this->_cgi = new Cgi(_headers, this);
// }

void    Response::set_file_path(const std::string &path)
{
    _file_path = path;
}

void    Response::set_status_code(unsigned int status)  {
    this->_status_code = status;
}

void	Response::set_cgi(Cgi *cgi)	{
	this->_cgi = cgi;
}

void    Response::set_file_name(std::string name)   {
    this->_file_name = name;
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

const std::string&   Response::get_old_uri() const  {
    return (this->_old_uri);
}

const std::string&   Response::get_uri() const  {
    return (this->_uri);
}

const std::string&   Response::get_method() const  {
    return (this->_request_line.method);
}

Client* Response::get_client() const   {
    return (this->_client);
}

const location& Response::get_location() const   {
    return (this->_location);
}

const std::string& Response::get_query() const   {
    return (this->_query);
}


int Response::get_resource_type()    {
    if (!check_location(_uri))
        return NOT_FOUND;
    DIR *dir = opendir(_uri.c_str());
    if (dir)    {
        closedir(dir);
        return DIREC;
    }
    std::fstream file(_uri.c_str());
    if (!file.fail()){
        file.close();   
        return FILE;
    }
    return NOT_FOUND;
}

std::string Response::get_ext() const   {
    size_t pos = _uri.rfind(".");
    
    if (pos == std::string::npos)
        return ("");
    else
        return _uri.substr(pos + 1, std::string::npos);
}

void    Response::fill_error_line() {
    const int code[] = {400, 501, 414, 413, 404, 405, 403, 408, 409, 500, 504};
    const char* line[] = {"Bad Request", "Not Implemented", "Request-URI Too Long",
        "Request Entity Too Large", "Not Found", "Method Not Allowed", "Forbidden",
        "Request Time-out", "Conflict", "Internal Server Error", "Gateway Timeout"};
    size_t size = sizeof(line) / sizeof(line[0]);
    std::string tmp;
    for(size_t i = 0; i < size; i++)    {
        std::stringstream ss;
        ss << code[i];
        ss >> tmp;
        _error_line[code[i]] = "HTTP/1.1 " + tmp + " " + line[i];
    }
}

void    Response::fill_extentions() {
    const char* ext[] = {"avif", "css", "csv", "docx", "gif", "html", "jpeg",
        "jpg", "js", "json", "mp3", "mp4", "png", "pdf", "php", "txt", "wav",
        "weba", "webm", "webp", "xml", ""};
    const char* mime[] = {"image/avif", "text/css", "text/csv", "application/vnd.openxmlformats-officedocument.wordprocessingml.document",
        "image/gif", "text/html", "image/jpeg", "image/jpeg", "text/javascript",
        "application/json", "audio/mpeg", "video/mp4", "image/png", "application/pdf",
        "application/x-httpd-php", "text/plain", "audio/wav", "audio/webm", "video/webm",
        "image/webp", "application/xml", "application/octet-stream"};
    size_t size = sizeof(ext) / sizeof(ext[0]);
    for(size_t i = 0; i < size; i++)    {
        _extensions[ext[i]] = mime[i];
    }
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
        _status_code = 404;
    }
    size_t pos = _uri.find('?', 0);
    if (pos != std::string::npos)   {
        _query = _uri.substr(pos + 1);
        _uri.erase(pos);
    }
}

void    Response::responde()    {
    if (_status_code == 200)    {
        if (!_found_location && _server_root_path.empty())
            _status_code = 404;
        else if (_found_location)   {
            std::vector<std::string>::iterator it = std::find(_location.methods.begin(), _location.methods.end(), _request_line.method);
            
            if (it == _location.methods.end()) {
                _status_code = 405;
            }
            else if (_location.return_code > 299 && _location.return_code < 308 )    {
                _old_uri = _location.return_path;
                _status_code = 301;
            }   
            else    {
                std::cout << "method = " << _request_line.method << std::endl;
                if (_request_line.method == "GET")
                    get();
                else if (_request_line.method == "DELETE")
                    delete_method();
                else if (_request_line.method == "POST")
                    post();
                else
                    _status_code = 405;
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
    else if (_status_code != 204) {
        set_body();
    }
    return _response_buffer;
}



void    Response::set_body()    {
    if (_status_code != 200 && _status_code != 201)    {
        bzero(_response_buffer, BUFFER_SIZE);
        if (_request_line.method != "HEAD") {
            if (_error_file_good == -1) {
                
                // strcpy(_response_buffer, _error_page.c_str());
                _response_length = _error_page.length();
                for (int i = 0; i < _response_length; i++) {
                    _response_buffer[i] = _error_page[i];
                }
                is_complete = true;
                _file.close();
                is_header = true;
            }
            else    {
                _file.read(_response_buffer, BUFFER_SIZE);
                _response_length = _file.gcount();
                if (_response_length == 0) {
                    is_complete = true;
                    // std::remove(_file_name.c_str());
                    // _file.close();
                    is_header = true;
                }
            }
        }
        else
            _response_length = 0;
    }
    else {
        if (_response_length > 0)    {
            bzero(_response_buffer, BUFFER_SIZE);
            _file.read(_response_buffer, BUFFER_SIZE);
            // std::cout << _response_buffer << std::endl;
            _response_length = _file.gcount();
            _total_response_body_length += _response_length;
            if (_response_length == 0 && !this->_cgi->is_running)  {
                is_complete = true;
                std::stringstream ss;
                ss << _total_response_body_length;
                ss >> _content_length;
                std::remove(_file_name.c_str());
                _file.close();
                // close(_file);
            }
        }
    }
}

void    Response::set_error() {
    std::map<int, std::string> error = _client->get_server().get_error_pages();
    std::map<int, std::string>::iterator it = error.find(_status_code);
    if (it != error.end())  {
        _file.open(it->second.c_str(), std::ios::in);
        if (!_file.good()) {
            _error_file_good = -1;
        }
        else {
            _content_type = "Content-Type: text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            is_header = true;
        }
    }
    if (it == error.end() || _error_file_good == -1)  {
        _error_file_good = -1;
        int pos;
        std::string tmp = "...";
        pos = _error_page.find(tmp, 0);
        _error_page.erase(pos, 3);
        if (_status_code == 400){
            _error_page.insert(pos, "<p>Error 400!<br>Bad Request</p>");
        }
        else if (_status_code == 501){
            _error_page.insert(pos, "<p>Error 501!<br>Not Implemented</p>");
        }
        else if (_status_code == 414){
            _error_page.insert(pos, "<p>Error 414!<br>Request-URI Too Long</p>");
        }
        else if (_status_code == 413){
            _error_page.insert(pos, "<p>Error 413!<br>Request Entity Too Large</p>");
        }
        else if (_status_code == 404) {
            _error_page.insert(pos, "<p>Error 404!<br>Not Found</p>");
        }
        else if (_status_code == 405) {
            _error_page.insert(pos, "<p>Error 405!<br>Method Not Allowed</p>");
        }
        else if (_status_code == 403) {
            _error_page.insert(pos, "<p>Error 403!<br>Forbidden</p>");
        }
        else if (_status_code == 408) {
            _error_page.insert(pos, "<p>Error 408!<br>Request Time-out</p>");
        }
        else if (_status_code == 409) {
            _error_page.insert(pos, "<p>Error 409!<br>Conflict</p>");
        }
        else if (_status_code == 500) {
            _error_page.insert(pos, "<p>Error 500!<br>Internal Server Error</p>");
        }
        else if (_status_code == 504) {
            _error_page.insert(pos, "<p>Error 504!<br>Gateway Timeout</p>");
        }
        if (_request_line.method != "HEAD"){
            _content_type = "text/html";
            _response_header = "Content-Type: " + _content_type + "\r\n";
            std::stringstream ss;
            ss << _error_page.length();
            ss >> _content_length;
            _response_header += "Content-Length: " + _content_length + "\r\n";
        }
        is_header = true;
    }
    _status_line = _error_line[_status_code] + "\r\n";
}

void   Response::set_headers()    {
    if (_status_code == 200)    {
        _status_line = "HTTP/1.1 200 OK\r\n";
		if (!_cgi->is_complete)	{
        	_response_header = "Content-Type: " + _content_type + "\r\n";
		}
        is_header = true;
    }
    else if (_status_code == 201)   {
        _status_line = "HTTP/1.1 201 Created\r\n";
        is_header = true;
    }
    else if (_status_code == 204)   {
        _status_line = "HTTP/1.1 204 No Content\r\n";
        is_header = true;
    }
    else if (_status_code == 301)   {
        _status_line = "HTTP/1.1 301 Moved Permanently\r\n";
        _response_header = "Location: " + _old_uri + "\r\n";
        is_header = true;
    }
    else    {
        set_error();
    }
    // if (_status_code == 200)
    //     _response_header += "Content-Length: " + _content_length + "\r\n";
    _response = _status_line + _response_header;
	if (!_cgi->is_complete)
		_response += "\r\n";
    bzero(_response_buffer, BUFFER_SIZE);
    strcpy(_response_buffer, _response.c_str());
    // std::cout << "[" << _response << "]" << std::endl;
    // is_header = true;
    _response_length = _response.length();
}

void Response::find_files() {
    DIR *dir;
	struct dirent *d;
	struct stat s;
	char buff[18];
    std::map<std::string, unsigned char> files;
	std::string file, name, path;
    unsigned char type;
	file = "<!DOCTYPE html>\n<html>\n<head>\n<title>Index of ";
	file += _old_uri;
	file += "</title>\n</head>\n<body>\n<h1>Index of ";
	file += _old_uri + "</h1>\n<hr>\n<pre>\n<table>\n<tbody>\n";
	dir = opendir(_uri.c_str());
    if (dir)    {
        d = readdir(dir);
        while (d)   {
            name = d->d_name;
            if (name == "." || (name != ".." && name[0] == '.'))    {
                d = readdir(dir);
                continue;
            }
            type = d->d_type;
            files.insert(std::pair<std::string, unsigned char>(name, type));
            d = readdir(dir);
        }
        for(std::map<std::string, unsigned char>::iterator it = files.begin(); it != files.end(); it++) {
            name = it->first;
            type = it->second;
			path = _uri + "/" + name;
			stat(path.c_str(), &s);
			file += "<tr>\n<td><a href=" + name;
			if (type == DT_DIR)
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
            if (type == DT_DIR)
                file += "-";
            else
                file += tmp;
            file += "</td></tr>\n";
        }
        file += "</tbody>\n</table>\n<hr>\n</pre>\n</body>\n</html>";
    }
	closedir(dir);
    std::string tmp = "/nfs/homes/";
    tmp += USER;
    tmp += "/.cache/autoindex.html"; 
    _file.open(tmp.c_str(), std::ios::out | std::ios::trunc);
    if (!_file.good())  {
        _status_code = 404;
        return;
    }
    _file.write(file.c_str(), file.length());
    std::stringstream sg;
    sg << file.length();
    sg >> _content_length;
    _file.close();
    _file.open(tmp.c_str(), std::ios::in);
}

