#include "../include/Cgi.hpp"

Cgi::Cgi() : _envp(NULL)
{
	//std::cout << "Class Cgi default constructor" << std::endl;
}


Cgi::Cgi(char **envp) : _envp(envp)
{
	//std::cout << "Class Cgi default constructor" << std::endl;

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
//./cgi_test/get-method.php?firstname=florian&lastname=Leger&submit=Envoyer 
char ** get_arg(std::string uri)
{
	std::string path;

	int length = uri.find_first_of('?');
	path = uri;
	path.resize(length);
	std::cout << "PATH= : [" << path << "]" << std::endl;
}

char ** get_arg(std::string uri, char **request)
{
	int nbr_tab = 0;
	while(request[nbr_tab])
		nbr_tab++;
}

void Cgi::execute_cgi(std::string uri, std::string cgi_path) //GET
{
	std::cout << "Execute cgi for GET: " << uri << std::endl;
	std::string path;

	int length = uri.find_first_of('?');
	path = uri;
	path.resize(length);
	_execute_cgi(path, get_arg(uri), cgi_path);
}

void Cgi::execute_cgi(char **request, std::string uri, std::string cgi_path) //POST
{
	std::cout << "Execute cgi for POST: " << uri << std::endl;
	_execute_cgi(uri, get_arg(uri, request), cgi_path);
}

void Cgi::_execute_cgi(std::string uri, char **arg, std::string cgi_path)
{
	int pid;

    int wait_pid;
    pid = fork();
    if (pid == 0)
	{
        if (execve(uri.c_str(), arg, _envp) < 0)
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