#ifndef VIRTUALSERVER_HPP
#define VIRTUALSERVER_HPP
#include <iostream>
#include <unistd.h>
#include <map>
#include "Location.hpp"
#include <fstream>
#include <sstream>
#include <string.h>
/*
Classe qui contient les infos d'un bloc server du fichier de configuration
*/
int count_appearance(std::string file, std::string str);
void cutblock(std::string & str);
int walk_end_block(std::string const & str);
std::vector<std::string> string_to_double_tab(std::string str);
static unsigned int	ft_get_nb_strs(char const *s, const char *delimiters);
static void			ft_get_next_str(char **next_str, unsigned int *next_str_len,
					const char *delimiters);
size_t				ft_strlcpy(char *dst, char const *src, size_t size);
char				**ft_clear_splitted(char **tab);
char				**ft_split(char const *s, const char *delimiters);
int ft_strlen(char *str);
int ft_strlen(const char *str);
class VirtualServer
{
	public:
		VirtualServer();
		VirtualServer(VirtualServer const & copy);
		~VirtualServer();
		VirtualServer & operator=(VirtualServer const & copy);
		void parse_conf_file(std::string str);
		void parse_double_tab(std::vector<std::string> double_tab);
		void parse_methods(std::string str);
		void parse_root(std::string str);
		void parse_ip(std::string str);
		void parse_server_name(std::string str);
		void parse_autoindex(std::string str);
		void parse_bodysize(std::string str);
		void parse_error_page(std::string str);
		void parse_index_list(std::string str);

		void set_get(bool value);
		void set_post(bool value);
		void set_delete(bool value);
		void set_autoindex(bool value);
		void set_root(std::string value);
		void set_location_list(std::vector<Location> * value);
		void set_body_size(unsigned long value);
		void set_ip(std::vector<std::string> value);
		void set_server_name(std::string value);
		void set_port(std::vector<std::string> value);
		void set_error_page(std::map<int, std::string> value);
		void set_index_list(std::vector<std::string> value);

		std::map<int, std::string> get_error_page() const;
		bool get_get() const;
		bool get_post() const;
		bool get_delete() const;
		bool get_autoindex() const;
		std::vector<std::string> get_port() const;
		std::string get_root() const;
		std::vector<std::string> get_ip() const;
		std::string get_server_name() const;
		std::vector<Location> * get_location_list();
		std::vector<std::string> get_index_list() const;
		unsigned long get_body_size() const;
		void ft_print_content(void);
	private:
		std::string _root;
		std::string _server_name;
		std::vector<std::string> _ip;
		std::vector<std::string> _port;
		std::vector<std::string> _index_list;
		std::map<int, std::string> _error_page;
		unsigned long _body_size;
		bool _autoindex;
		bool _get;
		bool _post;
		bool _delete;
		std::vector<Location> * _location_list;
};
#endif