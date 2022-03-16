#include "../include/Server.hpp"

Server::Server()
{}

Server::Server(Server const & other)
{
	this->client = other.client;
	this->msg_client = other.msg_client;
}

Server::~Server()
{}

Server &Server::operator=(Server const & other)
{
	this->client = other.client;
	this->msg_client = other.msg_client;
	return (*this);
}

int Server::getClient()
{
	return this->client;
}

int Server::server_accept(int socket_fd)
{
	struct sockaddr client_address;
	socklen_t client_addr_lenght;
	int new_client;

	if((new_client = accept(socket_fd, (struct sockaddr*)&client_address, (socklen_t*)&client_addr_lenght)) < 0)
	{
		std::cerr << "Error while accepting new client" << std::endl;
		close(socket_fd);
		return (1);
	}
	this->client = new_client;
	std::cout << "New client connected on server: " << std::endl;
	return (0);
}

int Server::receive_msg()
{
	char buff[1000];
	int n;
	if((n = recv(this->client, buff, sizeof(buff) - 1, 0)) < 0 || n == 0)
		return (1);
	buff[n] = '\0';
	std::cout << "Messaged received:\n" << buff << std::endl;
	this->msg_client = buff;
	return (0);
}

int Server::send_msg()
{
	//if (this->get_action() != 0)
	//	std::cerr << "action not permitted" << std::endl;
	char send_buff[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	if(send(this->client, send_buff, strlen(send_buff), 0) < 0)
	{
		std::cerr << "error: send()" <<std::endl;
		return (1);
	}
	return (0);
}

/*int Server::get_action()
{

}*/