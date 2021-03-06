#ifndef LOCATION_HPP
#define LOCATION_HPP
#include <iostream>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <map>
size_t				ft_strlcpy(char *dst, char const *src, size_t size);
char				**ft_clear_splitted(char **tab);
char				**ft_split(char const *s, const char *delimiters);
std::vector<std::string> string_to_double_tab(std::string str);
/*
Classe qui contient les infos d'un bloc location du fichier de configuration
*/
class Cgi;
class Location
{
	public:
		Location();
		Location(Location const & copy);
		~Location();
		Location & operator=(Location const & copy);

		void ft_print_content(void);

		void parse_conf_file(std::string str);
		void parse_methods(std::string str);
		void parse_root(std::string str);
		void parse_autoindex(std::string str);
		void parse_bodysize(std::string str);
		void parse_index_list(std::string str);
		void parse_double_tab(std::vector<std::string> double_tab);
		void parse_path(std::string str);
		void parse_upload_dir(std::string str);
		void parse_cgi_list(std::string str);
		void parse_redirect_list(std::string str);

		void set_cgi_list(std::vector<std::string> value);
		void set_cgi_exec(std::vector<Cgi> value);
		void set_get(bool value);
		void set_post(bool value);
		void set_delete(bool value);
		void set_autoindex(bool value);
		void set_root(std::string value);
		void set_path(std::string value);
		void set_index_list(std::vector<std::string> value);
		void set_upload_dir(std::string value);
		void set_body_size(unsigned long value);
		void set_redirect_list(std::map<int, std::string> value);

		std::map<int, std::string> get_redirect_list() const;
		std::vector<std::string> get_cgi_list() const;
		std::vector<Cgi> get_cgi_exec() const;
		bool get_get() const;
		bool get_post() const;
		bool get_delete() const;
		bool get_autoindex() const;
		std::string get_root() const;
		std::string get_path() const;
		std::string get_upload_dir() const;
		std::vector<std::string> get_index_list() const;
		unsigned long get_body_size() const;
	private:
		bool _get;
		bool _post;
		bool _delete;
		bool _autoindex;
		std::string _root;
		std::string _path;
		std::vector<std::string> _index_list;
		std::map<int, std::string> _redirect_list;
		std::string _upload_dir;
		std::vector<std::string> _cgi_list;
		std::vector<Cgi> _cgi_exec;
		unsigned long _body_size;
};
#endif