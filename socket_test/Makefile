NAME = server

CXX = clang++

CXXFLAGS = -Wall -Wextra -Werror -I include/

HEADER =

SRCS = main.cpp

SRC = $(addprefix srcs/, $(SRCS))

OBJ = $(SRC:cpp=o)

all: $(NAME)

$(NAME): $(OBJ) 
	@echo "\n"
	@echo "\033[0;32mCompiling server..."
	@$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)
	@echo "\n\033[0mDone !"
	@$(CXX) $(CXXFLAGS) -o client srcs/main_client.cpp srcs/get_next_line_utils.cpp srcs/get_next_line.cpp

%.o: %.c
	@printf "\033[0;33mGenerating server objects... %-33.33s\r" $@
	@${CXX} ${CXXFLAGS} -c $< -o $@

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
	@rm -f client
	@echo "\033[0m"

re: fclean all

.PHONY: clean fclean re test norm
