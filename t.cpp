/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t.cpp                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharrass <aharrass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 00:00:51 by aharrass          #+#    #+#             */
/*   Updated: 2023/12/28 05:35:32 by aharrass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <sys/types.h>
// #include <dirent.h>
// #include <sys/stat.h>
// #include <iostream>
// #include <time.h>

// int main()  {
//     std::string _uri = "/nfs/homes/aharrass/webserv";

//     DIR *dir = opendir(_uri.c_str());

//     struct dirent *d = readdir(dir);
//     struct stat s;
//     while (d)   {
//         std::string path = _uri + "/" + d->d_name;
//         if (stat(path.c_str(), &s) != 0) {
//             std::cout << "fail" << std::endl;
//             return -1;
//         }
//         char buff[18];
//         strftime(buff, sizeof(buff), "%d-%b-%Y %H:%M", gmtime(&s.st_mtim.tv_sec));
//         std::cout << d->d_name << "     ";
//         std::cout.flush(); 
//         std::cout << buff << std::endl;
//         d = readdir(dir);
//     }    
        
// }