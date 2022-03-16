#ifndef HEADER_HPP
#define HEADER_HPP
#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include "Conf.hpp"
#include "VirtualServer.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include "Socket.hpp"
void mutefo(char *str, char c);
void mutefo(std::string & str, char c);
#endif