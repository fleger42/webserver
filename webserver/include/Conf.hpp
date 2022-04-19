#ifndef CONF_HPP
#define CONF_HPP
#include <iostream>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string.h>


class Cgi;
class VirtualServer;
class Server;
void cutblock(std::string & str);
int count_appearance(std::string file, std::string str);
int walk_end_block(std::string const & str);
/*
Classe qui contient la totalité des informations du fichier de configuration après parsing
*/
class Conf
{
	public:
		Conf();
		Conf(std::string filename);
		Conf(Conf const & copy);
		~Conf();
		Conf & operator=(Conf const & copy);
		std::vector<VirtualServer> * get_list_virtual_server() const;
		void set_virtual_server(std::vector<VirtualServer> * server_list);
		void add_virtual_server(VirtualServer const & server);
		void parse_conf_file(std::string filename);
		std::string get_file_content() const;
		std::vector<Server> create_all_server(char **envp);
		void ft_print_content(void);
	private:
		std::vector<VirtualServer> * _list_virtual_server;
		std::string file_content;
};
#endif