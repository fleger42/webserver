#ifndef CGI_HPP
#define CGI_HPP
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
const std::string _content_length = "CONTENT_LENGTH";
const std::string _content_type = "CONTENT_TYPE";
const std::string _path_info= "PATH_INFO";
const std::string _query_string = "QUERY_STRING";
const std::string _server_protocol = "SERVER_PROTOCOL";
const std::string _request_method = "REQUEST_METHOD";
const std::string _script_filename = "SCRIPT_FILENAME";
const std::string _redirect_status= "REDIRECT_STATUS";
const std::string _gateway_interface= "GATEWAY_INTERFACE";
char** add_line_doubletab(char **dtab, const char * newline);
char				**ft_split(char const *s, const char *delimiters);
class Cgi
{
	public:
		Cgi();
		Cgi(char **envp, std::string cgi_conf);
		Cgi(Cgi const & copy);
		~Cgi();
		Cgi & operator=(Cgi const & copy);
		void set_cgi_launcher(std::string value);
		void set_target(std::string value);
		void free_cgi();
		std::string get_target();
		std::string get_cgi_launcher();
		void execute_cgi(char **request, std::string uri);
		void execute_cgi(std::string uri);
		void setup(char **envp, std::string cgi_conf);
		void reset_envp();
	private:
		void build_arg_and_envp(std::string uri);
		void build_arg_and_envp(std::string uri, char **request);
		char **_envp;
		char **_argv;
		char **_envp_save;
		void _execute_cgi();
		std::string _cgi_path;
		std::string _target;
		std::string _cgi_launcher;
};
#endif