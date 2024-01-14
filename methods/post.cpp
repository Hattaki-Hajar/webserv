#include "../response/response.hpp"

void    Response::post() {
    if (!_found_location) {
        std::remove(_file_path.c_str());
        _status_code = 404;
        return ;
    }
    if (!_location.upload_path.empty()) {
        std::string path = _location.upload_path + "/" + _file_path.substr(_file_path.find(".cache/") + 7);
        // std::cout << path << std::endl;
        std::cout << "file" << std::endl;
        std::fstream file(path.c_str(), std::ios::out | std::ios::trunc);
        if (file.fail()) {
            std::remove(_file_path.c_str());
            _status_code = 409;
            return ;
        }
        std::rename(_file_path.c_str(), path.c_str());
        _status_code = 201;
        return;
    }
    int type = get_resource_type();
    if (type == NOT_FOUND) {
        std::remove(_file_path.c_str());
        _status_code = 404;
        return;
    }
    if (type == FILE) {
		std::string extension = get_ext();
		std::map<std::string, std::string>::iterator it = _location.cgi.find(extension);
        if (it != _location.cgi.end() && (extension == "php" || extension == "py")) {
			if (extension == "php")
				_cgi->php_setup(_file_path);
			else
				_cgi->py_setup(_file_path);
			if (_status_code == 200)    {
                _file.open(_file_name.c_str(), std::ios::in);
                if (!_file.good())  {
                    std::remove(_file_path.c_str());
                    _status_code = 403;
                    return;
                }
                _content_type = "text/html";
            }
        }
        else {
            std::remove(_file_path.c_str());
            _status_code = 403;
            return;
        }
    } else if (type == DIREC) {
        if (_uri[_uri.length() - 1] != '/') {
            _old_uri += "/";
            std::remove(_file_path.c_str());
            _status_code = 301;
            return;
        }
        if (!_location.index.empty()) {
            std::remove(_file_path.c_str());
            _status_code = 403;
            return;
        }
        std::string extension = get_ext();
		std::map<std::string, std::string>::iterator it = _location.cgi.find(extension);
        if (it != _location.cgi.end() && (extension == "php" || extension == "py")) {
			if (extension == "php")
				_cgi->php_setup(_file_path);
			else
				_cgi->py_setup(_file_path);
			if (_status_code == 200)    {
                _file.open(_file_name.c_str(), std::ios::in);
                if (!_file.good())  {
                    std::remove(_file_path.c_str());
                    _status_code = 403;
                    return;
                }
                _content_type = "text/html";
            }
        }
        else {
            std::remove(_file_path.c_str());
            _status_code = 403;
            return;
        }
    }
}