#pragma once

#include <iostream>
#include <istream>
#include <vector>
#include "header.hpp"
#include <errno.h>
#include <fstream>
#include "VirtualServer.hpp"

class VirtualServer;
std::vector<std::string> string_to_double_tab(std::string str);
static unsigned int	ft_get_nb_strs(char const *s, const char *delimiters);
static void			ft_get_next_str(char **next_str, unsigned int *next_str_len, const char *delimiters);
size_t				ft_strlcpy(char *dst, char const *src, size_t size);
char				**ft_clear_splitted(char **tab);
char				**ft_split(char const *s, const char *delimiters);
int 				ft_strlen(char *str);
int 				ft_strlen(const char *str);

class Socket;
class Server {

		public:
			Server(Server const & other);
			Server(VirtualServer const & virtual_serv);
			Server();
			~Server();
			Server &operator=(Server const & other);

			int server_accept();
			int receive_msg();
			int send_msg();
			int get_action();
			int create_socket();
			std::string actionGet();
			void actionPost();
			void actionDelete();
			void set_virtual_server(VirtualServer const & value);
			int getClient();
			void close_all_fd();
			std::vector<Socket> get_all_socket() const;
			std::string get_server_name();
			int verif_get_location(std::string file);
			std::string get_location_path(std::string file, int index);
			VirtualServer get_info_serv();
			std::string add_index(std::string ret, int index, std::vector<Location>::iterator it);

		private:
			int client;
			char *msg_client;
			std::vector<Socket> all_socket;
			VirtualServer info_serv;

};