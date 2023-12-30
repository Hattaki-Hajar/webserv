/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharrass <aharrass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 15:29:02 by aharrass          #+#    #+#             */
/*   Updated: 2023/12/30 10:54:29 by aharrass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "method_utils.hpp"
#include "../Server.hpp"
#include "../Client.hpp"

#define DIREC 0
#define FILE 1
#define NOT_FOUND -1

class   Response    {
    private:
        int _status_code;
		int	_del_error;
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
        std::string						_root_path;
        std::string						_index_path;
        std::map<int, std::string>		_error_pages;
        std::map<std::string, std::string> _headers;
        Request_line    _request_line;

        Response();

    public:
        Response(int status_code, Client &client);
        ~Response();

        void    pars_uri();
        void match_uri();
        int get_resource_type();
        // void get();
		void	delete_method();
		void	clear_dir(const std::string &);
        void    responde();
        void setResponse();
        

        // const std::string &get_uri() const;
};