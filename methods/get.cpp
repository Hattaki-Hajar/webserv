/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharrass <aharrass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 21:59:20 by aharrass          #+#    #+#             */
/*   Updated: 2023/12/31 22:08:15 by aharrass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../response/response.hpp"

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
                    _uri += _location.index;
                else if (!_server_index_path.empty())
                    _uri += _server_index_path;
                std::cout << _uri << std::endl;
                //needs cgi
                _file.open(_uri.c_str());
                if (!_file.good())  {
                    _status_code = 404;
                    return;
                }
                // _file = open(_uri.c_str(), O_RDONLY);
                // if (_file < 0)    {
                //     _status_code = 404;
                //     return;
                // }
                _content_type = "text/html";
                return ;
            }
            else  {
                if (_location.autoindex)   {
                    // find_files();
                    return ;
                }
                else    {
                    _status_code = 403;
                    return ;
                }
            }
        }
    }
    else if (type == FILE)  {
        _file.open(_uri.c_str());
        
        if (!_file.good())  {
            _status_code = 404;
            return;
        }
        // _file = open(_uri.c_str(), O_RDONLY);
        _content_type = "video/webm";
        //need content-type
        return ;
    }
}  