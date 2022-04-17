#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <istream>
#include <vector>
#include "header.hpp"
#include <errno.h>
#include <fstream>
#include "VirtualServer.hpp"
#include "Cgi.hpp"

class VirtualServer;
std::vector<std::string> string_to_double_tab(std::string str);
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
			int create_socket();
			std::string actionGet();
			std::string actionPost();
			std::string actionDelete();
			int get_action();
			void close_all_fd();
			int verif_post_location(std::string file);
			int verif_get_location(std::string file);
			int	check_cgi(std::string uri);
			std::string add_index(std::string ret, size_t index, std::vector<Location>::iterator it);
			
			void set_virtual_server(VirtualServer const & value);
			void set_cgi(std::vector<Cgi> cgi_exec);
			
			int getClient();
			std::vector<Socket> get_all_socket() const;
			std::string get_server_name();
			std::string get_location_path(std::string file, int index);
			VirtualServer get_info_serv();
			Location get_request_location(std::string request);
			std::string get_cgi_path();
			std::vector<Cgi> get_cgi_exec();
		
		private:
			int client;
			char *msg_client;
			std::vector<Socket> all_socket;
			VirtualServer info_serv;
			std::vector<Cgi> cgi_exec;

};
#endif