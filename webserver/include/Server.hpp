#pragma once

#include <iostream>
#include <vector>
#include "header.hpp"

class Server {

		public:

		Server();
		Server(Server const & other);
		~Server();
		Server &operator=(Server const & other);

		int server_accept(int socket_fd);
		int receive_msg();
		int send_msg();

		int getClient();


		private:

		int client;
		char *msg_client;

};