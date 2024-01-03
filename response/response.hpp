/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharrass <aharrass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 15:29:02 by aharrass          #+#    #+#             */
/*   Updated: 2024/01/03 08:41:52 by aharrass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "method_utils.hpp"
#include "../Server.hpp"
#include "../Request.hpp"
#include <algorithm>
#include <fstream>
#include <sys/stat.h>

#define DIREC 0
#define FILE 1
#define NOT_FOUND -1
class Client;
class Cgi;
class   Response    {
    private:
        unsigned int _status_code;
		int	_del_error;
        std::string _status_line;
        std::string _response_header;
        char _response_buffer[BUFFER_SIZE];
        std::string _uri;
        std::string _old_uri;
        std::string _query;
        std::string _content_type;
        std::string _response;
        std::string _error_page;
        std::string _file_path;
        std::string _file_name;
        location _location;
        bool    _found_location;
        Client  *_client;
        std::fstream _file;
        std::map<std::string, std::string> _extensions;
        std::string _ext;
        std::string						_server_root_path;
        std::string						_server_index_path;
        std::map<int, std::string>		_server_error_pages;
        std::map<std::string, std::string> _headers;
        Request_line    _request_line;
        bool    is_complete;
        bool    is_header;
        int _response_length;
        Cgi *_cgi;

        Response();

    public:
		bool	has_cgi;
        Response(unsigned int status_code, Client &client);
        ~Response();

        void    pars_uri();
        void    match_uri();
        int     get_resource_type();
        void    find_files();
        void    fill_extentions();
        void    get();
        char*   send();
        void    set_body();
		void	delete_method();
        void    post();
		void	clear_dir(const std::string &);
        void    responde();
        void    set_headers();
        void    set_cgi(Cgi *);
        void    set_file_path(const std::string &);
        void    set_status_code(unsigned int);
        void    set_file_name(std::string);
        std::string    get_ext() const;
        
        bool    getIs_complete() const;
        bool    getIs_header() const;
        const std::string&  get_old_uri() const;
        const std::string&  get_uri() const;
        const std::string&  get_method() const;
        Client*             get_client() const;
        const location&     get_location() const;
        const std::string&  get_query() const;
        const int &getResponse_length() const;
        // const std::string &get_uri() const;
};
#include "../Client.hpp"