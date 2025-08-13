# Compiler settings
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

# Directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = includes

# Main executable name
NAME = ircserv

# Source files
SRC_FILES = main.cpp  Server.cpp Client.cpp Channel.cpp # Command.cpp
#SRC_COMMAND_FILES = NickCommand.cpp JoinCommand.cpp PartCommand.cpp PrivmsgCommand.cpp QuitCommand.cpp

SRCS = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
#SRCS += $(addprefix $(SRC_DIR)/commands/, $(SRC_COMMAND_FILES))

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Phony targets
.PHONY: all clean fclean re

# Default target
all: $(NAME)

# Link the executable
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

# Compile C++ source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Create the object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up object files
clean:
	rm -rf $(OBJ_DIR)

# Clean up object files and the executable
fclean: clean
	rm -f $(NAME)

# Rebuild the project
re: fclean all