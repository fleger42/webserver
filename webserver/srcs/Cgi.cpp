#include "../include/Cgi.hpp"

Cgi::Cgi() : _envp(NULL)
{
	//std::cout << "Class Cgi default constructor" << std::endl;
}


Cgi::Cgi(char **envp, std::string cgi_conf) : _envp(envp)
{
	//std::cout << "Class Cgi default constructor" << std::endl;
	setup(envp, cgi_conf);
}

Cgi::Cgi(Cgi const & copy)
{
	*this = copy;
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
	return(*this);
	//std::cout << "Class Cgi operator=" << std::endl;
}

char ** Cgi::build_arg_and_envp(std::string uri)
{
	std::string path;

	int length = uri.find_first_of('?');
	path = uri;
	path.resize(length);
	std::cout << "PATH= : [" << path << "]" << std::endl;
}

char ** Cgi::build_arg_and_envp(std::string uri, char **request)
{
	int nbr_tab = 0;
	while(request[nbr_tab])
		nbr_tab++;
}

void Cgi::execute_cgi(std::string uri, std::string _cgi_path) //GET
{
	std::cout << "Execute cgi for GET: " << uri << std::endl;
	std::string path;

	int length = uri.find_first_of('?');
	path = uri;
	path.resize(length);
	_execute_cgi(path, build_arg_and_envp(uri), _cgi_path);
}

void Cgi::execute_cgi(char **request, std::string uri, std::string _cgi_path) //POST
{
	std::cout << "Execute cgi for POST: " << uri << std::endl;
	std::string path;

	int length = uri.find_first_of('?');
	path = uri;
	path.resize(length);
	_execute_cgi(path, build_arg_and_envp(uri, request), _cgi_path);
}

void Cgi::_execute_cgi(std::string uri, char **arg, std::string _cgi_path)
{
	int pid;

    int wait_pid;
    pid = fork();
    if (pid == 0)
	{
        if (execve(_cgi_launcher.c_str(), arg, _envp) < 0)
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
