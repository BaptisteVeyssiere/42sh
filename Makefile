##
## Makefile for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2
##
## Made by Baptiste veyssiere
## Login   <VEYSSI_B@epitech.net>
##
## Started on  Tue Mar 29 16:23:09 2016 Baptiste veyssiere
## Last update Tue May 17 17:51:59 2016 Baptiste veyssiere
##

NAME	= 42sh

RM	= rm -f

CC	= gcc

CFLAGS	= -W -Wall -Wextra -Werror -ansi -pedantic -I./include

SRCS	= src/mysh.c \
	src/prompt.c \
	src/get_next_line.c \
	src/tools.c \
	src/tools_bis.c	\
        src/get_full_tree.c \
        src/sort_tree.c	\
        src/get_interpipe_tab.c	\
        src/get_command_tab.c \
        src/epure_str.c	\
        src/get_fd.c \
        src/get_redirection_nbr.c \
        src/get_red_tab.c \
        src/check_instruction.c	\
        src/check_if_exist.c \
        src/str_to_wordtab.c \
        src/slash_test.c \
        src/get_varpath.c \
        src/get_varenv.c \
        src/cd.c \
        src/env.c \
        src/setenv.c \
        src/unsetenv.c \
        src/exit.c \
        src/do_instruction.c \
        src/env_copy.c \
        src/builtins.c \
	src/change_fd.c	\
	src/double_left_red.c \
	src/get_fd_bis.c \
	src/execute_interpipe.c	\
	src/is_builtin.c

OBJS	= $(SRCS:.c=.o)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS)


all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
