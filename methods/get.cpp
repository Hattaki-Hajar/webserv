/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharrass <aharrass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 21:59:20 by aharrass          #+#    #+#             */
/*   Updated: 2024/01/01 21:47:31 by aharrass         ###   ########.fr       */
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
                _file.open(_uri.c_str(), std::ios::in);
                if (!_file.good())  {
                    _status_code = 404;
                    return;
                }
                _content_type = _extensions[get_ext()];
                return ;
            }
            else  {
                if (_location.autoindex)   {
                    find_files();
                    _content_type = "text/html";
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
        //needs cgi
        _file.open(_uri.c_str(), std::ios::in | std::ios::out);
        
        if (!_file.good())  {
            _status_code = 404;
            return;
        }
        _content_type = _extensions[get_ext()];
        //need content-type
        return ;
    }
}  