/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharrass <aharrass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 15:34:16 by aharrass          #+#    #+#             */
/*   Updated: 2023/12/24 21:45:38 by aharrass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

Response::Response()    {
}

Response::Response(int status_code, Client &client)
: _status_code(status_code)   {
    _location = NULL;
    _client = &client;
    _headers = _client->get_request()->get_headers();
    _uri = _client->get_request()->get_request_line().uri;
    pars_uri();
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

// void Response::get()  {
//     if (_status_code != 200)
//         return;
    
//     int type = get_resource_type();
    
//     if (type == NOT_FOUND)
//     {
//         _status_code = 404;
//         return;
//     }
//     if (type == DIREC)  {
//         if (_uri[_uri.length() - 1] != '/') {
//             _response_header += "location: ";
//             _response_header += _uri + "/";
//             _status_code = 301;
//             return;
//         }
//         if ()
        
//     }
    
// }

void Response::match_uri()  {
    std::map<std::string, location> locations = _client->get_server().get_locations();
    int diff = 0;
    int diff_tmp = 0;
    int comp = 0;

    if (_status_code != 200)
        return ;
    std::map<std::string, location>::const_iterator it_tmp = locations.end();
    for(std::map<std::string, location>::const_iterator it = locations.begin(); it != locations.end(); it++)    {
        if (it->first.length() >= _uri.length())    {
            comp = _uri.compare(it->first.substr(0, _uri.length()));
            std::cout << "a: " << it->first.substr(0, _uri.length()) << std::endl;
            diff = it->first.length() - _uri.length();
            if (it != locations.begin() && comp == 0)    {
                if (diff < diff_tmp)   {
                    it_tmp = it;
                    diff_tmp = diff;
                }
            }
            else
                diff_tmp = diff;
        }
        else  if (it->first.length() < _uri.length())    {
            comp = it->first.compare(_uri.substr(0, it->first.length()));
            std::cout << "b: " << _uri.substr(0, it->first.length()) << std::endl;
            diff = _uri.length() - it->first.length();
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
    if (it_tmp != locations.end())
        _location = &it_tmp->second;
    if (_uri.length() >= it_tmp->first.length()) {
        std::string tt = _uri.substr(0, it_tmp->first.length());
        _uri.erase(0, it_tmp->first.length());
        _uri = _location->root + tt + _uri;
        
    }
    std::cout << _uri << std::endl;
}

// const std::string &Response::get_uri() const {
//     return _uri;
// }