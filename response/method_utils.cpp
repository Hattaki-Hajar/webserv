/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharrass <aharrass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 15:35:45 by aharrass          #+#    #+#             */
/*   Updated: 2024/01/09 01:49:20 by aharrass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "method_utils.hpp"

bool check_location(const std::string &path)   {
    
    if (access(path.c_str(), F_OK) == 0)
        return true;
    else
        return false;
    // std::fstream location(path.c_str(), std::ios::in);
	
    // if (location.fail()){
    //     return false;
    // }
    // location.close();
    // return true;
    
}



