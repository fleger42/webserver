#include "../include/Socket.hpp"

Socket::Socket()
{}

Socket::Socket(std::string port, std::string ip) : port(port), ip(ip)
{

}

Socket::~Socket()
{
	//std::cout << "Class Socket destructor" << std::endl;
}

Socket::Socket(Socket const & other)
{
	this->server_address = other.server_address;
	this->server_fd = other.server_fd;
	this->port = other.port;
	this->ip = other.ip;
}

void Socket::set_port(std::string port)
{
	this->port = port;
}

void Socket::set_ip(std::string ip)
{
	this->ip = ip;
}

Socket &Socket::operator=(Socket const & other)
{
	this->server_address = other.server_address;
	this->server_fd = other.server_fd;
	this->port = other.port;
	this->ip = other.ip;
	return (*this);
}

int Socket::getServerFd()
{
	return this->server_fd;
}

sockaddr_in Socket::getServerAddress()
{
	return this->server_address;
}

int Socket::create_socket()
{
	struct sockaddr_in tmp;
	memset((char*)&tmp, 0, sizeof(tmp));
	tmp.sin_family = AF_INET;
	tmp.sin_addr.s_addr = htonl(INADDR_ANY);
	tmp.sin_port = htons(atoi(port.c_str()));

	this->server_address = tmp;
	int ret;
	if((ret = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "Error with socked creation" << std::endl;
		return (1);
	}
	this->server_fd = ret;
	return (0);
}

int Socket::make_bind()
{
	if (bind(server_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
	{
		std::cerr << "Error with biding" << std::endl;
		close(server_fd);
		return (1);
	}
	return (0);
}

int Socket::listen_socket(int max_queue)
{
	if (listen((server_fd), max_queue) < 0)
	{
		std::cerr << "Error while listening" << std::endl;
		close(server_fd);
		return (1);
	}
	if (fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Error:fcntl" << std::endl;
		close(server_fd);
		return (1);
	}
	return (0);
}
