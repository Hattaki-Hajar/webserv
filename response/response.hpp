/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharrass <aharrass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 15:29:02 by aharrass          #+#    #+#             */
/*   Updated: 2023/12/24 21:30:12 by aharrass         ###   ########.fr       */
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
        std::string _status_line;
        std::string _response_header;
        std::string _response_body;
        std::string _uri;
        const location *_location;
        Client  *_client;
        std::map<std::string, std::string> _headers;
        Request_line    _request_line;

        Response();

    public:
        Response(int status_code, Client &client);
        ~Response();

        void    pars_uri();
        void match_uri();
        int get_resource_type();
        void get();
        // void setStatusLine();

        // const std::string &get_uri() const;
};