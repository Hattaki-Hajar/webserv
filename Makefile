SRCS = $(wildcard *.cpp)

HEADERS = $(wildcard *.hpp)

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

clean:
	@rm -f $(OBJS)
	@echo "\033[35mDeleting object files!\033[0m"

fclean: clean
	@rm -f $(NAME)
	@echo "\033[35mDeleting webserv!\033[0m"

re: fclean all
