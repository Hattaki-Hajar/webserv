/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharrass <aharrass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 15:35:45 by aharrass          #+#    #+#             */
/*   Updated: 2023/12/17 22:14:12 by aharrass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "method_utils.hpp"

bool check_location(const std::string &path)   {
    std::fstream location(path.c_str(), std::ios::in);
    if (location.fail()){
        return false;
    }
    return true;
    
}

int get_resource_type(const std::string &path)    {
    if (!check_location(path))
        return NOT_FOUND;
    DIR *dir = opendir(path.c_str());
    if (dir)    {
        closedir(dir);
        return DIREC;
    }
    std::fstream file(path.c_str(), std::ios::in);
    if (!file.fail()){
        file.close();   
        return FILE;
    }
    return NOT_FOUND;
}


