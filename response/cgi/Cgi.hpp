#pragma once
#include "../../Server.hpp"
#include "sys/wait.h"
#include <time.h>

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
		// std::string                         _resource;
		std::map<std::string, std::string>  _headers;
		int									_outfile;
		Response							*_response;
		int									_fd;
		Cgi();
	public:
		char								**_env;
		clock_t								_start;
		pid_t                               _pid;
		bool                                is_complete;
		bool                                is_timeout;
		bool								is_running;
		Cgi(std::map<std::string, std::string>, Response *);
		~Cgi();
		void	py_setup(const std::string &);
		void	php_setup(const std::string &);
		void	run(const std::string &);
		void	setup_env(std::map<std::string, std::string>);
		int		get_outfile() const;
};
#include "../response.hpp"