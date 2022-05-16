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

void	ft_close_webserver(std::string str)
{
	std::cerr << str << std::endl;
	g_ctrl_c_called = 1;
}

int		loop_cycle(fd_set & select_set_read_dump, fd_set & select_set_read_ready, int max_fd)
{
	int status = 0;
	struct timeval select_timeout;
	select_timeout.tv_sec = 1;
	select_timeout.tv_usec = 0;
	while(status == 0 && g_ctrl_c_called == 0)
	{
		FD_ZERO(&select_set_read_ready);
		select_set_read_ready = select_set_read_dump;
		usleep(2000);
		if((status = select(max_fd + 1, &select_set_read_ready, NULL, NULL, &select_timeout)) < 0)
		{
			if(g_ctrl_c_called == 1)
				return (-1);
		}
	}
	return (1);
}

void	routine(std::vector<Server> & all_server)
{
	fd_set select_set_read_dump;
	fd_set select_set_read_ready;
	FD_ZERO(&select_set_read_dump);
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
	while (g_ctrl_c_called == 0)
	{
		if(loop_cycle(select_set_read_dump, select_set_read_ready, max_fd) < 0)
			return ft_close_webserver("");
		if(g_ctrl_c_called == 0)
		{
			for(std::vector<Server>::iterator it_server = all_server.begin(); it_server != all_server.end(); it_server++)
			{
				std::vector<Socket> temp =  it_server->get_all_socket();
				for(std::vector<Socket>::iterator it_socket = temp.begin(); it_socket != temp.end(); it_socket++)
				{
					if(FD_ISSET(it_socket->getServerFd(), &select_set_read_ready))
					{
						if(it_server->server_accept() != 0)
							return ft_close_webserver("");
						if (it_server->receive_msg() != 0)
							return ft_close_webserver("");
						if (it_server->send_msg() != 0)
							return ft_close_webserver("");
						close(it_server->getClient());
					}
				}
			}
		}
	}
}

void	close_all_socket(std::vector<Server> & all_server)
{
	for(std::vector<Server>::iterator it_server = all_server.begin(); it_server != all_server.end(); it_server++)
	{
		std::vector<Socket> temp =  it_server->get_all_socket();
		for(std::vector<Socket>::iterator it_socket = temp.begin(); it_socket != temp.end(); it_socket++)
			it_socket->close_fd();
	}
}

int main(int ac, char **av, char **envp)
{
	g_ctrl_c_called = 0;
	std::vector<Server> all_server;
	if(ac != 2)
	{
		std::cerr << "usage: ./server [file.conf]" << std::endl;
		return (1);
	}
	signal(signal_nbr, ft_signal_handler);
	Conf conf;
	if(conf.parse_conf_file(av[1]) == 0)
	{
		//conf.ft_print_content();
		try
		{
			all_server = conf.create_all_server(envp);
		}
		catch(const std::exception& e)
		{
			g_ctrl_c_called = 1;
		}		
		routine(all_server);
		close_all_socket(all_server);
	}
	std::cout << "Closing webserver sucessfully.." << std::endl;
	return (0);
} 