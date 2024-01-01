#pragma once
#include "../../Server.hpp"
class Cgi
{
private:
    std::map<std::string, std::string>  _extension_map;
    std::string                         _resource;
    pid_t                               _pid;
    int                                 _fd[2];                        

    Cgi();
public:
    bool                                is_complete;
    bool                                is_timeout;
    Cgi(std::map<std::string, std::string> config_info, std::string resource);
    ~Cgi();
    void    run();
};