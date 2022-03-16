#ifndef LOCATION_HPP
#define LOCATION_HPP
#include <iostream>
#include <unistd.h>
#include <vector>
#include <string.h>
static unsigned int	ft_get_nb_strs(char const *s, const char *delimiters);
static void			ft_get_next_str(char **next_str, unsigned int *next_str_len,
					const char *delimiters);
size_t				ft_strlcpy(char *dst, char const *src, size_t size);
char				**ft_clear_splitted(char **tab);
char				**ft_split(char const *s, const char *delimiters);
std::vector<std::string> string_to_double_tab(std::string str);
/*
Classe qui contient les infos d'un bloc location du fichier de configuration
*/
class Location
{
	public:
		Location();
		Location(Location const & copy);
		~Location();
		Location & operator=(Location const & copy);

		void parse_conf_file(std::string str);
		void parse_methods(std::string str);
		void parse_root(std::string str);
		void parse_autoindex(std::string str);
		void parse_bodysize(std::string str);
		void parse_index_list(std::string str);
		void parse_double_tab(std::vector<std::string> double_tab);
		void parse_path(std::string str);
		void parse_upload_dir(std::string str);

		void set_get(bool value);
		void set_post(bool value);
		void set_delete(bool value);
		void set_autoindex(bool value);
		void set_root(std::string value);
		void set_path(std::string value);
		void set_index_list(std::vector<std::string> value);
		void set_upload_dir(std::string value);
		void set_body_size(unsigned long value);

		bool get_get();
		bool get_post();
		bool get_delete();
		bool get_autoindex();
		std::string get_root();
		std::string get_path();
		std::string get_upload_dir();
		std::vector<std::string> get_index_list();
		unsigned long get_body_size();
	private:
		bool _get;
		bool _post;
		bool _delete;
		bool _autoindex;
		std::string _root;
		std::string _path;
		std::vector<std::string> _index_list;
		std::string _upload_dir;
		unsigned long _body_size;
};
#endif