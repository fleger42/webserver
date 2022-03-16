#include "../include/Server.hpp"

Server::Server()
{}

Server::Server(Server const & other)
{
	this->client = other.client;
	this->msg_client = other.msg_client;
}

Server::Server(VirtualServer const & virtual_serv)
{
	this->info_serv = virtual_serv;
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
	int ret = this->get_action();
	if (ret < 0)
		std::cerr << "action not permited" << std::endl;
	else if (ret == 1)
	{
		std::cout << "before action get" << std::endl;
		std::string tmp = this->actionGet();
		std::cout << "test tmp :" << tmp << std::endl;
		std::string send_buff = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: ";
		std::cout << "test send_buff :" << send_buff << std::endl;
		send_buff += std::to_string(tmp.size());
		std::cout << "test send_buff :" << send_buff << std::endl;
		send_buff += "\n\n";
		std::cout << "test send_buff :" << send_buff << std::endl;
		send_buff += tmp;
		std::cout << "test send_buff :" << send_buff << std::endl;
		std::cerr << send_buff << std::endl;
		//char send_buff[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
		if(send(this->client, send_buff.c_str(), ft_strlen(send_buff.c_str()), 0) < 0)
		{
			std::cerr << "error: send()" <<std::endl;
			return (1);
		}
	}
	else if (ret == 2)
		this->actionPost();
	else
		this->actionDelete();

	return (0);
}

int Server::get_action()
{
	std::string str = this->msg_client;
	if (str.find("GET") != std::string::npos)
		return (1);
	if (str.find("POST") != std::string::npos)
		return (2);
	if (str.find("DELETE") != std::string::npos)
		return (3);
	return (-1);
}

std::string Server::actionGet()
{
	/*if (this->info_serv.get_get() == 0)
	{
		std::cerr << "Not permited to GET" << std::endl;
		return NULL;
	}*/

	char **tmp;
	char *tmp2;
	tmp = ft_split(this->msg_client, " ");
	tmp2 = tmp[1];
	std::string file = "www/test.html";
//	file += tmp2;
	std::ifstream input(file.c_str());
	std::stringstream buff;
	if (input.good() == 0)
	{
		std::cerr << "Fail to open file" << std::endl;
		return NULL;
	}
	buff << input.rdbuf();
	file = buff.str();
	std::cout << "Exit action get" << file << std::endl;
	return (file);
}

void Server::actionPost()
{

}

void Server::actionDelete()
{

}