#include "../include/Server.hpp"

Server::Server() : client(0)
{}

Server::Server(Server const & other)
{
	this->client = other.client;
	this->msg_client = other.msg_client;
	this->info_serv = other.info_serv;
}

Server::Server(VirtualServer const & virtual_serv) : client(0)
{
	this->info_serv = virtual_serv;
}

Server::~Server()
{}

Server &Server::operator=(Server const & other)
{
	this->client = other.client;
	this->msg_client = other.msg_client;
	this->info_serv = other.info_serv;
	return (*this);
}

std::vector<Socket> Server::get_all_socket() const
{
	return (all_socket);
}

int Server::create_socket()
{
	std::vector<std::string> ip_list = info_serv.get_ip();
	std::vector<std::string> port_list = info_serv.get_port();
	std::vector<Socket> temp(port_list.size());

	for(int i = 0; i < port_list.size(); i++)
	{
		std::cout << "Create new socket for server " << this->info_serv.get_server_name() << 
		" with ip: " << ip_list[i] << std::endl;
		temp[i].set_ip(ip_list[i]);
		temp[i].set_port(port_list[i]);
		if(temp[i].create_socket() != 0)
			return (1);
		if (temp[i].make_bind() != 0)
			return (1);
		if (temp[i].listen_socket(30) < 0)
			return (1);
	}
	all_socket = temp;
	return (0);
}

int Server::getClient()
{
	return this->client;
}

std::string Server::get_server_name()
{
	return (info_serv.get_server_name());
}

int Server::server_accept()
{
	std::cout << "server " << info_serv.get_server_name() << " accept new connection." << std::endl;
	struct sockaddr client_address;
	socklen_t client_addr_lenght = sizeof( (socklen_t *)&client_address);

	if((this->client = accept(all_socket[0].getServerFd(), (struct sockaddr*)&client_address, (socklen_t*)&client_addr_lenght)) < 0)
	{
		std::cerr << "Error while accepting new client" << std::endl;
		return (1);
	}
	std::cout << "New client connect on server [" << get_server_name() << "]" << std::endl;
	return (0);
}

void Server::set_virtual_server(VirtualServer const & value)
{
	info_serv = value;
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
		std::string tmp = this->actionGet();
		std::string send_buff = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
		send_buff += std::to_string(tmp.size());
		send_buff += "\n\n";
		send_buff += tmp;
		if(send(this->client, send_buff.c_str(), ft_strlen(send_buff.c_str()), 0) < 0)
		{
			std::cerr << "error: send()" <<std::endl;
			return (1);
		}
		std::cout << "MESSAGE SEND" << std::endl;
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
	std::cout << "TEST 1" << std::endl;
	char **tmp;
	char *tmp2;
	tmp = ft_split(this->msg_client, " ");
	tmp2 = tmp[1];
	std::string file;
	file += tmp2;
	std::cout << "TEST 2" << std::endl;
	if (this->verif_get_location(file) != 0)
	{
		std::cerr << "Not permited to GET" << std::endl;
		return NULL;	
	}
	std::cout << "TEST 3" << std::endl;
	file = this->get_location_path(file);
	std::cout << "TEST 4" << std::endl;
	std::cout << "FILE OPEN :" << file << std::endl;
	std::ifstream input(file.c_str());
	std::stringstream buff;
	std::cout << "TEST 5" << std::endl;
	if (input.good() == 0)
	{
		std::cerr << "Fail to open file" << std::endl;
		return NULL;
	}
	buff << input.rdbuf();
	file = buff.str();
	std::cout << "TEST 6" << std::endl;
	return (file);
}

void Server::actionPost()
{

}

void Server::actionDelete()
{

}

void Server::close_all_fd()
{
	for(std::vector<Socket>::iterator it = all_socket.begin(); it != all_socket.end(); it++)
		close(it->getServerFd());
}

int Server::verif_get_location(std::string file)
{
	int i = 0;
	int tmp = 0;
	std::string path_loca;
	std::string path_tmp;
	std::vector<Location> *loca = this->info_serv.get_location_list();
	std::vector<Location>::iterator it = loca->begin();
	while (it != loca->end())
	{
		i = 0;
		path_tmp = it->get_path();
		while (file[i] && path_tmp[i] && file[i] == path_tmp[i])
			i++;
		if (i > tmp && i >= path_tmp.size())
		{
			tmp = i;
			path_loca = path_tmp;
		}
		it++;
	}
	it = loca->begin();
	while (it != loca->end())
	{
		if (path_loca == it->get_path())
		{
			if (it->get_get() == 1)
				return 0;
			else
				return 1;
		}
		it++;
	}
	return 0;
}

std::string Server::get_location_path(std::string file)
{
	int i = 0;
	int tmp = 0;
	std::string path_loca;
	std::string path_tmp;
	std::string ret;
	std::vector<Location> *loca = this->info_serv.get_location_list();
	std::vector<Location>::iterator it = loca->begin();
	std::cout << "SUB TEST 1" << std::endl;
	while (it != loca->end())
	{
		i = 0;
		path_tmp = it->get_path();
		while (file[i] && path_tmp[i] && file[i] == path_tmp[i])
			i++;
		if (i > tmp && i >= path_tmp.size())
		{
			tmp = i;
			path_loca = path_tmp;
		}
		it++;
	}
	it = loca->begin();
	std::cout << "SUB TEST 2" << std::endl;
	while (it != loca->end())
	{
		if (path_loca == it->get_path())
		{
			path_tmp = it->get_root();
			std::cout << "SUB TEST 3" << std::endl;
			if (!path_tmp.empty())
			{
				ret = path_tmp;
				if (path_loca.size() - file.size() >= 0)
				{
					ret += path_loca;
					if (ret[ret.size() - 1] != '/')
						ret += '/';
					ret += it->get_index_list().front();
					return ret;
				}
				ret += file;
				return ret;
			}
			path_tmp = this->info_serv.get_root();
			std::cout << "SUB TEST 4" << std::endl;
			if (!path_tmp.empty())
			{
				ret = path_tmp;
				std::cout << "path loca :" << path_loca << std::endl;
				std::cout << "ret (root) :" << ret << std::endl;
				std::cout << "file :" << file << std::endl;
				if (path_loca.size() - file.size() >= 0)
				{
					ret += path_loca;
					if (ret[ret.size() - 1] != '/')
						ret += '/';
					ret += it->get_index_list().front();
					return ret;
				}
				ret += file;
				return ret;
			}
			std::cout << "SUB TEST 5" << std::endl;
			if (path_loca.size() - file.size() >= 0)
			{
				ret += path_loca;
				if (ret[ret.size() - 1] != '/')
					ret += '/';
				ret += it->get_index_list().front();
				return ret;
			}
			ret += file;
			return ret;
		}
		it++;
	}
	ret+= this->info_serv.get_root();
	ret += file;
	return ret;
}