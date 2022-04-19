#include "../include/Conf.hpp"

Conf::Conf() : _list_virtual_server(), file_content()
{
	//std::cout << "Class Conf default constructor" << std::endl;
}


Conf::Conf(std::string filename) : _list_virtual_server(), file_content()
{
	//std::cout << "Class Conf default constructor" << std::endl;
	parse_conf_file(filename);
}

Conf::Conf(Conf const & copy)
{
	*this = copy;
	//std::cout << "Class Conf copy constructor" << std::endl;
}

Conf::~Conf()
{
	delete _list_virtual_server;
	//std::cout << "Class Conf destructor" << std::endl;
}

Conf & Conf::operator=(Conf const & copy)
{
	if(this != &copy)
		*this = copy;
	return(*this);
	//std::cout << "Class Conf operator=" << std::endl;
}

std::vector<VirtualServer> * Conf::get_list_virtual_server() const
{
	return (_list_virtual_server);
}

void Conf::add_virtual_server(VirtualServer const & server)
{
	_list_virtual_server->push_back(server);
}

void Conf::set_virtual_server(std::vector<VirtualServer> * server_list)
{
	_list_virtual_server = server_list;
}

std::string Conf::get_file_content() const
{
	return(file_content);
}

std::vector<Server> Conf::create_all_server(char **envp)
{
	std::vector<Server> list_server(_list_virtual_server->size());
	int i = 0;
	int j;
	std::vector<Cgi> cgi_list;
	std::vector<std::string> list;
	for(std::vector<VirtualServer>::iterator it = _list_virtual_server->begin(); it != _list_virtual_server->end(); it++)
	{
		list_server[i].set_virtual_server(*it);
		//list_server[i].get_error_class().SetErrorPage(list_server[i].get_info_serv().get_error_page());
		cgi_list.clear();
		cgi_list.resize(it->get_cgi_list().size());
		j = 0;
		list = it->get_cgi_list();
		for(std::vector<std::string>::iterator it2 = list.begin();  it2 != list.end(); it2++)
			cgi_list[j++].setup(envp, *it2);
		list_server[i].set_cgi(cgi_list);
		if(list_server[i].create_socket())
		{
			std::cerr << "Error with socket creation." << std::endl;
			exit(1);
		}
		//list_server[i].get_error_class().SetErrorPage(list_server[i].get_info_serv().get_error_page());
		i++;
	}
	return(list_server);
}

void Conf::parse_conf_file(std::string filename)
{
	std::cout << "Start parsing of file: " << filename << std::endl;
	std::ifstream read_stream(filename);
	std::stringstream buffer;
	std::string new_str;
	std::string content;
	if(read_stream.good() == 0)
	{
		std::cerr << "Error, file is not valid." << std::endl;
		return ;
	}
	buffer << read_stream.rdbuf();
	content = buffer.str();
	this->file_content = content;
	size_t found;
	size_t server_nbr = count_appearance(content, "server");
	_list_virtual_server = new std::vector<VirtualServer>(server_nbr);
	std::vector<Location> * temp;
	for(size_t i = 0; i < server_nbr; i++)
	{
		found = content.find("server");
		if(found == std::string::npos)
			break;
		new_str = content;
		cutblock(new_str);
		(*_list_virtual_server)[i].parse_conf_file(new_str);
		temp = (*_list_virtual_server)[i].get_location_list();
		content = &content[walk_end_block(&content[found])];
	}
}

void Conf::ft_print_content(void)
{
	std::cout << "--Content of conf file--" << std::endl;
	for(std::vector<VirtualServer>::iterator it = _list_virtual_server->begin(); it != _list_virtual_server->end(); it++)
		it->ft_print_content();
}
