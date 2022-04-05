#ifndef CGI_HPP
#define CGI_HPP
#include <iostream>
#include <unistd.h>

class Cgi
{
	public:
		Cgi();
		Cgi(std::string filename);
		Cgi(Cgi const & copy);
		~Cgi();
		Cgi & operator=(Cgi const & copy);
	private:
        const std::string _auth_type;
        const std::string _content_length;
        const std::string _content_type;
        const std::string _gateway_interface;
        const std::string _path_info;
        const std::string _path_translated;
        const std::string _query_string;
        const std::string _remote_addr;
        const std::string _remote_ident;
        const std::string _remote_user;
        const std::string _request_method;
        const std::string _request_uri;
        const std::string _script_filename;
        const std::string _server_name;
        const std::string _server_port;
        const std::string _server_protocol;
        const std::string _server_software;
        const std::string _redirect_status;
};
#endif