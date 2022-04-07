#ifndef HEADER_HPP
#define HEADER_HPP
# define BLACK    "\033[0;30m"
# define RED      "\033[0;31m"
# define GREEN    "\033[0;32m"
# define YELLOW   "\033[0;33m"
# define BLUE     "\033[0;34m"
# define PURPLE   "\033[0;35m"
# define CYAN     "\033[0;36m"
# define GREY     "\033[0;37m"
# define NORMAL	"\033[00m"
#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include "Conf.hpp"
#include "VirtualServer.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include <signal.h>
#include "Socket.hpp"
void mutefo(char *str, char c);
void mutefo(std::string & str, char c);
void	ft_signal_handler(int signal);
#endif