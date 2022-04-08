#include "../include/Server.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

Server::Server() : client(0)
{}

Server::Server(Server const & other)
{
	this->client = other.client;
	this->msg_client = other.msg_client;
	this->info_serv = other.info_serv;
	this->all_socket = other.all_socket;
	this->cgi_exec = other.cgi_exec;
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
	this->all_socket = other.all_socket;
	this->cgi_exec = other.cgi_exec;
	return (*this);
}

void Server::set_cgi(Cgi cgi_exec)
{
	this->cgi_exec = cgi_exec;
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
		//std::cout << "Create new socket for server " << this->info_serv.get_server_name() << 
		//" with ip: " << ip_list[i] << std::endl;
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

VirtualServer Server::get_info_serv()
{
	return this->info_serv;
}

std::string Server::get_server_name()
{
	return (info_serv.get_server_name());
}

int Server::server_accept()
{
	//std::cout << "server " << info_serv.get_server_name() << " accept new connection." << std::endl;
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
	char buff[10000];
	int n;
	if((n = recv(this->client, buff, sizeof(buff) - 1, 0)) < 0 || n == 0)
		return (1);
	buff[n] = '\0';
	//std::cout << "[Messaged received:\n" << buff << "]" << std::endl;
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
		if(tmp.empty())
			return (0);
		std::string send_buff = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
		send_buff += std::to_string(tmp.size());
		send_buff += "\n\n";
		send_buff += tmp;
		if(send(this->client, send_buff.c_str(), ft_strlen(send_buff.c_str()), 0) < 0)
		{
			std::cerr << "error: send()" <<std::endl;
			return (1);
		};
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
	char **tmp;
	char *tmp2;
	int i = -1;
	tmp = ft_split(this->msg_client, " ");
	tmp2 = tmp[1];
	std::string file;
	std::string file_tmp;
	file += tmp2;
	if (this->verif_get_location(file) != 0 && this->info_serv.get_get() == 0)
	{
		std::cerr << "Not permited to GET" << std::endl;
		return "";	
	}
	file_tmp = this->get_location_path(file, i);
	std::ifstream input(file_tmp); // HARDCODE
	struct stat path_stat;
	stat(file_tmp.c_str(), &path_stat);
	int o = S_ISREG(path_stat.st_mode);
	std::stringstream buff;
	//std::cout << "FILE_TMP =" << file_tmp << std::endl;
	if(file_tmp.find(".php") != std::string::npos)
	{
		//cgi_exec.execute_cgi(file_tmp, this->get_info_serv().get);
	}
	if (input.good() == 0 || o == 0)
	{
		while (file_tmp != "error")
		{
			i++;
			input.close();
			file_tmp = this->get_location_path(file, i);
			input.open(file_tmp);
			if (input.good() == 1)
			{
				buff << input.rdbuf();
				file = buff.str();
				for(int i = 0; tmp[i]; i++)
					free(tmp[i]);
				free(tmp);
				return (file_tmp);
			}
		}
		std::cerr << "Fail to open file ["  << file_tmp << "]" << std::endl;
		for(int i = 0; tmp[i]; i++)
			free(tmp[i]);
		free(tmp);
		return "";
	}
	buff << input.rdbuf();
	file = buff.str();
	for(int i = 0; tmp[i]; i++)
		free(tmp[i]);
	free(tmp);
	return (file);
}

std::string Server::actionPost()
{
	//std::cout << "POSTTTTTTTT" << std::endl;
	char **tmp;
	char *tmp2;
	int i = -1;
	tmp = ft_split(this->msg_client, " ");
	tmp2 = tmp[1];
	std::string file;
	std::string file_tmp;
	file += tmp2;
	//FAUT FAIRE l'EQUIVALENT POUR POST
	/*if (this->verif_get_location(file) != 0 && this->info_serv.get_get() == 0)
	{
		std::cerr << "Not permited to GET" << std::endl;
		return "";	
	}*/
	//FAUT FAIRE l'EQUIVALENT POUR POST
	file_tmp = this->get_location_path(file, i);
	//std::cout << "FILE_TMP =" << file_tmp << std::endl;
	std::ifstream input(file_tmp); // HARDCODE
	struct stat path_stat;
	stat(file_tmp.c_str(), &path_stat);
	int o = S_ISREG(path_stat.st_mode);
	std::stringstream buff;
	if(file_tmp.find(".php") != std::string::npos)
		//cgi_exec.execute_cgi(tmp, file_tmp, );
	if (input.good() == 0 || o == 0)
	{
		while (file_tmp != "error")
		{
			i++;
			input.close();
			file_tmp = this->get_location_path(file, i);
			input.open(file_tmp);
			if (input.good() == 1)
			{
				buff << input.rdbuf();
				file = buff.str();
				for(int i = 0; tmp[i]; i++)
					free(tmp[i]);
				free(tmp);
				return (file_tmp);
			}
		}
		std::cerr << "Fail to open file ["  << file_tmp << "]" << std::endl;
		for(int i = 0; tmp[i]; i++)
			free(tmp[i]);
		free(tmp);
		return "";
	}
	buff << input.rdbuf();
	file = buff.str();
	for(int i = 0; tmp[i]; i++)
		free(tmp[i]);
	free(tmp);
	return (file);
}

void Server::actionDelete()
{
	if (this->info_serv.get_delete() == 0)
	{
		std::cerr << "Not permited to DELETE" << std::endl;
		return ;
	}
}

void Server::close_all_fd()
{
	for(std::vector<Socket>::iterator it = all_socket.begin(); it != all_socket.end(); it++)
		close(it->getServerFd());
}

Location &Server::get_request_location(std::string request)
{
	int i = 0;
	int tmp = 0;
	Location ret;
	std::string path_loca;
	std::string path_tmp;
	std::vector<Location> *loca = this->info_serv.get_location_list();
	std::vector<Location>::iterator it = loca->begin();
	while (it != loca->end())
	{
		i = 0;
		path_tmp = it->get_path();
		while (request[i] && path_tmp[i] && request[i] == path_tmp[i])
			i++;
		if (i > tmp && i >= path_tmp.size())
		{
			tmp = i;
			path_loca = path_tmp;
		}
		it++;
	}
	it = loca->begin();
	i = 0;
	while (it != loca->end())
	{
		if (path_loca == it->get_path())
		{
			ret = loca->at(i);
			return (ret);
		}
		it++;
		i++;
	}
	return (ret);
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

std::string Server::get_location_path(std::string file, int index)
{
	int i = 0;
	int tmp = 0;
	std::string path_loca;
	std::string path_tmp;
	std::string file_tmp = file;
	std::string ret;
	std::vector<Location> *loca = this->info_serv.get_location_list();
	std::vector<Location>::iterator it = loca->begin();
	if (file_tmp[file_tmp.size() - 1] != '/')
		file_tmp += '/';
	while (it != loca->end())
	{
		i = 0;
		path_tmp = it->get_path();
		while (file_tmp[i] && path_tmp[i] && file_tmp[i] == path_tmp[i])
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
			path_tmp = it->get_root();
			if (!path_tmp.empty())
			{
				ret = path_tmp;
				if (path_loca.size() - file.size() >= 0 && index != -1)
				{
					ret += path_loca;
					ret = this->add_index(ret, index, it);
					return ret;
				}
				ret += file;
				return ret;
			}
			path_tmp = this->info_serv.get_root();
			if (!path_tmp.empty())
			{
				ret = path_tmp;
				if (path_loca.size() - file.size() >= 0 && index != -1)
				{
					ret += path_loca;
					ret = this->add_index(ret, index, it);
					return ret;
				}
				ret += file;
				return ret;
			}
			if (path_loca.size() - file.size() >= 0 && index != -1)
			{
				ret = path_loca;
				ret = this->add_index(ret, index, it);
				return ret;
			}
			ret += file;
			return ret;
		}
		it++;
	}
	ret += this->info_serv.get_root();
	ret += file;
	return ret;
}

std::string Server::add_index(std::string ret, int index, std::vector<Location>::iterator it)
{
	if (it->get_index_list().empty() == 0)
	{
		if (index >= it->get_index_list().size())
			index -= it->get_index_list().size();
		else
		{
			if (ret[ret.size() - 1] != '/')
				ret += '/';
			ret += it->get_index_list()[index];
			return ret;
		}
	}
	if (this->info_serv.get_index_list().empty() == 0)
	{
		if (index >= this->info_serv.get_index_list().size())
			return "error";
		else
		{
			if (ret[ret.size() - 1] != '/')
				ret += '/';
			ret += this->info_serv.get_index_list()[index];
			return ret;
		}
	}
	return "error";
}