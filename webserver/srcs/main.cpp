#include "../include/header.hpp"

int main(int ac, char **av)
{
	if(ac != 2)
	{
		std::cerr << "usage: ./server [PORT]" << std::endl;
		return (1);
	}

	Socket socket("8080", "0.0.0.0");
	Server server;
	Conf conf(av[1]);
	socket.create_socket();
	if (socket.make_bind() != 0)
		return (1);
	if (socket.listen_socket(3) < 0)
		return (1);

	int i = 1;
	while (42)
	{
		while (av[i])
		{
			server.server_accept(socket.getServerFd());
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
	close(socket.getServerFd());
	return (0);
}