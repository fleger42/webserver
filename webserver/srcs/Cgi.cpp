#include "../include/Cgi.hpp"

Cgi::Cgi() : _envp(NULL), _argv(NULL), _envp_save(NULL), _cgi_path(std::string()), _target(std::string()), _cgi_launcher(std::string())
{
	//std::cout << "Class Cgi default constructor" << std::endl;
}


Cgi::Cgi(char **envp, std::string cgi_conf) : _envp(NULL), _argv(NULL), _envp_save(NULL), _cgi_path(std::string()), _target(std::string()), _cgi_launcher(std::string())
{
	//std::cout << "Class Cgi param constructor" << std::endl;
	setup(envp, cgi_conf);
}

Cgi::Cgi(Cgi const & copy) : _envp(NULL), _argv(NULL), _envp_save(NULL), _cgi_path(std::string()), _target(std::string()), _cgi_launcher(std::string())
{
	_cgi_launcher = copy._cgi_launcher;
	_target = copy._target;
	_cgi_path = copy._cgi_path;
	_envp_save = copy._envp_save;
	if(copy._envp)
		for(int i = 0; copy._envp[i]; i++)
			this->_envp = add_line_doubletab(this->_envp, copy._envp[i]);
	if(copy._argv)
		for(int i = 0; copy._argv[i]; i++)
			this->_argv = add_line_doubletab(this->_argv, copy._argv[i]);
	//std::cout << "Class Cgi copy constructor" << std::endl;
}

Cgi::~Cgi()
{
	//std::cout << "Class Cgi destructor" << std::endl;
	free_cgi();
}

Cgi & Cgi::operator=(Cgi const & copy)
{
	//std::cout << "CGI OPERATOR=" << std::endl;
	if(this != &copy)
	{
		_cgi_launcher = copy._cgi_launcher;
		_argv = copy._argv;
		_envp = copy._envp;
		_target = copy._target;
		_cgi_path = copy._cgi_path;
	}
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
	int i = 0;
	while(request[i])
		i++;
	_arg_string = request[i - 1];
	std::string::iterator it = _arg_string.end();
	while(it != _arg_string.begin() && *it != '\n')
		it--;
	it++;
	_arg_string.assign(it, _arg_string.end());
	_cgi_path = uri;
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
	std::stringstream ss;
	ss << _arg_string.size();
	std::string lenght = ss.str();
	if(_cgi_path[0] == '.')
		_cgi_path.erase(0,1);
	_envp = add_line_doubletab(_envp, (_content_length + "=" + lenght).c_str());
	_envp = add_line_doubletab(_envp, (_content_type + "=application/x-www-form-urlencoded").c_str());
	_envp = add_line_doubletab(_envp, (_path_info + "=" + current_path + _cgi_path).c_str());
	_envp = add_line_doubletab(_envp, (_server_protocol + "=HTTP/1.1 ").c_str());
	_envp = add_line_doubletab(_envp, (_request_method + "=POST").c_str());
	_envp = add_line_doubletab(_envp, (_script_filename + "=" + current_path + _cgi_path).c_str());
	_envp = add_line_doubletab(_envp, (_redirect_status + "=200").c_str());
	_envp = add_line_doubletab(_envp, (_gateway_interface + "=CGI/1.1").c_str());
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
	size_t length = uri.find_first_of('?');
	std::string arg_string;
	if(length != std::string::npos)
	{
		_cgi_path = uri;
		_cgi_path.resize(length);
		arg_string = &uri[length + 1];
		_argv = (char**)malloc(sizeof(char**) * 3);
		_argv[0] = strdup(_cgi_launcher.c_str());
		_argv[1] = strdup(_cgi_path.c_str());
		_argv[2] = NULL;
	}
	else
	{
		_argv = (char**)malloc(sizeof(char**) * 3);
		_argv[0] = strdup(_cgi_launcher.c_str());
		_argv[1] = strdup(uri.c_str());
		_argv[2] = NULL;
	}
	// BUILD ENVP //
	int count = 0;
	while(_envp[count])
		count++;
	count += 8;
	char buf[500];
	std::string current_path = getcwd(buf, 500);
	if(_cgi_path[0] == '.')
		_cgi_path.erase(0,1);
	_envp = add_line_doubletab(_envp, (_content_type + "=application/x-www-form-urlencoded").c_str());
	_envp = add_line_doubletab(_envp, (_path_info + "=" + current_path + _cgi_path).c_str());
	_envp = add_line_doubletab(_envp, (_query_string + "=" + arg_string).c_str());
	_envp = add_line_doubletab(_envp, (_server_protocol + "=HTTP/1.1 ").c_str());
	_envp = add_line_doubletab(_envp, (_request_method + "=GET").c_str());
	_envp = add_line_doubletab(_envp, (_script_filename + "=" + current_path + _cgi_path).c_str());
	_envp = add_line_doubletab(_envp, (_redirect_status + "=200").c_str());
	_envp = add_line_doubletab(_envp, (_gateway_interface + "=CGI/1.1").c_str());
}

std::string Cgi::execute_cgi(std::string uri) //GET
{
	std::cout << "Execute cgi for GET: " << uri << std::endl;
	build_arg_and_envp(uri);
	return(_execute_cgi_get());
}

std::string Cgi::execute_cgi(char **request, std::string uri) //POST
{
	std::cout << "Execute cgi for POST: " << uri << std::endl;
	build_arg_and_envp(uri, request);
	return(_execute_cgi_post());
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

std::string Cgi::_execute_cgi_post()
{
	int pid;
	int fd[2];
	char buffer_read[10000];
    int wait_pid;
	memset(buffer_read, 0, 10000);
	/*std::cout << "launcher = " << _cgi_launcher << std::endl;

	std::cout << "_argv = " << std::endl;
	if(_argv)
		for(int i = 0; _argv[i]; i++)
			std::cout << _argv[i] << std::endl;
	std::cout << std::endl << "_envp = " << std::endl;
	if(_envp)
		for(int i = 0; _envp[i]; i++)
			std::cout << _envp[i] << std::endl << std::endl;*/
	std::cout << "START CGI POST EXECUTION" << std::endl << std::endl;
	if (pipe(fd) == -1)
		std::cerr << "Error, pipe" << std::endl;
    if ((pid = fork()) == -1)
		std::cerr << "Error, fork" << std::endl;
    if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
		int s_fd[2];
		int s_pid;
		if (pipe(s_fd) == -1)
			std::cerr << "Error, pipe" << std::endl;
   		if ((s_pid = fork()) == -1)
			std::cerr << "Error, fork" << std::endl;
		if(s_pid == 0)
		{
			close(s_fd[0]);
			dup2(s_fd[1], 1);
			char ** exec;
			exec = (char**)malloc(sizeof(char*) * 3);
			exec[0] = strdup("echo");
			exec[1] = strdup(_arg_string.c_str());
			exec[2] = NULL;
			if (execve("/bin/echo", exec, _envp) < 0)
			{
				std::cerr << "Error, execve" << std::endl;
				exit(1);
			}
		}
		else
		{
			close(s_fd[1]);
			dup2(s_fd[0], 0);
			waitpid(s_pid, &wait_pid, 0);
			if (execve(_cgi_launcher.c_str(), _argv, _envp) < 0)
			{
				std::cerr << "Error, execve" << std::endl;
				exit(1);
			}
		}
    }
	else
	{
		close(fd[1]);
		waitpid(pid, &wait_pid, 0);
		read(fd[0], buffer_read, 10000);
		close(fd[0]);
	}
	reset_envp();
	std::cout << std::endl << std::endl << "END CGI POST EXECUTION" << std::endl;
	//std::cout << "buffer_read [" << buffer_read << "]" << std::endl;
	return (buffer_read);
}

std::string Cgi::_execute_cgi_get()
{
	int pid;
	int fd[2];
	char buffer_read[10000];
	memset(buffer_read, 0, 10000);
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
	std::cout << "START CGI GET EXECUTION" << std::endl << std::endl;
	if (pipe(fd) == -1)
		std::cerr << "Error, pipe" << std::endl;
    if ((pid = fork()) == -1)
		std::cerr << "Error, fork" << std::endl;
    if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
        if (execve(_cgi_launcher.c_str(), _argv, _envp) < 0)
	    {
            std::cerr << "Error, execve" << std::endl;
            exit(1);
        }
    }
	else
	{
		close(fd[1]);
		waitpid(pid, &wait_pid, 0);
		read(fd[0], buffer_read, 10000);
		close(fd[0]);
	}
	reset_envp();
	//std::cout << std::endl << std::endl << "END CGI GET EXECUTION" << std::endl;
	//std::cout << "buffer_read [" << buffer_read << "]" << std::endl;
	return (buffer_read);
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
	std::cout << "IN SETUP :" << _cgi_launcher << std::endl;
	free_double_tab(tmp);
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
