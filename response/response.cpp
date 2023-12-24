/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharrass <aharrass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 15:34:16 by aharrass          #+#    #+#             */
/*   Updated: 2023/12/24 17:53:05 by aharrass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

Response::Response()    {
}

Response::Response(int status_code, Server serv, Client client)
: _status_code(status_code) , _serv(serv)   {
    _location = NULL;
    _client = &client;
    _headers = _client->get_request()->get_headers();
    _uri = _client->get_request()->get_request_line().uri;
    pars_uri();
}

Response::~Response()   {
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
//     match_uri();
//     if (_status_code != 200)
//         return;
    
//     int type = get_resource_type(_uri);
    
//     if (type == NOT_FOUND)
//     {
//         _status_code = 404;
//         return;
//     }
//     if (type == DIREC)  {
        
//     }
    
// }

void Response::match_uri()  {
    std::map<std::string, location> locations = _serv.get_location();

    if (_status_code != 200)
        return ;
    std::map<std::string, location>::const_iterator it = locations.find(_uri);

    if (it == locations.end() && _uri[_uri.length() - 1] != '/')  {
        it = locations.find(_uri + "/");
    }
    else if (it == locations.end() && _uri[_uri.length() - 1] == '/')   {
        _uri.erase(_uri.end() - 1);
        it = locations.find(_uri);
    }
    if (it != locations.end())  {
        _location = &it->second;
        if (_location->root[0])
            _uri = _location->root;
    }
    std::cout << _uri << std::endl;
}

const std::string &Response::get_uri() const {
    return _uri;
}