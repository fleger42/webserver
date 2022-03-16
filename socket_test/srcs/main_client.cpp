#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include "../include/get_next_line.hpp"
#include <sys/types.h>
#include <sys/wait.h>

int main(int ac, char *av[])
{
	if(ac != 3)
	{
		std::cerr << "usage: ./mini_client [IP] [PORT]" << std::endl;
		exit(1);
	}
	struct sockaddr_in server_address;
	memset((char*)&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(av[1]);
	server_address.sin_port = htons(std::atoi(av[2]));
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

	if(client_fd < 0)
		std::cerr << "Error with socked creation" << std::endl;
	if(connect(client_fd, (struct sockaddr*)&server_address, (socklen_t)sizeof(server_address)) < 0)
	{
		std::cerr << "Error while connecting socket" << std::endl;
		close(client_fd);
		exit(1);	
	}
	char send_buff[] = "GET / HTTP/1.1\r\n\r\n";
	if(send(client_fd, send_buff, strlen(send_buff), 0) < 0)
	{
		std::cerr << "error: send()" <<std::endl;
		exit(1);
	}
	while(42)
	{
		char buff[1000];
		int n;
		if((n = recv(client_fd, buff, sizeof(buff) - 1, 0)) < 0 || n == 0)
			break;
		buff[n] = '\0';
		std::cout << "Messaged received:" << buff << std::endl;
	}
	/*int status = 0;
	char *buf;
	while ((status = get_next_line(client_fd, &buf)) > 0)
	{
		std::cout << buf << std::endl;
		std::cout << "IN GNL" << std::endl;
		free(buf);
	}
	std::cout << buf << std::endl;
	free(buf);
	if (status == -1)
	{
		std::cerr << "error: recv()"<< std::endl;
		exit(1);
	}*/
	close(client_fd);
	return (0);
}
