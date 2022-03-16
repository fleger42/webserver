#pragma once

#include <iostream>
#include <vector>
#include "header.hpp"


class Socket
{

	public:

	Socket();
	Socket(Socket const & other);
	~Socket();
	Socket &operator=(Socket const & other);

	void create_socket(char *port);
	int find_and_bind(char *port);
	int listen_socket(char *port, int connec);

	int getServerFd(int n);
	int getServerFd(char *port);
	std::vector<int> getServerFd();

	sockaddr_in getServerAddress(int n);
	sockaddr_in getServerAddress(char *port);
	std::vector<sockaddr_in> getServerAddress();


	private:

	std::vector<sockaddr_in> server_address;
	std::vector<int> server_fd;


};