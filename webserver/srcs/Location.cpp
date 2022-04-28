#include "../include/Location.hpp"
#include "../include/Cgi.hpp"

Location::Location() : _get(0), _post(0), _delete(0), _autoindex(0), _root(),
						_path(), _index_list(), _redirect_list(), _upload_dir(), _cgi_list(), _cgi_exec(), _body_size(0)
{
	//std::cout << "Class Location default constructor" << std::endl;
}

Location::Location(Location const & copy)
{
	*this = copy;
	//std::cout << "Class Location copy constructor" << std::endl;
}

Location::~Location()
{
	//std::cout << "Class Location destructor" << std::endl;
}

Location & Location::operator=(Location const & copy)
{
	if (this != &copy)
	{
		this->_autoindex = copy._autoindex;
		this->_body_size = copy._body_size;
		this->_cgi_list = copy._cgi_list;
		this->_delete = copy._delete;
		this->_get = copy._get;
		this->_index_list = copy._index_list;
		this->_path = copy._path;
		this->_post = copy._post;
		this->_root = copy._root;
		this->_upload_dir = copy._upload_dir;
		this->_redirect_list = copy._redirect_list;
		this->_cgi_exec = copy._cgi_exec;
	}
	return(*this);
	//std::cout << "Class Location operator=" << std::endl;
}

void Location::parse_methods(std::string str)
{
	if(str.find("POST") != std::string::npos)
			_post = 1;
	if(str.find("GET") != std::string::npos)
			_get = 1;
	if(str.find("DELETE") != std::string::npos)
			_delete = 1;
}

void Location::parse_root(std::string str)
{
	int length = str.find("root");
	_root = &str[length + strlen("root") + 1];
	int i = 0; 
	while(_root[i] != ';')
		i++;
	_root.resize(i);
}

void Location::parse_autoindex(std::string str)
{
	if(str.find("on") != std::string::npos)
		_autoindex = 1;
}

void Location::parse_bodysize(std::string str)
{
	int length = str.find("client_max_body_size");
	std::string temp = &str[length + strlen("client_max_body_size") + 1];
	_body_size = atoi(temp.c_str());
}

void Location::parse_index_list(std::string str)
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

void Location::parse_path(std::string str)
{
	int length = str.find("location");
	_path = &str[length + strlen("location") + 1];
		int i = 0; 
	while(_path[i] != ' ')
		i++;
	_path.resize(i);
	if(_path[_path.size() - 1] != '/')
		_path.push_back('/');
}

void Location::parse_upload_dir(std::string str)
{
	int length = str.find("upload_dir");
	_upload_dir = &str[length + strlen("upload_dir") + 1];
	int i = 0; 
	while(_upload_dir[i] != ';')
		i++;
	_upload_dir.resize(i);
}
void Location::parse_cgi_list(std::string str)
{
	int length = str.find("cgi");
	std::string temp = &str[length + strlen("cgi") + 1];
	_cgi_list.push_back(temp);
}

void Location::parse_redirect_list(std::string str)
{
	int length = str.find("return");
	std::string temp = &str[length + strlen("return") + 1];
	while(temp[length] && (temp[length] >= '0' && temp[length] <= '9'))
		length++;
	int i = 0; 
	while(temp[i] != ';')
		i++;
	temp.resize(i);
	_redirect_list.insert(std::pair<int, std::string>(atoi(temp.c_str()), &temp[length + 1]));
}


void Location::parse_double_tab(std::vector<std::string> double_tab)
{
	for(size_t i = 0; i < double_tab.size(); i++)
	{
		if(double_tab[i].find(" location ") != std::string::npos || (double_tab[i].find("\tlocation ") != std::string::npos) || (double_tab[i].find("location ") != std::string::npos))
			parse_path(double_tab[i]);
		else if(double_tab[i].find(" cgi ") != std::string::npos || (double_tab[i].find("\tcgi ") != std::string::npos))
			parse_cgi_list(double_tab[i]);
		else if(double_tab[i].find(" return ") != std::string::npos || (double_tab[i].find("\treturn ") != std::string::npos))
			parse_redirect_list(double_tab[i]);
		else if(double_tab[i].find(" root ") != std::string::npos || (double_tab[i].find("\troot ") != std::string::npos))
			parse_root(double_tab[i]);
		else if(double_tab[i].find(" methods ") != std::string::npos || (double_tab[i].find("\tmethods ") != std::string::npos))
			parse_methods(double_tab[i]);
		else if(double_tab[i].find(" autoindex ") != std::string::npos || (double_tab[i].find("\tautoindex ") != std::string::npos))
			parse_autoindex(double_tab[i]);
		else if(double_tab[i].find(" client_max_body_size ") != std::string::npos || (double_tab[i].find("\tclient_max_body_size ") != std::string::npos))
			parse_bodysize(double_tab[i]);
		else if(double_tab[i].find(" index ") != std::string::npos || (double_tab[i].find("\tindex ") != std::string::npos))
			parse_index_list(double_tab[i]);
		else if(double_tab[i].find(" upload_dir ") != std::string::npos || (double_tab[i].find("\tupload_dir ") != std::string::npos))
			parse_upload_dir(double_tab[i]);
	}
}

void Location::parse_conf_file(std::string str)
{
	std::vector<std::string> double_tab = string_to_double_tab(str);
	parse_double_tab(double_tab);
}

void Location::ft_print_content(void)
{
	std::cout << "--Content of Location--" << std::endl;
	std::cout << "_PATH = " << _path << std::endl;
	std::cout << "_ROOT = " << _root << std::endl;
	std::cout << "_GET = " << _get << std::endl;
	std::cout << "_POST = " << _post << std::endl;
	std::cout << "_DELETE = " << _delete << std::endl;
	std::cout << "_BODY_SIZE = " << _body_size << std::endl;
	std::cout << "_UPLOAD_DIR = " << _upload_dir << std::endl;
	std::cout << "_INDEX_LIST = " << std::endl;
	for(std::vector<std::string>::iterator it = _index_list.begin(); it != _index_list.end(); it++)
		std::cout << *it << std::endl;
	std::cout << "_REDIRECT_LIST = "<< std::endl;
	for(std::map<int, std::string>::iterator it = _redirect_list.begin(); it != _redirect_list.end(); it++)
		std::cout << it->first << " " << it->second << std::endl;
	std::cout << "_CGI_LIST = " << std::endl;
	for(std::vector<std::string>::iterator it = _cgi_list.begin(); it != _cgi_list.end(); it++)
		std::cout << *it <<std::endl;	
	std::cout << "_AUTOINDEX = " << _autoindex << std::endl << std::endl;
}

void Location::set_cgi_exec(std::vector<Cgi> value)
{
	_cgi_exec = value;
}

void Location::set_redirect_list(std::map<int, std::string> value)
{
	_redirect_list = value;
}

void Location::set_get(bool value)
{
	_get = value;
}

void Location::set_cgi_list(std::vector<std::string> value)
{
	_cgi_list = value;
}

void Location::set_post(bool value)
{
	_post = value;
}
void Location::set_delete(bool value)
{
	_delete = value;
}
void Location::set_autoindex(bool value)
{
	_autoindex = value;
}
void Location::set_root(std::string value)
{
	_root = value;
}
void Location::set_path(std::string value)
{
	_path = value;
}
void Location::set_index_list(std::vector<std::string> value)
{
	_index_list = value;
}
void Location::set_upload_dir(std::string value)
{
	_upload_dir = value;
}
void Location::set_body_size(unsigned long value)
{
	_body_size = value;
}

std::vector<std::string> Location::get_cgi_list() const
{
	return(_cgi_list);
}

std::map<int, std::string> Location::get_redirect_list() const
{
	return(_redirect_list);
}

std::vector<Cgi> Location::get_cgi_exec() const
{
	return(_cgi_exec);
}

bool Location::get_get() const
{
	return (_get);
}
bool Location::get_post() const
{
	return (_post);
}
bool Location::get_delete() const
{
	return (_delete);
}
bool Location::get_autoindex() const
{
	return (_autoindex);
}
std::string Location::get_root() const
{
	return (_root);
}
std::string Location::get_path() const
{
	return (_path);
}
std::string Location::get_upload_dir() const
{
	return (_upload_dir);
}
std::vector<std::string> Location::get_index_list() const
{
	return (_index_list);
}
unsigned long Location::get_body_size() const
{
	return (_body_size);
}