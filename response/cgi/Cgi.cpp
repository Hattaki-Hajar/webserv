#include "Cgi.hpp"

Cgi::Cgi(std::map<std::string, std::string> config_info, std::string resource)   {
    this->_extension_map = config_info;
    this->_resource = resource;
}
