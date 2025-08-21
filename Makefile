CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = includes

NAME = ircserv

SRC_FILES = main.cpp  Server.cpp Client.cpp  # Command.cpp

SRCS = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
#SRCS += $(addprefix $(SRC_DIR)/commands/, $(SRC_COMMAND_FILES))

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
run:
	./ircserv 5555 123
val:
	valgrind \
	  --leak-check=full \
	  --show-leak-kinds=all \
	  --track-fds=yes \
	  --track-origins=yes \
	  ./ircserv 5555 123
clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all