#include "../include/VirtualServer.hpp"

VirtualServer::VirtualServer() : _root(), _ip(), _server_name(), _port(), _error_page(),_body_size(0), 
								_autoindex(0), _get(0), _post(0), _delete(0), _location_list()
{
	//std::cout << "Class VirtualServer default constructor" << std::endl;
}

VirtualServer::VirtualServer(VirtualServer const & copy)
{
	*this = copy;
	//std::cout << "Class VirtualServer copy constructor" << std::endl;
}

VirtualServer::~VirtualServer()
{
	delete _location_list;
	//std::cout << "Class VirtualServer destructor" << std::endl;
}

VirtualServer & VirtualServer::operator=(VirtualServer const & copy)
{
	//std::cout << "Class VirtualServer operator=" << std::endl;
	if(this != &copy)
	{
		this->_root = copy._root;
		this->_ip = copy._ip;
		this->_server_name = copy._server_name;
		this->_port = copy._port;
		this->_error_page = copy._error_page;
		this->_body_size = copy._body_size;
		this->_autoindex = copy._autoindex;
		this->_index_list = copy._index_list;
		this->_get = copy._get;
		this->_post = copy._post;
		this->_delete = copy._delete;
		this->_location_list = copy._location_list;	
	}
	return(*this);
}

void VirtualServer::parse_methods(std::string str)
{
	if(str.find("POST") != std::string::npos)
			_post = 1;
	if(str.find("GET") != std::string::npos)
			_get = 1;
	if(str.find("DELETE") != std::string::npos)
			_delete = 1;
}

void VirtualServer::parse_ip(std::string str)
{
	int i = 0;
	int length = 0;
	while(str[i] && !(str[i] >= '0' && str[i] <= '9'))
		i++;
	if(str[i] >= '0' && str[i] <= '9')
	{
		length = i;
		while(str[i] && str[i] != ':')
			i++;
		_ip.push_back(&str[length]);
		_ip[_ip.size() - 1].resize(i - length);
	}
	if(str[i] == ':')
	{
		str[ft_strlen(str.c_str()) - 1] = '\0';
		_port.push_back(&str[i + 1]);
		
	}
}

void VirtualServer::parse_root(std::string str)
{
	int length = str.find("root");
	_root = &str[length + strlen("root") + 1];
	int i = 0; 
	while(_root[i] != ';')
		i++;
	_root.resize(i);
}

void VirtualServer::parse_server_name(std::string str)
{
	int length = str.find("server_name");
	_server_name = &str[length + strlen("server_name") + 1];
	int i = 0; 
	while(_server_name[i] != ';')
		i++;
	_server_name.resize(i);
}

void VirtualServer::parse_autoindex(std::string str)
{
	if(str.find("on") != std::string::npos)
		_autoindex = 1;
}

void VirtualServer::parse_bodysize(std::string str)
{
	int length = str.find("client_max_body_size");
	std::string temp = &str[length + strlen("client_max_body_size") + 1];
	_body_size = atoi(temp.c_str());
}

void VirtualServer::parse_error_page(std::string str)
{
	int length = str.find("error_page");
	std::string temp = &str[length + strlen("error_page") + 1];
	while(temp[length] && (temp[length] >= '0' && temp[length] <= '9'))
		length++;
	int i = 0; 
	while(temp[i] != ';')
		i++;
	temp.resize(i);
	_error_page.insert(std::pair<int, std::string>(atoi(temp.c_str()), &temp[length + 1]));
}


void VirtualServer::parse_index_list(std::string str)
{
	int i = str.find("index");
	std::string s = &str[i + strlen("index")];
	
	char **ret = ft_split(s.c_str(), " ");
	i = 0;
	while(ret[i])
	{
		_index_list.push_back(ret[i]);
		free(ret[i]);
		i++;
	}
	free(ret);
	for(std::vector<std::string>::iterator it = _index_list.begin(); it != _index_list.end(); it++)
	{
		i = 0; 
		while((*it)[i] != ';')
			i++;
		it->resize(i);
	}
}

void VirtualServer::parse_double_tab(std::vector<std::string> double_tab)
{
	int length = 0;
	for(int i = 0; i < double_tab.size(); i++)
	{
		if((length = double_tab[i].find("location")) != std::string::npos)
		{
			while((length = double_tab[i].find("}")) == std::string::npos)
				i++;
		}
		else if((length = double_tab[i].find("listen")) != std::string::npos)
			parse_ip(double_tab[i]);
		else if((length = double_tab[i].find("root")) != std::string::npos)
			parse_root(double_tab[i]);
		else if((length = double_tab[i].find("server_name")) != std::string::npos)
			parse_server_name(double_tab[i]);
		else if((length = double_tab[i].find("methods")) != std::string::npos)
			parse_methods(double_tab[i]);
		else if((length = double_tab[i].find("autoindex")) != std::string::npos)
			parse_autoindex(double_tab[i]);
		else if((length = double_tab[i].find("error_page")) != std::string::npos)
			parse_error_page(double_tab[i]);
		else if((length = double_tab[i].find("client_max_body_size")) != std::string::npos)
			parse_bodysize(double_tab[i]);
		else if((length = double_tab[i].find("index")) != std::string::npos)
			parse_index_list(double_tab[i]);
	}
}

void VirtualServer::parse_conf_file(std::string str)
{
	size_t found;
	std::string new_str;

	std::vector<std::string> double_tab = string_to_double_tab(str);
	parse_double_tab(double_tab);
	size_t location_nbr = count_appearance(str, "location");
	_location_list = new std::vector<Location>(location_nbr);
	for(size_t i = 0; i < location_nbr; i++)
	{
		found = str.find("location");
		if(found == std::string::npos)
			break;
		new_str = &str[found];
		cutblock(new_str);
		(*_location_list)[i].parse_conf_file(new_str);
		str = &str[walk_end_block(&str[found])];
	}
}

void VirtualServer::ft_print_content(void)
{
	std::cout << "--Content of VirtualServer--" << std::endl;
	std::cout << "_SERVER_NAME = " << _server_name << std::endl;
	std::cout << "_IP = " << std::endl;
	for(std::vector<std::string>::iterator it = _ip.begin(); it != _ip.end(); it++)
		std::cout << *it << std::endl;
	std::cout << "_PORT = " << std::endl;
	for(std::vector<std::string>::iterator it = _port.begin(); it != _port.end(); it++)
		std::cout << *it << std::endl;
	std::cout << "_ROOT = " << _root << std::endl;
	std::cout << "_GET = " << _get << std::endl;
	std::cout << "_POST = " << _post << std::endl;
	std::cout << "_DELETE = " << _delete << std::endl;
	std::cout << "_BODY_SIZE = " << _body_size << std::endl;
	std::cout << "_ERROR_PAGE = " << std::endl;
	for(std::map<int, std::string>::iterator it = _error_page.begin(); it != _error_page.end(); it++)
		std::cout << it->first <<  " " << it->second << std::endl;
	std::cout << "_INDEX_LIST = " << std::endl;
	for(std::vector<std::string>::iterator it = _index_list.begin(); it != _index_list.end(); it++)
		std::cout << *it << std::endl;
	std::cout << "_AUTOINDEX = " << _autoindex << std::endl << std::endl;
	for(std::vector<Location>::iterator it = _location_list->begin(); it != _location_list->end(); it++)
		it->ft_print_content();

}

void VirtualServer::set_index_list(std::vector<std::string> value)
{
	_index_list = value;
}

void VirtualServer::set_get(bool value)
{
	_get = value;
}

void VirtualServer::set_post(bool value)
{
	_post = value;
}

void VirtualServer::set_delete(bool value)
{
	_delete = value;
}

void VirtualServer::set_autoindex(bool value)
{
	_autoindex = value;
}

void VirtualServer::set_root(std::string value)
{
	_root = value;
}

void VirtualServer::set_location_list(std::vector<Location> * value)
{
	_location_list = value;
}

void VirtualServer::set_body_size(unsigned long value)
{
	_body_size = value;
}

void VirtualServer::set_ip(std::vector<std::string> value)
{
	_ip = value;
}

void VirtualServer::set_server_name(std::string value)
{
	_server_name = value;
}

void VirtualServer::set_port(std::vector<std::string> value)
{
	_port = value;
}

void VirtualServer::set_error_page(std::map<int, std::string> value)
{
	_error_page = value;
}

std::vector<std::string> VirtualServer::get_index_list() const
{
	return(_index_list);
}

std::map<int, std::string> VirtualServer::get_error_page() const
{
	return(_error_page);
}

bool VirtualServer::get_get() const
{
	return(_get);
}

bool VirtualServer::get_post() const
{
	return(_post);
}

bool VirtualServer::get_delete() const
{
	return(_delete);
}

bool VirtualServer::get_autoindex() const
{
	return(_autoindex);
}

std::vector<std::string> VirtualServer::get_port() const
{
	return(_port);
}

std::string VirtualServer::get_root() const
{
	return(_root);
}

std::vector<std::string> VirtualServer::get_ip() const
{
	return(_ip);
}

std::string VirtualServer::get_server_name() const
{
	return(_server_name);
}

std::vector<Location> * VirtualServer::get_location_list()
{
	return(_location_list);
}

unsigned long VirtualServer::get_body_size() const
{
	return(_body_size);
}
