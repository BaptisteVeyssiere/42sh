##
## Makefile for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
##
## Made by Baptiste veyssiere
## Login   <VEYSSI_B@epitech.net>
##
## Started on  Wed May 18 21:26:30 2016 Baptiste veyssiere
## Last update Mon May 23 21:35:07 2016 Baptiste veyssiere
##

NAME	= 42sh

RM	= rm -f

CC	= gcc

MKDIR	= mkdir -p

SRC	= src/main.c \
	src/tools.c \
	src/get_tree.c \
	src/get_command_tab.c \
	src/epure_str.c \
	src/get_and_or_tree.c \
	src/get_and_or_tree_bis.c \
	src/get_pipe_nbr.c \
	src/get_interpipe_tab.c \
	src/get_red_tab.c \
	src/sort_tree.c \
	src/check_instruction.c \
	src/get_redirection_nbr.c \
	src/get_fd.c \
	src/str_to_wordtab.c \
	src/check_if_exist.c \
	src/get_fd_bis.c \
	src/slash_test.c \
	src/get_varenv.c \
	src/get_varpath.c \
	src/free_tools.c \
	src/prompt.c \
	src/get_next_line.c \
	src/env_copy.c \
	src/tools_bis.c \
	src/do_instruction.c \
	src/execute_interpipe.c \
	src/builtins.c \
	src/double_left_red.c \
	src/change_fd.c \
	src/is_builtin.c \
	src/env.c \
	src/setenv.c \
	src/unsetenv.c \
	src/cd.c \
	src/exit.c

SRCDIR	= src

OBJDIR	= obj

OBJ	= $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

CFLAGS	= -W -Werror -Wextra -Wall -ansi -pedantic -Iinclude -g

$(NAME): $(OBJ)
	@$(CC) -o $(NAME) $(OBJ)
	@echo "Linking complete!"

$(OBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(MKDIR) $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

all: $(NAME)

clean:
	@$(RM) $(OBJ)
	@echo "Cleanup complete!"

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
