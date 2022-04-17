#include "../include/Cgi.hpp"

Cgi::Cgi() : _envp(NULL), _argv(NULL), _envp_save(NULL), _cgi_path(std::string()), _target(std::string()), _cgi_launcher(std::string())
{
	//std::cout << "Class Cgi default constructor" << std::endl;
}


Cgi::Cgi(char **envp, std::string cgi_conf) : _envp(NULL), _argv(NULL), _envp_save(NULL), _cgi_path(std::string()), _target(std::string()), _cgi_launcher(std::string())
{
	//std::cout << "Class Cgi default constructor" << std::endl;
	for(int i = 0; envp[i]; i++)
		this->_envp = add_line_doubletab(this->_envp, envp[i]);
	setup(envp, cgi_conf);
}

Cgi::Cgi(Cgi const & copy)
{
	_envp = copy._envp;
	_argv = copy._argv;
	_cgi_launcher = copy._cgi_launcher;
	_target = copy._target;
	_cgi_path = copy._cgi_path;
	_envp_save = copy._envp_save;
	//std::cout << "Class Cgi copy constructor" << std::endl;
}

Cgi::~Cgi()
{
	//std::cout << "Class Cgi destructor" << std::endl;
}

Cgi & Cgi::operator=(Cgi const & copy)
{
	if(this == &copy)
		*this = copy;
	_cgi_launcher = copy._cgi_launcher;
	_argv = copy._argv;
	_envp = copy._envp;
	_target = copy._target;
	_cgi_path = copy._cgi_path;
	return(*this);
}
/*
_content_length
PATH_INFO
REQUEST_METHOD
SCRIPT_FILENAME
SERVER_PROTOCOL
REDIRECT_STATUS
CONTENT_TYPE
*/ 

void Cgi::build_arg_and_envp(std::string uri, char **request) //POST
{
	(void)uri;
	std::cout << "sup" << std::endl;
	for(int i = 0; _envp[i]; i++)
		std::cout << "sup" <<_envp[i] << std::endl;
	std::string arg_string;
	_argv = (char**)malloc(sizeof(char**) * 3);
	_argv[0] = strdup(_cgi_launcher.c_str());
	_argv[1] = strdup(_cgi_path.c_str());
	_argv[2] = NULL;

	// BUILD ENVP //

	int count = 0;
	while(_envp[count])
		count++;
	count += 8;
	char buf[500];
	std::string current_path = getcwd(buf, 500);
	if(_cgi_path.front() == '.')
		_cgi_path.erase(0,1);
	_envp = add_line_doubletab(_envp, (_content_length + "=" + std::to_string(arg_string.size())).c_str());
	_envp = add_line_doubletab(_envp, (_content_type + "=application/x-www-form-urlencoded").c_str());
	_envp = add_line_doubletab(_envp, (_path_info + "=" + current_path + _cgi_path).c_str());
	_envp = add_line_doubletab(_envp, (_server_protocol + "=HTTP/1.1 ").c_str());
	_envp = add_line_doubletab(_envp, (_request_method + "=POST").c_str());
	_envp = add_line_doubletab(_envp, (_script_filename + "=" + current_path + _cgi_path).c_str());
	_envp = add_line_doubletab(_envp, (_redirect_status + "=200").c_str());
	_envp = add_line_doubletab(_envp, (_gateway_interface + "=CGI/1.1").c_str());
	(void)request;
}
/*
PATH_INFO
REQUEST_METHOD
SCRIPT_FILENAME
SERVER_PROTOCOL
REDIRECT_STATUS
QUERY_STRING
CONTENT_TYPE
*/ 
void Cgi::build_arg_and_envp(std::string uri) //GET
{
	// BUILD ARG //
	int length = uri.find_first_of('?');
	_cgi_path = uri;
	_cgi_path.resize(length);
	std::string arg_string = &uri[length + 1];
	_argv = (char**)malloc(sizeof(char**) * 3);
	_argv[0] = strdup(_cgi_launcher.c_str());
	_argv[1] = strdup(_cgi_path.c_str());
	_argv[2] = NULL;

	// BUILD ENVP //

	int count = 0;
	while(_envp[count])
		count++;
	count += 8;
	char buf[500];
	std::string current_path = getcwd(buf, 500);
	if(_cgi_path.front() == '.')
		_cgi_path.erase(0,1);
	_envp = add_line_doubletab(_envp, (_content_length + "=" + std::to_string(arg_string.size())).c_str());
	_envp = add_line_doubletab(_envp, (_content_type + "=application/x-www-form-urlencoded").c_str());
	_envp = add_line_doubletab(_envp, (_path_info + "=" + current_path + _cgi_path).c_str());
	_envp = add_line_doubletab(_envp, (_query_string + "=" + arg_string).c_str());
	_envp = add_line_doubletab(_envp, (_server_protocol + "=HTTP/1.1 ").c_str());
	_envp = add_line_doubletab(_envp, (_request_method + "=GET").c_str());
	_envp = add_line_doubletab(_envp, (_script_filename + "=" + current_path + _cgi_path).c_str());
	_envp = add_line_doubletab(_envp, (_redirect_status + "=200").c_str());
	_envp = add_line_doubletab(_envp, (_gateway_interface + "=CGI/1.1").c_str());
}

void Cgi::execute_cgi(std::string uri) //GET
{
	std::cout << "Execute cgi for GET: " << uri << std::endl;
	build_arg_and_envp(uri);
	_execute_cgi();
}

void Cgi::execute_cgi(char **request, std::string uri) //POST
{
	std::cout << "Execute cgi for POST: " << uri << std::endl;
	build_arg_and_envp(uri, request);
	_execute_cgi();
}

void Cgi::free_cgi()
{
	if(_envp)
	{
		for(int i = 0; _envp[i]; i++)
			free(_envp[i]);
		free(_envp);
	}
	_envp = NULL;
	if(_argv)
	{
		for(int i = 0; _argv[i]; i++)
			free(_argv[i]);
		free(_argv);
	}
	_argv = NULL;
}

void Cgi::reset_envp()
{
	free_cgi();
	if(_envp_save)
		for(int i = 0; _envp_save[i]; i++)
			this->_envp = add_line_doubletab(this->_envp, _envp_save[i]);
}

void Cgi::_execute_cgi()
{
	int pid;

    int wait_pid;
	std::cout << "launcher = " << _cgi_launcher << std::endl;

	std::cout << "_argv = " << std::endl;
	if(_argv)
		for(int i = 0; _argv[i]; i++)
			std::cout << _argv[i] << std::endl;
	std::cout << std::endl << "_envp = " << std::endl;
	if(_envp)
		for(int i = 0; _envp[i]; i++)
			std::cout << _envp[i] << std::endl << std::endl;
	std::cout << "START CGI EXECUTION" << std::endl << std::endl;
    pid = fork();
    if (pid == 0)
	{
        if (execve(_cgi_launcher.c_str(), _argv, _envp) < 0)
	    {
            std::cerr << "Error, execve" << std::endl;
            exit(1);
        }
    }
	else if (pid == -1)
		std::cerr << "Error, pid = -1" << std::endl;
	else
		waitpid(pid, &wait_pid, 0);
	reset_envp();
	std::cout << std::endl << std::endl << "END CGI EXECUTION" << std::endl;
}

void Cgi::setup(char **envp, std::string cgi_conf)
{
	_envp_save = envp;
	std::cout << "IN CGI CLASS : PARSE " << cgi_conf << std::endl;
	for(int i = 0; envp[i]; i++)
		this->_envp = add_line_doubletab(this->_envp, envp[i]);
	char **tmp = ft_split(cgi_conf.c_str(), " ");
	_target = tmp[0];
	_cgi_launcher = tmp[1];
	int i = 0; 
	while(_cgi_launcher[i] != ';')
		i++;
	_cgi_launcher.resize(i);
	for(int i = 0; tmp[i]; i++)
		free(tmp[i]);
	free(tmp);
}

void Cgi::set_cgi_launcher(std::string value)
{
	_cgi_launcher = value;
}

void Cgi::set_target(std::string value)
{
	_target = value;
}

std::string Cgi::get_target()
{
	return (_target);
}

std::string Cgi::get_cgi_launcher()
{
	return (_cgi_launcher);
}
