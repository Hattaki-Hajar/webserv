/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharrass <aharrass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 21:59:20 by aharrass          #+#    #+#             */
/*   Updated: 2024/01/07 17:20:29 by aharrass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../response/response.hpp"

void Response::get()  {
    int type = get_resource_type();
    if (type == NOT_FOUND)   {
        _status_code = 404;
        // std::cout << "huh" << std::endl;
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
                std::string extension = get_ext();
                if (extension == "php" || extension == "py"){
                    try { 
                        if (extension == "php")
                            _cgi->php_setup("");
                        else
                            _cgi->py_setup("");
                    }
                    catch (std::exception &e)   {
                        std::cout << e.what() << std::endl;
                    }
                }
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
                    std::cout << "here" << std::endl;
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
		std::cout << "debug: in get file" << std::endl;
        std::string extension = get_ext();
		std::map<std::string, std::string>::iterator it = _location.cgi.find(extension);
        if ((extension == "php" && it != _location.cgi.end()) || (extension == "py" && it != _location.cgi.end())){
            int f = open(_uri.c_str(), O_RDONLY);
            if (f == -1)    {
                _status_code = 403;
                return;
            }
            try
            {
                if (extension == "php")
                    _cgi->php_setup("");
                else
                    _cgi->py_setup("");
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
            if (_status_code == 200)    {
                _file.open(_file_name.c_str(), std::ios::in);
                if (!_file.good())  {
                    std::cout << "fail" << std::endl;
                    _status_code = 403;
                    return;
                }
                _content_type = "text/html";
            }
        }
        else    {
            _file.open(_uri.c_str(), std::ios::in | std::ios::out);
            
            if (!_file.good())  {
                _status_code = 403;
                return;
            }
            _content_type = _extensions[get_ext()];
            
        }
        return ;
    }
}  