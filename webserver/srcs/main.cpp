#include "../include/header.hpp"

int main(int ac, char **av)
{
	if(ac != 2)
	{
		std::cerr << "usage: ./server [PORT]" << std::endl;
		return (1);
	}

	Conf conf(av[1]);
	std::vector<Server> all_server = conf.create_all_server();
	while (42)
	{
		for(std::vector<Server>::iterator it = all_server.begin(); it != all_server.end(); it++)
		{
			it->server_accept();
			if (it->receive_msg() != 0)
				break;
			if (it->send_msg() != 0)
				return (1);
		}
	}
	std::cout << "Close server" << std::endl;
	return (0);
}