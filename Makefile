SRCS = $(wildcard *.cpp) $(wildcard config_file_parsing/*.cpp) $(wildcard response/*.cpp)

HEADERS = $(wildcard *.hpp) $(wildcard config_file_parsing/*.hpp) $(wildcard response/*.hpp)

CPP = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3

OBJS = $(SRCS:.cpp=.o)

NAME = webserv

all: $(NAME)

$(NAME): $(OBJS) $(HEADERS)
	@$(CPP) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "\033[35mCreating webserv!\033[0m"

%.o: %.cpp
	@$(CPP) $(CFLAGS) -c $< -o $@

run: $(NAME)
	@./$(NAME) config

clean:
	@rm -f $(OBJS)
	@echo "\033[35mDeleting object files!\033[0m"

fclean: clean
	@rm -f $(NAME)
	@echo "\033[35mDeleting webserv!\033[0m"

re: fclean all
