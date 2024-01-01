#include "../response/response.hpp"

void    Response::post() {
    if (!_found_location) {
        _status_code = 404;
        return ;
    }
    if (!_location.upload_path.empty()) {
        std::string path = _location.upload_path + "/" + _file_path.substr(_file_path.find("/") + 1);
        std::rename(_file_path.c_str(), path.c_str());
        _status_code = 201;
        return;
    }
    int type = get_resource_type();
    if (type == NOT_FOUND) {
        _status_code = 404;
        return;
    }
    if (type == FILE) {
        if (!_location.Cgi.path.empty()) {
            // run cgi on requested file with POST.
            // return _status_code of the cgi;
        }
        else {
            _status_code = 403;
            return;
        }
    } else if (type == DIREC) {
        if (_uri[_uri.length() - 1] != '/') {
            _old_uri += "/";
            _status_code = 301;
            return;
        }
        if (!_location.index.empty()) {
            _status_code = 403;
            return;
        }
        if (!_location.Cgi.path.empty()) {
            // run cgi on requested file with POST.
            // return _status_code of the cgi;
        }
        else {
            _status_code = 403;
            return;
        }
    }
}