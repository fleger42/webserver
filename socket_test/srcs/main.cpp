#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>

int main(int ac, char *av[])
{
	if(ac != 2)
	{
		std::cerr << "usage: ./mini_client [PORT]" << std::endl;
		exit(1);
	}
	struct sockaddr_in server_address;
	memset((char*)&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(std::atoi(av[1]));
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd < 0)
		std::cerr << "Error with socked creation" << std::endl;
	if(bind(server_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
	{
		std::cerr << "Error with biding" << std::endl;
		close(server_fd);
		exit(1);	
	}
	if(listen(server_fd, 3) < 0)
	{
		std::cerr << "Error while listening" << std::endl;
		close(server_fd);
		exit(1);	
	}
	struct sockaddr client_address;
	socklen_t client_addr_lenght;
	int new_client;
	while(42)
	{
		if((new_client = accept(server_fd, (struct sockaddr*)&client_address, (socklen_t*)&client_addr_lenght)) < 0)
		{
			std::cerr << "Error while accepting new client" << std::endl;
			
			close(server_fd);
			exit(1);
		}
		std::cout << "New client connected on server: " << std::endl;
		char buff[1000];
		int n;
		if((n = recv(new_client, buff, sizeof(buff) - 1, 0)) < 0 || n == 0)
			break;
		buff[n] = '\0';
		std::cout << "Messaged received:\n" << buff << std::endl;
		
		char send_buff[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
		if(send(new_client, send_buff, strlen(send_buff), 0) < 0)
		{
			std::cerr << "error: send()" <<std::endl;
			exit(1);
		}
		close(new_client);
	}
	std::cout << "Close server" << std::endl;
	close(server_fd);
	return (0);
}