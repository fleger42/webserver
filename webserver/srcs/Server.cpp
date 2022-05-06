#include "../include/Server.hpp"
#include "../include/Cgi.hpp"
#include "../include/Location.hpp"
#include "../include/Socket.hpp"

Server::Server() : client(0)
{}

Server::Server(Server const & other)
{
	this->client = other.client;
	this->msg_client = other.msg_client;
	this->info_serv = other.info_serv;
	this->all_socket = other.all_socket;
	this->cgi_exec = other.cgi_exec;
	this->error_class = other.error_class;
}

Server::Server(VirtualServer const & virtual_serv) : client(0)
{
	this->info_serv = virtual_serv;
}

Server::~Server()
{
	//std::cout << "Class Server destructor" << std::endl;
}

Server &Server::operator=(Server const & other)
{
	this->client = other.client;
	this->msg_client = other.msg_client;
	this->info_serv = other.info_serv;
	this->all_socket = other.all_socket;
	this->cgi_exec = other.cgi_exec;
	this->error_class = other.error_class;
	return (*this);
}

void Server::set_cgi(std::vector<Cgi> param)
{
	this->cgi_exec = param;
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

	for(size_t i = 0; i < port_list.size(); i++)
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

Error &Server::get_error_class()
{
	return this->error_class;
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
	if (fcntl(client, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Error:fcntl" << std::endl;
		close(client);
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
	fd_set read_set;
	fd_set read_dump;
	int status = 0;
	struct timeval select_timeout;

	memset(buff, 0, 10000);
	select_timeout.tv_sec = 1;
	select_timeout.tv_usec = 0;
	FD_ZERO(&read_dump);
	FD_SET(client, &read_dump);
	while(status == 0 && g_ctrl_c_called == 0)
	{
			FD_ZERO(&read_set);
			read_set = read_dump;
			usleep(2000);
			if((status = select(client + 1, &read_set, NULL, NULL, &select_timeout)) < 0)
			{
				if(g_ctrl_c_called == 1)
				{
						std::cerr << "Closing webserver..." << std::endl;
						exit(0);
				}
				std::cerr << "Error: select()" << std::endl;
				exit(1);
			}
	}
	if((n = recv(this->client, buff, sizeof(buff) - 1, 0)) < 0 || n == 0)
		return (1);
	buff[n] = '\0';
	std::cout << "[Messaged received:\n" << buff << "]" << std::endl;
	this->msg_client = buff;
	
	return (0);
}

int	Server::verif_header()
{
	int i = 0;
	int is_host = 0;
	while(msg_client[i] && msg_client[i] != '\n')
		i++;
	std::string mut = msg_client;
	std::string tmp;
	mut.resize(i - 1);
	char **double_tab =  ft_split(mut.c_str(), " ");
	i = 0;
	while(double_tab[i])
		i++;
	if(i != 3)
	{
		free_double_tab(double_tab);
		return(1);
	}
	std::string first = double_tab[0];
	std::string second = double_tab[1];
	std::string third = double_tab[2];
	free_double_tab(double_tab);
	if(first.compare("GET") == 1 && first.find(" POST ") == 1 && first.find(" DELETE ") == 1)
		return(1);
	if(second[0] != '/')
		return(1);
	if(third.find("HTTP/1.1") == std::string::npos && third.find("HTTP/1.0") == std::string::npos)
		return(1);
	double_tab = ft_split(this->msg_client.c_str(), "\n");
	for(int i = 0; double_tab[i];i++)
	{
		tmp = double_tab[i];
		if(tmp.find("Host:") != std::string::npos && is_host)
		{
			free_double_tab(double_tab);	
			return (1);
		}
		if(tmp.find("Host:") != std::string::npos)
			is_host = 1;
	}
	free_double_tab(double_tab);
	return(0);
}

size_t Server::body_size(std::string header)
{
	int i = 0;
	while(header[i])
	{
		if(header[i] == '\n' && (header[i + 1] == '\r' && header[i + 2] == '\n'))
		{
			return (ft_strlen(&header[i + 3]));
		}
		i++;
	}
	return (0);
}

int Server::send_msg()
{
	int ret = this->get_action();
	std::string tmp;
	char * stock;
	if (ret < 0  || verif_header() == 1)
	{
		std::string send_buff;
		send_buff = "HTTP/1.1 405 Method Not Allowed\nContent-Type: text/html; charset=utf-8\n\n";
		if(send(this->client, send_buff.c_str(), ft_strlen(send_buff.c_str()), 0) < 0)
		{
			std::cerr << "error: send()" <<std::endl;
			this->error_class.SetErrorCode("200");
			this->error_class.SetErrorMsg("OK");
			this->error_class.SetContentMsg("Content-Type: text/html; charset=utf-8\n\n");
			return (1);
		};
		this->error_class.SetErrorCode("200");
		this->error_class.SetErrorMsg("OK");
		this->error_class.SetContentMsg("Content-Type: text/html; charset=utf-8\n\n");
	}
	else if (ret == 1)
	{
		tmp = this->actionGet();
		std::string send_buff;
		std::cout << "RESPONSE SENT" << std::endl;
		stock = ft_itoa(body_size(tmp));
		send_buff = "HTTP/1.1 " + this->error_class.GetErrorCode() + " " + this->error_class.GetErrorMsg() + "\nContent-Length: " + stock + "\n";
		free(stock);
		if(tmp.find("Content-Type:") == std::string::npos)
			send_buff += this->error_class.GetContentMsg();
		send_buff += tmp;
		//std::cout << "send_buff [" << send_buff << "]" << std::endl;
		if(send(this->client, send_buff.c_str(), ft_strlen(send_buff.c_str()), 0) < 0)
		{
			std::cerr << "error: send()" <<std::endl;
			this->error_class.SetErrorCode("200");
			this->error_class.SetErrorMsg("OK");
			this->error_class.SetContentMsg("Content-Type: text/html; charset=utf-8\n\n");
			return (1);
		};
		this->error_class.SetErrorCode("200");
		this->error_class.SetErrorMsg("OK");
		this->error_class.SetContentMsg("Content-Type: text/html; charset=utf-8\n\n");
	}
	else if (ret == 2)
	{
		tmp = this->actionPost();
		std::string send_buff;
		stock = ft_itoa(body_size(tmp));
		send_buff = "HTTP/1.1 " + this->error_class.GetErrorCode() + " " + this->error_class.GetErrorMsg() + "\nContent-Length: " + stock + "\n";
		free(stock);
		if(tmp.find("Content-Type:") == std::string::npos)
			send_buff += this->error_class.GetContentMsg();
		send_buff += tmp;
		if(send(this->client, send_buff.c_str(), ft_strlen(send_buff.c_str()), 0) < 0)
		{
			std::cerr << "error: send()" <<std::endl;
			this->error_class.SetErrorCode("200");
			this->error_class.SetErrorMsg("OK");
			this->error_class.SetContentMsg("Content-Type: text/html; charset=utf-8\n\n");
			return (1);
		};
		this->error_class.SetErrorCode("200");
		this->error_class.SetErrorMsg("OK");
		this->error_class.SetContentMsg("Content-Type: text/html; charset=utf-8\n\n");
	}
	else
	{
		tmp = this->actionDelete();
		std::string send_buff;
		stock = ft_itoa(body_size(tmp));
		send_buff = "HTTP/1.1 " + this->error_class.GetErrorCode() + " " + this->error_class.GetErrorMsg() + "\nContent-Length: " + stock + this->error_class.GetContentMsg() + tmp;
		free(stock);
		if(send(this->client, send_buff.c_str(), ft_strlen(send_buff.c_str()), 0) < 0)
		{
			std::cerr << "error: send()" <<std::endl;
			this->error_class.SetErrorCode("200");
			this->error_class.SetErrorMsg("OK");
			this->error_class.SetContentMsg("Content-Type: text/html; charset=utf-8\n\n");
			return (1);
		};
		this->error_class.SetErrorCode("200");
		this->error_class.SetErrorMsg("OK");
		this->error_class.SetContentMsg("Content-Type: text/html; charset=utf-8\n\n");
	}

	return (0);
}

int Server::get_action()
{
	std::string str = this->msg_client;
	if(str.empty())
		return (-1);
	if (str.find("GET") != std::string::npos)
		return (1);
	if (str.find("POST") != std::string::npos)
		return (2);
	if (str.find("DELETE") != std::string::npos)
		return (3);
	return (-1);
}

/*
	FIND IF YOU NEED OR NOT TO USE CGI
	Return -1 if you dont need cgi.
	If you need cgi return the index of it or -2 for error
*/
int	Server::check_cgi(std::string uri, std::vector<Cgi> cgi) 
{
	std::string extension;
	std::string final_conf_target;
	int index = 0;
	std::string::iterator it = uri.begin();
	it++;
	while(it != uri.end() && *it != '.')
		it++;
	for(; it != uri.end() && *it != '?'; it++)
		extension.push_back(*it);
	for(std::vector<Cgi>::iterator it = cgi.begin(); it != cgi.end(); it++)
	{
		final_conf_target = &it->get_target()[1];
		if(extension.compare(final_conf_target) == 0)
			return (index);
		index++;
	}
	if(extension.compare(".php") == 0)
		return (-2);
	if(extension.compare(".cgi") == 0)
		return (-2);
	return (-1);
}

std::string Server::autoindex(std::string uri, std::string real_path)
{
	std::string ret;
	DIR *dir;
	std::cout << "IN AUTO INDEX" << std::endl;
	struct dirent *ent;
	if ((dir = opendir(uri.c_str())) != NULL)
	{
		ret += "<html>";
		ret += "<head><title>Index of";
		ret += real_path + "</title></head>\n";
		ret += "<body>";
		ret += "<h1>Index of ";
		ret += real_path + "</h1><hr><pre>";
		while ((ent = readdir(dir)) != NULL)
		{
			if(strcmp(ent->d_name, ".") != 0)
			{
				ret += "<a href=";
				ret += real_path;
				if(real_path[real_path.size() - 1] != '/')
					ret += '/';
				ret += ent->d_name;
				ret += ">";
				ret += ent->d_name;
				ret += " </a>\n";
			}
		}
		ret += "</pre><hr></body>";
		ret += "</html>";
		closedir(dir);
	} 
	else 
		return this->error_class.error_404();
	return(ret);
}

std::string Server::actionGet()
{
	char **tmp;
	char *tmp2;
	std::ifstream input;
	std::stringstream buff;
	int i = -1;
	//std::cout << "msg client from get = " << this->msg_client << std::endl;
	tmp = ft_split(this->msg_client.c_str(), " ");
	tmp2 = tmp[1];
	std::string file;
	std::string file_tmp;
	//std::cout << "[" << msg_client << "]" << std::endl;
	file += tmp2;
	if (this->verif_get_location(file) != 0 && this->info_serv.get_get() == 0)
	{
		free_double_tab(tmp);
		return this->error_class.error_403();//Ou erreur 405 jsp
	}
	Location redirection = get_request_location(file);
	if(redirection.get_path().empty() == 1)
	{
		if(this->info_serv.get_body_size() < body_size(msg_client))
		{
			free_double_tab(tmp);
			return this->error_class.error_431();
		}
	}
	else
	{
		if(redirection.get_body_size() < body_size(msg_client))
		{
			free_double_tab(tmp);
			return this->error_class.error_431();
		}
	}
	file_tmp = this->get_location_path(file, i);
	if (this->error_class.GetRedir() == 0)
	{
		if (this->info_serv.get_redirect_list().empty() == 0)
		{
			free_double_tab(tmp);
			this->error_class.SetRedir(1);
			if (this->info_serv.get_redirect_list().begin()->first == 301)
				return this->error_class.error_301(this->info_serv.get_redirect_list().begin()->second);
			if (this->info_serv.get_redirect_list().begin()->first == 302)
				return this->error_class.error_302(this->info_serv.get_redirect_list().begin()->second);
			return this->error_class.error_404();
		}
		if (redirection.get_redirect_list().empty() == 0)
		{
			free_double_tab(tmp);
			this->error_class.SetRedir(1);
			if (redirection.get_redirect_list().begin()->first == 301)
				return this->error_class.error_301(redirection.get_redirect_list().begin()->second);
			if (redirection.get_redirect_list().begin()->first == 302)
				return this->error_class.error_302(redirection.get_redirect_list().begin()->second);
			return this->error_class.error_404();
		}
	}
	this->error_class.SetRedir(0);
	int o = -1;
	struct stat info;

	std::string file_tmp_without_arg = file_tmp;
	int temp_size = 0;
	while(file_tmp_without_arg[temp_size] && file_tmp_without_arg[temp_size] != '?')
		temp_size++;
	file_tmp_without_arg.resize(temp_size);

	if( stat(file_tmp_without_arg.c_str(), &info ) != 0)
	{
		free_double_tab(tmp);
		return this->error_class.error_404();
	}
	else if( info.st_mode & S_IFDIR )
		o = 0;
	Location loc = get_request_location(file);
	int ret_check_cgi;
	if(loc.get_cgi_exec().empty() == 0)
	{
		ret_check_cgi = check_cgi(file_tmp, loc.get_cgi_exec());
		if(ret_check_cgi != -1 && ret_check_cgi != -2)
		{
			free_double_tab(tmp);
			return (loc.get_cgi_exec()[ret_check_cgi].execute_cgi(file_tmp));
		}
	}
	else
	{
		ret_check_cgi = check_cgi(file_tmp, this->cgi_exec);
		if(ret_check_cgi != -1 && ret_check_cgi != -2)
		{
			free_double_tab(tmp);
			return (cgi_exec[ret_check_cgi].execute_cgi(file_tmp));
		}
	}
	if(ret_check_cgi == -2)
	{
		free_double_tab(tmp);
		return this->error_class.error_404();
	}
	else
	{
		Location temp_location = get_request_location(file);
		input.open(file_tmp.c_str());
		if ( (input.rdstate() & std::ifstream::failbit ) != 0 )
		{
			free_double_tab(tmp);
			return this->error_class.error_404();
		}
		if (o == 0 && temp_location.get_autoindex() == 1)
		{
			free_double_tab(tmp);
			return (autoindex(file_tmp, file));
		}
		else if (o == 0)
		{
			
			while (file_tmp != "error")
			{
				i++;
				input.close();
				file_tmp = this->get_location_path(file, i);
				input.open(file_tmp.c_str());
				if (input.good() == 1)
				{
					if(loc.get_cgi_exec().empty() == 0)
					{
						ret_check_cgi = check_cgi(file_tmp, loc.get_cgi_exec());
						if(ret_check_cgi != -1 && ret_check_cgi != -2)
						{
							free_double_tab(tmp);
							return (loc.get_cgi_exec()[ret_check_cgi].execute_cgi(file_tmp));
						}
					}
					else
					{
						ret_check_cgi = check_cgi(file_tmp, this->cgi_exec);
						if(ret_check_cgi != -1 && ret_check_cgi != -2)
						{
							free_double_tab(tmp);
							return (cgi_exec[ret_check_cgi].execute_cgi(file_tmp));
						}
					}
					if(ret_check_cgi == -2)
					{
						free_double_tab(tmp);
						return this->error_class.error_404();
					}
					Location bla;
					bla = get_request_location(file);
					if (bla.get_path().find("/dowload/") != std::string::npos)
						this->error_class.SetContentMsg("Content-Disposition: attachment\nContent-Type: text/html; charset=utf-8\n\n");
					buff << input.rdbuf();
					file_tmp = buff.str();
					free_double_tab(tmp);
					return (file_tmp);
				}
			}
			//std::cerr << "Fail to open file ["  << file_tmp << "]" << std::endl;
			free_double_tab(tmp);
			return this->error_class.error_404();
		}
		Location bla;
		bla = get_request_location(file);
		if (bla.get_path().find("/dowload/") != std::string::npos)
			this->error_class.SetContentMsg("Content-Disposition: attachment\nContent-Type: text/html; charset=utf-8\n\n");
		buff << input.rdbuf();
		file_tmp = buff.str();
		//std::cout << "file_tmp" << file_tmp << std::endl;
		free_double_tab(tmp);
		return (file_tmp);
	}
	return this->error_class.error_404();
}

std::string Server::actionPost()
{
	char **tmp;
	char *tmp2;
	std::ifstream input;
	std::stringstream buff;
	int i = -1;
	//std::cout << "msg client from get = " << this->msg_client << std::endl;
	tmp = ft_split(this->msg_client.c_str(), " ");
	tmp2 = tmp[1];
	std::string file;
	std::string file_tmp;
	//std::cout << "[" << msg_client << "]" << std::endl;
	file += tmp2;
	if (this->verif_post_location(file) != 0 && this->info_serv.get_post() == 0)
	{
		free_double_tab(tmp);
		return this->error_class.error_403();//Ou erreur 405 jsp
	}
	Location redirection = get_request_location(file);
	if(redirection.get_path().empty() == 1)
	{
		if(this->info_serv.get_body_size() < body_size(msg_client))
			return this->error_class.error_431();
	}
	else
	{
		if(redirection.get_body_size() < body_size(msg_client))
			return this->error_class.error_431();
	}
	file_tmp = this->get_location_path(file, i);
	if (this->error_class.GetRedir() == 0)
	{
		if (this->info_serv.get_redirect_list().empty() == 0)
		{
			free_double_tab(tmp);
			this->error_class.SetRedir(1);
			if (this->info_serv.get_redirect_list().begin()->first == 301)
				return this->error_class.error_301(this->info_serv.get_redirect_list().begin()->second);
			if (this->info_serv.get_redirect_list().begin()->first == 302)
				return this->error_class.error_302(this->info_serv.get_redirect_list().begin()->second);
			return this->error_class.error_404();
		}
		if (redirection.get_redirect_list().empty() == 0)
		{
			free_double_tab(tmp);
			this->error_class.SetRedir(1);
			if (redirection.get_redirect_list().begin()->first == 301)
				return this->error_class.error_301(redirection.get_redirect_list().begin()->second);
			if (redirection.get_redirect_list().begin()->first == 302)
				return this->error_class.error_302(redirection.get_redirect_list().begin()->second);
			return this->error_class.error_404();
		}
	}
	this->error_class.SetRedir(0);
	int o = -1;
	struct stat info;

	std::string file_tmp_without_arg = file_tmp;
	int temp_size = 0;
	while(file_tmp_without_arg[temp_size] && file_tmp_without_arg[temp_size] != '?')
		temp_size++;
	file_tmp_without_arg.resize(temp_size);

	if( stat(file_tmp_without_arg.c_str(), &info ) != 0)
	{
		free_double_tab(tmp);
		return this->error_class.error_404();
	}
	else if( info.st_mode & S_IFDIR )
		o = 0;
	Location loc = get_request_location(file);
	int ret_check_cgi;
	if(loc.get_cgi_exec().empty() == 0)
	{
		ret_check_cgi = check_cgi(file_tmp, loc.get_cgi_exec());
		if(ret_check_cgi != -1 && ret_check_cgi != -2)
		{
			std::string str_holder = loc.get_cgi_exec()[ret_check_cgi].execute_cgi(tmp, file_tmp);
			free_double_tab(tmp);
			return (str_holder);
		}
	}
	else
	{
		ret_check_cgi = check_cgi(file_tmp, this->cgi_exec);
		if(ret_check_cgi != -1 && ret_check_cgi != -2)
		{
			free_double_tab(tmp);
			return (cgi_exec[ret_check_cgi].execute_cgi(tmp, file_tmp));
		}
	}
	if(ret_check_cgi == -2)
	{
		free_double_tab(tmp);
		return this->error_class.error_404();
	}
	else
	{
		Location temp_location = get_request_location(file);
		input.open(file_tmp.c_str());
		if ( (input.rdstate() & std::ifstream::failbit ) != 0 )
		{
			free_double_tab(tmp);
			return this->error_class.error_404();
		}
		if (o == 0 && temp_location.get_autoindex() == 1)
		{
			free_double_tab(tmp);
			return (autoindex(file_tmp, file));
		}
		else if (o == 0)
		{
			
			while (file_tmp != "error")
			{
				i++;
				input.close();
				file_tmp = this->get_location_path(file, i);
				input.open(file_tmp.c_str());
				if (input.good() == 1)
				{
					if(loc.get_cgi_exec().empty() == 0)
					{
						ret_check_cgi = check_cgi(file_tmp, loc.get_cgi_exec());
						if(ret_check_cgi != -1 && ret_check_cgi != -2)
						{
							free_double_tab(tmp);
							return (loc.get_cgi_exec()[ret_check_cgi].execute_cgi(file_tmp));
						}
					}
					else
					{
						ret_check_cgi = check_cgi(file_tmp, this->cgi_exec);
						if(ret_check_cgi != -1 && ret_check_cgi != -2)
						{
							free_double_tab(tmp);
							return (cgi_exec[ret_check_cgi].execute_cgi(file_tmp));
						}
					}
					if(ret_check_cgi == -2)
					{
						free_double_tab(tmp);
						return this->error_class.error_404();
					}
					Location bla;
					bla = get_request_location(file);
					if (bla.get_path().find("/dowload/") != std::string::npos)
						this->error_class.SetContentMsg("Content-Disposition: attachment\nContent-Type: text/html; charset=utf-8\n\n");
					buff << input.rdbuf();
					file_tmp = buff.str();
					free_double_tab(tmp);
					return (file_tmp);
				}
			}
			//std::cerr << "Fail to open file ["  << file_tmp << "]" << std::endl;
			free_double_tab(tmp);
			return this->error_class.error_404();
		}
		Location bla;
		bla = get_request_location(file);
		if (bla.get_path().find("/dowload/") != std::string::npos)
			this->error_class.SetContentMsg("Content-Disposition: attachment\nContent-Type: text/html; charset=utf-8\n\n");
		buff << input.rdbuf();
		file_tmp = buff.str();
		//std::cout << "file_tmp" << file_tmp << std::endl;
		free_double_tab(tmp);
		return (file_tmp);
	}
	return this->error_class.error_404();
}

std::string Server::actionDelete()
{
	char **tmp;
	char *tmp2;
	int i;
	std::string file;
	std::string file_tmp;
	Location loca;

	tmp = ft_split(this->msg_client.c_str(), " ");
	tmp2 = tmp[1];
	file += tmp2;
	loca = this->get_request_location(file); 
	if (loca.get_path().empty() == 0)
	{
		if (loca.get_delete() == 0 && this->info_serv.get_delete() == 0)
			return this->error_class.error_403();//ou 405
	}
	else
	{
		if (this->info_serv.get_delete() == 0)
			return this->error_class.error_403();//ou 405
	}
	file_tmp = loca.get_root();
	if (file_tmp.empty() == 1)
		file_tmp = this->info_serv.get_root();
	file_tmp += file;
	i = std::remove(file_tmp.c_str());
	if (i == 0)
	{
		return this->error_class.error_204();
	}
	return this->error_class.error_404();
}

void Server::close_all_fd()
{
	for(std::vector<Socket>::iterator it = all_socket.begin(); it != all_socket.end(); it++)
		close(it->getServerFd());
}

Location Server::get_request_location(std::string request)
{
	size_t i = 0;
	size_t tmp = 0;
	Location ret;
	std::string path_loca;
	std::string path_tmp;
	std::string request_tmp = request;
	std::vector<Location> *loca = this->info_serv.get_location_list();
	std::vector<Location>::iterator it = loca->begin();
	if (request_tmp[request_tmp.size() - 1] != '/')
		request_tmp += '/';
	while (it != loca->end())
	{
		i = 0;
		path_tmp = it->get_path();
		while (request_tmp[i] && path_tmp[i] && request_tmp[i] == path_tmp[i])
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
			return (*it);
		it++;
		i++;
	}
	return (ret);
}

int Server::verif_get_location(std::string file)
{
	size_t i = 0;
	size_t tmp = 0;
	std::string path_loca;
	std::string path_tmp;
	std::string file_tmp = file;
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
			if (it->get_get() == 1)
				return 0;
			else
				return 1;
		}
		it++;
	}
	return 0;
}

int Server::verif_post_location(std::string file)
{
	size_t i = 0;
	size_t tmp = 0;
	std::string path_loca;
	std::string path_tmp;
	std::string file_tmp = file;
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
			if (it->get_post() == 1)
				return 0;
			else
				return 1;
		}
		it++;
	}
	return 0;
}

std::vector<Cgi> Server::get_cgi_exec()
{
	return (cgi_exec);
}

int Server::verif_root(std::string file, std::string root)
{
	unsigned long i = 0;
	unsigned long j = 0;

	while (file[i] == '.' || file[i] == '/')
		i++;
	while (root[j] == '.' || root[j] == '/')
		j++;
	while (file[i] == root[j] && file[i] && root[j])
	{
		i++;
		j++;
	}
	if ((file.size() == i || file[i] == '/') && (root.size() == j || root[j] == '/'))
		return 1;
	return 0;
}

std::string Server::get_location_path(std::string file, int index)
{
	size_t i = 0;
	size_t tmp = 0;
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
			if (!it->get_root().empty())
			{
				if (verif_root(file, it->get_root()) == 0)
					ret += it->get_root();
				if (path_loca.size() - file.size() >= 0 && index != -1)
				{
					ret += path_loca;
					ret = this->add_index(ret, index, it);
					return ret;
				}
				if (verif_root(file, it->get_root()) == 1)
				{
					if (it->get_root()[0] == '.')
						ret = '.';
				}
				ret += file;
				return ret;
			}
			if (verif_root(file, this->info_serv.get_root()) == 0)
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
			if (verif_root(file, this->info_serv.get_root()) == 1)
			{
				if (this->info_serv.get_root()[0] == '.')
					ret = '.';
			}
			ret += file;
			return ret;
		}
		it++;
	}
	if (index != -1)
	{
		if (verif_root(file, this->info_serv.get_root()) == 0)
			ret = this->info_serv.get_root();
		ret += file;
		ret = this->add_index(ret, index, it);
		return ret;
	}
	if (verif_root(file, this->info_serv.get_root()) == 0)
		ret = this->info_serv.get_root();
	ret += file;
	return ret;
}

std::string Server::add_index(std::string ret, size_t index, std::vector<Location>::iterator it)
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