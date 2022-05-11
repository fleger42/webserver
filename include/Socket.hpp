#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

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
		void close_fd();
	private:
		sockaddr_in server_address;
		int server_fd;
		std::string port;
		std::string ip;
};
#endif