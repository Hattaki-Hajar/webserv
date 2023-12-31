/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharrass <aharrass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 15:35:45 by aharrass          #+#    #+#             */
/*   Updated: 2023/12/30 14:25:01 by aharrass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "method_utils.hpp"

bool check_location(const std::string &path)   {
    std::fstream location(path.c_str(), std::ios::in);
	
    if (location.fail()){
        return false;
    }
    location.close();
    return true;
    
}



