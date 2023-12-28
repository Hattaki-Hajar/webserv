CONF_PATH = config_file_parsing/

RES_PATH = response/

OBJ_PATH = obj/

SRCS = $(wildcard *.cpp) $(wildcard config_file_parsing/*.cpp) $(wildcard response/*.cpp)

HEADERS = $(wildcard *.hpp) $(wildcard config_file_parsing/*.hpp) $(wildcard response/*.hpp)

CPP = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3

OBJS = $(addprefix $(OBJ_PATH), $(SRCS:.cpp=.o))

NAME = webserv

all: $(NAME)

$(NAME): $(OBJS) $(HEADERS)
	@$(CPP) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "\033[35mCreating webserv!\033[0m"

$(OBJ_PATH)%.o: %.cpp
	@mkdir -p $(OBJ_PATH) $(OBJ_PATH)$(CONF_PATH) $(OBJ_PATH)$(RES_PATH)
	@$(CPP) $(CFLAGS) -c $< -o $@

run: $(NAME)
	@./$(NAME) config

clean:
	@rm -rf $(OBJ_PATH)
	@rm -f $(OBJS)
	@echo "\033[35mDeleting object files!\033[0m"

fclean: clean
	@rm -f $(NAME)
	@echo "\033[35mDeleting webserv!\033[0m"

re: fclean all
