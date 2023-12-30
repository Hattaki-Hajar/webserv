/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharrass <aharrass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 15:29:02 by aharrass          #+#    #+#             */
/*   Updated: 2023/12/30 16:21:10 by aharrass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "method_utils.hpp"
#include "../Server.hpp"
#include "../Client.hpp"
#include <algorithm>
#include <fstream>

#define DIREC 0
#define FILE 1
#define NOT_FOUND -1

class   Response    {
    private:
        int _status_code;
        std::string _status_line;
        std::string _response_header;
        std::string _response_body;
        std::string _uri;
        std::string _old_uri;
        std::string _content_type;
        std::string _response;
        std::string _error_page;
        location _location;
        bool    _found_location;
        Client  *_client;
        std::string						_server_root_path;
        std::string						_server_index_path;
        std::map<int, std::string>		_server_error_pages;
        std::map<std::string, std::string> _headers;
        Request_line    _request_line;

        Response();

    public:
        Response(int status_code, Client &client);
        ~Response();

        void    pars_uri();
        void    match_uri();
        int     get_resource_type();
        void    find_files();
        void    get();
        void    responde();
        void    setResponse();
        

        // const std::string &get_uri() const;
};