#include "../include/header.hpp"

int signal_nbr = SIGINT;

void	ft_signal_handler(int signal)
{
	if (signal == SIGINT)
	{
			write(STDERR_FILENO, "\b\b  ", 4);
			write(STDERR_FILENO, "\n", 1);
			g_ctrl_c_called = 1;
	}
}

void routine(std::vector<Server> & all_server)
{
	fd_set select_set_read_dump;
	fd_set select_set_read_ready;
	struct timeval select_timeout;
	select_timeout.tv_sec = 1;
	select_timeout.tv_usec = 0;
	FD_ZERO(&select_set_read_dump);
	int status = 0;
	int max_fd = 0;
	for(std::vector<Server>::iterator it_server = all_server.begin(); it_server != all_server.end(); it_server++)
	{
		std::vector<Socket> temp =  it_server->get_all_socket();
		for(std::vector<Socket>::iterator it_socket = temp.begin(); it_socket != temp.end(); it_socket++)
		{
			FD_SET(it_socket->getServerFd(), &select_set_read_dump);
			if(it_socket->getServerFd() > max_fd)
				max_fd = it_socket->getServerFd();
		}
	}
	signal(signal_nbr, ft_signal_handler);
	while (g_ctrl_c_called == 0)
	{
		while(status == 0 && g_ctrl_c_called == 0)
		{
			FD_ZERO(&select_set_read_ready);
			select_set_read_ready = select_set_read_dump;
			usleep(2000);
			if((status = select(max_fd + 1, &select_set_read_ready, NULL, NULL, &select_timeout)) < 0)
			{
				if(g_ctrl_c_called == 1)
				{
					std::cerr << "Closing webserver..." << std::endl;
					exit(0);
				}
				std::cerr << "Error: select()" << std::endl;
				exit(1);
			}
		}
		if(status > 0 && g_ctrl_c_called == 0)
		{
			for(std::vector<Server>::iterator it_server = all_server.begin(); it_server != all_server.end(); it_server++)
			{
				std::vector<Socket> temp =  it_server->get_all_socket();
				for(std::vector<Socket>::iterator it_socket = temp.begin(); it_socket != temp.end(); it_socket++)
				{
					if(FD_ISSET(it_socket->getServerFd(), &select_set_read_ready))
					{
						if(it_server->server_accept() != 0)
							exit(1);
						if (it_server->receive_msg() != 0)
							exit(1);
						if (it_server->send_msg() != 0)
							exit(1);
						close(it_server->getClient());
					}
				}
			}
		}
		status = 0;
	}
}

int main(int ac, char **av, char **envp)
{
	g_ctrl_c_called = 0;
	if(ac != 2)
	{
		std::cerr << "usage: ./server [PORT]" << std::endl;
		return (1);
	}
	Conf conf(av[1]);
	std::vector<Server> all_server = conf.create_all_server(envp);
	conf.ft_print_content();
	routine(all_server);
	std::cerr << "Closing webserver..." << std::endl;
	return (0);
} 