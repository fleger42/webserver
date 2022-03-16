#include "../include/header.hpp"

int main(int ac, char **av)
{
	if(ac != 2)
	{
		std::cerr << "usage: ./server [PORT]" << std::endl;
		return (1);
	}

	Server server;
	Socket socket;
	Conf conf(av[1]);
	socket.create_socket(av[1]);
	if (socket.find_and_bind(av[1]) != 0)
		return (1);
	if (socket.listen_socket(av[1], 3) < 0)
		return (1);

	int i = 1;
	while (42)
	{
		while (av[i])
		{
			server.server_accept(socket.getServerFd(av[i]));
			i++;
		}
		if (server.receive_msg() != 0)
			break;
		if (server.send_msg() != 0)
			return (1);
		close(server.getClient());
		i = 1;
	}
	std::cout << "Close server" << std::endl;
	close(socket.getServerFd(av[1]));
	return (0);
}