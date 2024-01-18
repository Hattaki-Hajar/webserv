/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharrass <aharrass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 15:35:45 by aharrass          #+#    #+#             */
/*   Updated: 2024/01/18 21:20:05 by aharrass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "method_utils.hpp"

bool check_location(std::string &path)   {
    
    if (access(path.c_str(), F_OK) == 0)
        return true;
    else {
        size_t pos = path.find("%20");
        while (pos != std::string::npos)   {
            std::string tmp = path.substr(0, pos);
            tmp += " ";
            tmp += path.substr(pos + 3);
            if (access(tmp.c_str(), F_OK) == 0) {
                path = tmp;
                return true;
            }
            pos = path.find("%20", pos + 1);
        }
        return false;
    }
}



