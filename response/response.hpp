/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharrass <aharrass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 15:29:02 by aharrass          #+#    #+#             */
/*   Updated: 2023/12/31 22:07:03 by aharrass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "method_utils.hpp"
#include "../Server.hpp"
#include "../Request.hpp"
#include <algorithm>
#include <fstream>

#define DIREC 0
#define FILE 1
#define NOT_FOUND -1
class Client;
class   Response    {
    private:
        unsigned int _status_code;
		int	_del_error;
        std::string _status_line;
        std::string _response_header;
        char _response_buffer[BUFFER_SIZE];
        std::string _uri;
        std::string _old_uri;
        std::string _content_type;
        std::string _response;
        std::string _error_page;
        location _location;
        bool    _found_location;
        Client  *_client;
        std::ifstream _file;
        std::string						_server_root_path;
        std::string						_server_index_path;
        std::map<int, std::string>		_server_error_pages;
        std::map<std::string, std::string> _headers;
        Request_line    _request_line;
        bool    is_complete;
        bool    is_header;
        int _response_length;

        Response();

    public:
        Response(unsigned int status_code, Client &client);
        ~Response();

        void    pars_uri();
        void    match_uri();
        int     get_resource_type();
        void    find_files();
        void    get();
        char*   send();
        void    set_body();
		void	delete_method();
		void	clear_dir(const std::string &);
        void   responde();
        void   set_headers();
        
        bool    getIs_complete() const;
        bool    getIs_header() const;
        const int &getResponse_length() const;
        // const std::string &get_uri() const;
};
#include "../Client.hpp"