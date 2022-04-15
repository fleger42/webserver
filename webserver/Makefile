NAME = webserv

CXX = clang++

CXXFLAGS = -Wall -Wextra -Werror -g -fsanitize=address #-std=c++98

HEADER = -I include/

SRCS = main.cpp Conf.cpp Location.cpp VirtualServer.cpp utils.cpp Server.cpp Socket.cpp Cgi.cpp

SRC = $(addprefix srcs/, $(SRCS))

OBJ = $(SRC:cpp=o)

all: $(NAME)

$(NAME): $(OBJ)
	@echo "\n"
	@echo "\033[0;32mCompiling webserv..."
	@$(CXX) $(CXXFLAGS) $(HEADER) -o $(NAME) $(OBJ)
	@echo "\n\033[0mDone !"

%.o: %.c
	@printf "\033[0;33mGenerating webserv objects... %-33.33s\r" $@
	@${CXX} ${CXXFLAGS} $(HEADER) -c $< -o $@

clean:
	@echo "\nRemoving binaries..."
	@rm -f $(OBJ)
	@echo "\033[0m"

fclean:
	@echo "\nRemoving binaries..."
	@rm -f $(OBJ)
	@echo "\033[0m"
	@echo "\nDeleting executable..."
	@rm -f $(NAME)
	@echo "\033[0m"

re: fclean all

.PHONY: clean fclean re test norm
