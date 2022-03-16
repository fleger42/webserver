#pragma once

#include <iostream>
#include <vector>
#include "header.hpp"

std::vector<std::string> string_to_double_tab(std::string str);
static unsigned int	ft_get_nb_strs(char const *s, const char *delimiters);
static void			ft_get_next_str(char **next_str, unsigned int *next_str_len, const char *delimiters);
size_t				ft_strlcpy(char *dst, char const *src, size_t size);
char				**ft_clear_splitted(char **tab);
char				**ft_split(char const *s, const char *delimiters);
int 				ft_strlen(char *str);
int 				ft_strlen(const char *str);

class Server {

		public:

		Server();
		Server(Server const & other);
		Server(VirtualServer const & virtual_serv);
		~Server();
		Server &operator=(Server const & other);

		int server_accept(int socket_fd);
		int receive_msg();
		int send_msg();
		int get_action();

		std::string actionGet();
		void actionPost();
		void actionDelete();

		int getClient();


		private:

		int client;
		char *msg_client;
		std::vector<Socket> list_socket;
		VirtualServer info_serv;

};