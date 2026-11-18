##
## 42sh — a small Unix shell
##

NAME		= 42sh

CC		= gcc
RM		= rm -f
MKDIR		= mkdir -p

SRCDIR		= src
OBJDIR		= obj

SRC		= src/main.c \
		  src/syntax.c \
		  src/get_tree.c \
		  src/error.c \
		  src/alias_tools_bis.c \
		  src/tools.c \
		  src/free_tools.c \
		  src/epure_str.c \
		  src/alias.c \
		  src/source_command.c \
		  src/alias_tools.c \
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
		  src/history.c \
		  src/profiles.c \
		  src/aff_history.c \
		  src/check_if_directory.c \
		  src/verif_command.c \
		  src/check_command.c \
		  src/match.c \
		  src/check_var.c \
		  src/my_getstr.c \
		  src/check_status.c \
		  src/get_dir.c \
		  src/check_dir_permission.c

OBJ		= $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEP		= $(OBJ:.o=.d)

CPPFLAGS	= -Iinclude -D_POSIX_C_SOURCE=200809L
CFLAGS		= -std=gnu11 -Wall -Wextra -Werror -pedantic
LDFLAGS		=

ifeq ($(ASAN),1)
CFLAGS		+= -fsanitize=address,undefined -fno-omit-frame-pointer -g
LDFLAGS		+= -fsanitize=address,undefined
endif

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) -o $(NAME) $(OBJ)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(MKDIR) $(OBJDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
	$(RM) $(OBJ) $(DEP)
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

test: $(NAME)
	./tests/test_shell.sh

.PHONY: all clean fclean re test

-include $(DEP)
