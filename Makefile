##
## Makefile for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
##
## Made by Baptiste veyssiere
## Login   <VEYSSI_B@epitech.net>
##
## Started on  Wed May 18 21:26:30 2016 Baptiste veyssiere
## Last update Thu Jun  2 11:30:47 2016 vigner_g
##

NAME	= 42sh

RM	= rm -f

CC	= gcc

MKDIR	= mkdir -p

SRC	= src/main.c \
	src/get_tree.c \
	src/error.c \
	src/tools.c \
	src/free_tools.c \
	src/epure_str.c \
	src/execute_command.c \
	src/get_and_or_tree.c \
	src/get_interpipe_tree.c \
	src/get_command_tree.c \
	src/get_and_or_tree_add.c \
	src/str_to_wordtab.c \
	src/fill_leaf.c \
	src/get_redirect_files.c \
	src/prompt.c \
	src/env_copy.c \
	src/get_next_line.c \
	src/check_and_add_path.c \
	src/slash_test.c \
	src/get_varpath.c \
	src/get_varenv.c \
	src/open_fd.c \
	src/execute_interpipe.c \
	src/do_instruction.c \
	src/double_left_red.c \
	src/builtins.c \
	src/setenv.c \
	src/unsetenv.c \
	src/env.c \
	src/echo.c \
	src/exit.c \
	src/cd.c \
	src/tools_bis.c \
	src/history.c	\
	src/profiles.c	\
	src/aff_history.c

SRCDIR	= src

OBJDIR	= obj

OBJ	= $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

CFLAGS	= -W -Wextra -Wall -ansi -pedantic -Iinclude -g

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
