#include "../include/Cgi.hpp"

Cgi::Cgi() : _envp(NULL), _argv(NULL), _cgi_path(std::string()), _target(std::string()), _cgi_launcher(std::string())
{
	//std::cout << "Class Cgi default constructor" << std::endl;
}


Cgi::Cgi(char **envp, std::string cgi_conf) : _envp(envp) ,_argv(NULL), _cgi_path(std::string()), _target(std::string()), _cgi_launcher(std::string())
{
	//std::cout << "Class Cgi default constructor" << std::endl;
	setup(envp, cgi_conf);
}

Cgi::Cgi(Cgi const & copy)
{
	_envp = copy._envp;
	_argv = copy._argv;
	_cgi_launcher = copy._cgi_launcher;
	_target = copy._target;
	_cgi_path = copy._cgi_path;
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
	_envp = copy._envp;
	_cgi_launcher = copy._cgi_launcher;
	_target = copy._target;
	_cgi_path = copy._cgi_path;
	return(*this);
	//std::cout << "Class Cgi operator=" << std::endl;
}


void Cgi::build_arg_and_envp(std::string uri, char **request) //POST
{
	int nbr_tab = 0;
	while(request[nbr_tab])
		nbr_tab++;
	(void)uri;
}
void Cgi::build_arg_and_envp(std::string uri) //GET
{
	int length = uri.find_first_of('?');
	std::cout << "before, uri = " << uri << std::endl;
	std::string test = uri;
	std::cout << "middle, uri = " << uri << std::endl;
	_cgi_path = uri;
	std::cout << "after" << std::endl;
	_cgi_path.resize(length);
	std::string arg_string = &uri[length];
	std::cout << "arg_string= : [" << arg_string << "]" << std::endl;
	std::cout << "_cgi_path= : [" << _cgi_path << "]" << std::endl;
}

void Cgi::execute_cgi(std::string uri) //GET
{
	std::cout << "Execute cgi for GET: " << uri << std::endl;
	std::string path;

	int length = uri.find_first_of('?');
	path = uri;
	path.resize(length);
	build_arg_and_envp(uri);
	_execute_cgi();
}

void Cgi::execute_cgi(char **request, std::string uri) //POST
{
	std::cout << "Execute cgi for POST: " << uri << std::endl;
	std::string path;

	int length = uri.find_first_of('?');
	path = uri;
	path.resize(length);
	build_arg_and_envp(uri, request);
	_execute_cgi();
}

void Cgi::_execute_cgi()
{
	int pid;

    int wait_pid;
    pid = fork();
	std::cout << "launcher = " << _cgi_launcher << std::endl;

	std::cout << "_argv = " << _argv << std::endl;
	for(int i = 0; _argv[i]; i++)
		std::cout << _argv[i] << std::endl;

	std::cout << std::endl << "_envp = " << _envp << std::endl;
	for(int i = 0; _envp[i]; i++)
		std::cout << _envp[i] << std::endl << std::endl;
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
}

void Cgi::setup(char **envp, std::string cgi_conf)
{
	std::cout << "IN CGI CLASS : PARSE " << cgi_conf << std::endl;
	_envp = envp;
	char **tmp = ft_split(cgi_conf.c_str(), " ");
	_target = tmp[0];
	_cgi_launcher = tmp[1];
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
