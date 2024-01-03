#pragma once
#include "../../Server.hpp"
#include "sys/wait.h"

// typedef enum extension {
// 	PHP, // 0
// 	PYTHON, // 1
// 	INVALID = -1,
// } extension;
class Response;
class Cgi
{
	private:
		std::map<std::string, std::string>  _extension_map;
		std::map<std::string, std::string>  _headers;
		// std::string                         _resource;
		pid_t                               _pid;
		int									_outfile;
		char								**_env;
		Response							*_response;
		Cgi();
	public:
		bool                                is_complete;
		bool                                is_timeout;
		Cgi(std::map<std::string, std::string>, Response *);
		~Cgi();
		void	py_setup();
		void	php_setup();
		void	run(const std::string &);
		void	setup_env(std::map<std::string, std::string>);
		int		get_outfile() const;
};
#include "../response.hpp"