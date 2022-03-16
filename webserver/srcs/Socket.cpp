#include "../include/Socket.hpp"


Socket::Socket()
{}

Socket::~Socket()
{}

Socket::Socket(Socket const & other)
{
	this->server_address = other.server_address;
	this->server_fd = other.server_fd;
}

Socket &Socket::operator=(Socket const & other)
{
	this->server_address = other.server_address;
	this->server_fd = other.server_fd;
	return (*this);
}

int Socket::getServerFd(int n)
{
	return this->server_fd.at(n);
}

int Socket::getServerFd(char *port)
{
	std::vector<sockaddr_in>::iterator it = this->server_address.begin();
	std::vector<int>::iterator it2 = this->server_fd.begin();
	while (it.base()->sin_port != htons(std::atoi(port)))
	{
		it++;
		it2++;
	}
	return (*it2);
}

std::vector<int> Socket::getServerFd()
{
	return this->server_fd;
}

sockaddr_in Socket::getServerAddress(int n)
{
	return this->server_address.at(n);
}

sockaddr_in Socket::getServerAddress(char *port)
{
	std::vector<sockaddr_in>::iterator it = this->server_address.begin();
	std::vector<int>::iterator it2 = this->server_fd.begin();
	while (it.base()->sin_port != htons(std::atoi(port)))
	{
		it++;
		it2++;
	}
	return (*it);
}

std::vector<sockaddr_in> Socket::getServerAddress()
{
	return this->server_address;
}

void Socket::create_socket(char *port)
{
	struct sockaddr_in tmp;
	memset((char*)&tmp, 0, sizeof(tmp));
	tmp.sin_family = AF_INET;
	tmp.sin_addr.s_addr = htonl(INADDR_ANY);
	tmp.sin_port = htons(std::atoi(port));

	this->server_address.push_back(tmp);

	int ret = socket(AF_INET, SOCK_STREAM, 0);
	this->server_fd.push_back(ret);
	if (ret < 0)
		std::cerr << "Error with socked creation" << std::endl;
}

int Socket::find_and_bind(char *port)
{
	std::vector<sockaddr_in>::iterator it = this->server_address.begin();
	std::vector<int>::iterator it2 = this->server_fd.begin();
	while (it->sin_port != htons(std::atoi(port)))
	{
		it++;
		it2++;
	}
	if (bind(*it2, (struct sockaddr*)&*it, sizeof(*it)) < 0)
	{
		std::cerr << "Error with biding" << std::endl;
		close(*it2);
		return (1);
	}
	return (0);
}

int Socket::listen_socket(char *port, int connec)
{
	std::vector<sockaddr_in>::iterator it = this->server_address.begin();
	std::vector<int>::iterator it2 = this->server_fd.begin();
	while (it->sin_port != htons(std::atoi(port)))
	{
		it++;
		it2++;
	}
	if (listen((*it2), connec) < 0)
	{
		std::cerr << "Error while listening" << std::endl;
		close(*it2);
		return (1);
	}
	return (0);
}