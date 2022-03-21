#pragma once

#include <iostream>
#include <vector>
#include "header.hpp"


class Socket
{
	public:
		Socket();
		Socket(std::string port, std::string ip);
		Socket(Socket const & other);
		~Socket();
		Socket &operator=(Socket const & other);
		void set_port(std::string port);
		void set_ip(std::string ip);
		int create_socket();
		int make_bind();
		int listen_socket(int max_queue);
		int getServerFd();
		sockaddr_in getServerAddress();
	private:
		sockaddr_in server_address;
		int server_fd;
		std::string port;
		std::string ip;
};