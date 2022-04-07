#include "../include/Cgi.hpp"

Cgi::Cgi() : _cgi_path(), _wait_pid(), _arg(), _envp()
{
	//std::cout << "Class Cgi default constructor" << std::endl;
}


Cgi::Cgi(char **envp) : _cgi_path(), _wait_pid(), _arg(), _envp()
{
	//std::cout << "Class Cgi default constructor" << std::endl;
	parse_Cgi_file(filename);
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
	if(this != &copy)
		*this = copy;
	return(*this);
	//std::cout << "Class Cgi operator=" << std::endl;
}

void Cgi::execute_cgi()
{
    int pid;

    int wait_pid;
    pid = fork();
    if (pid == 0)
	{
        if (execve(_cgi_path.c_str(), _arg, _envp) < 0)
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