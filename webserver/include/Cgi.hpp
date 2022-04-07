#ifndef CGI_HPP
#define CGI_HPP
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
const std::string _content_length = "CONTENT_LENGTH";
const std::string _content_type = "CONTENT_TYPE";
const std::string _path_info= "PATH_INFO";
const std::string _query_string = "QUERY_STRING";
const std::string _server_protocol = "SERVER_PROTOCOL";
const std::string _request_method = "REQUEST_METHOD";
const std::string _script_filename = "SCRIPT_FILENAME";
const std::string _redirect_status= "REDIRECT_STATUS";

class Cgi
{
	public:
		Cgi(char **envp);
		Cgi(Cgi const & copy);
		~Cgi();
		Cgi & operator=(Cgi const & copy);
		void execute_cgi();
	private:
		Cgi();
		std::string _cgi_path;
		int _wait_pid;
		char **_arg;
		char **_envp;
};
#endif