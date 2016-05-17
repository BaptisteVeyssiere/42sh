/*
** mysh.h for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2/test
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Fri Apr  8 16:22:17 2016 Baptiste veyssiere
** Last update Tue Apr 12 16:10:56 2016 Baptiste veyssiere
*/

#ifndef MYSH_H_
# define MYSH_H_

#include "get_next_line.h"

# define UNUSED __attribute__((unused))


typedef struct
{
  int		fd_input;
  int		fd_output;
  char		if_double_left;
  char		if_double_right;
  char		pipe;
  char		prev;
  char		*double_left_end;
  char          *str;
  char		**args;
}               t_interpipe;

typedef struct
{
  int           pipe_nbr;
  t_interpipe   **command;
}               t_command;

/*
** get_red_tab.c
*/

char	**get_red_tab(char*);

/*
** get_interpipe_tab.c
*/

t_interpipe	**get_interpipe_tab(char**);

/*
** get_command_tab.c
*/

char	**get_command_tab(char*);

/*
** sort_tree.c
*/

int	sort_tree(t_command**, char**);

/*
** tools.c
*/

void	free_wordtab(char**);
int	my_perror(char*);
int	my_perror_function(char*);
void	my_strcpy(char*, char*);
int	my_strlen(char*);
int	my_strcmp(char*, char*);
int     my_strcmp_strict(char*, char*);
int     my_tablen(char**);
void    my_puterr(char*);
int     my_getnbr(char*);

/*
** get_fd.c
*/

int     get_fd_output(t_interpipe*);
int     get_fd_input(t_interpipe*);
int	get_fd_out(char*, char);
int	get_fd_in(char*);

/*
** get_redirection_nbr.c
*/

int     get_left_redirection_nbr(char*);
int     get_right_redirection_nbr(char*);

/*
** epure_str.c
*/

int     get_epure_str_length(char*);

/*
** get_full_tree.c
*/

int     get_full_tree(char*, char ***);

/*
** str_to_wordtab.c
*/

char	**str_to_wordtab(char*);

/*
** check_if_exist.c
*/

int	check_if_exist(char**, char**);

/*
** prompt.c
*/

void    aff_prompt(char**);
char    **get_prompt();
int	free_prompt(char**);

/*
** slash_test.c
*/

int	slash_test(char*);

/*
** get_varpath.c
*/

char    *str_to_word_path(char*, int);
char    **get_varpath(char*);
int     get_word_nbr_path(char*);

/*
** get_varenv.c
*/

char    *get_varenv(char**, char*);

/*
** check_instruction.c
*/

char	**check_instruction(char*, char**);
int     check_if_builtins(char*);

/*
** do_instruction.c
*/

int     execute_tree(t_command**, char***);
int     do_instruction(t_command*, char***, int);
char    *get_varenv(char**, char*);
int     exec_builtins(char**, char***);

/*
** env.c
*/

int     env_builtin(char**, char*);

/*
** setenv.c
*/

char    **setenv_builtin(char**, char**, int*);
char    **set_new_varenv(char**, char**);
char    **reset_varenv(char**, char**, int);

/*
** unsetenv.c
*/

char    **unsetenv_builtin(char**, char**, int*);

/*
** exit.c
*/

int     exit_builtin(char**, char**);

/*
** cd.c
*/

char    **cd_builtin(char**, char**, int*);
char    **cd_home(char**);
char    **cd_prev(char**, char**);
char    **cd_basic(char**, char**);

/*
** env_copy.c
*/

void    free_env(char**);
char    *copy_varenv(char*);
char    **env_copy(char**);

/*
** builtins.c
*/

char	**builtins(char**, char**, char*);

/*
** change_fd.c
*/

int     change_fd_on(int*, int*, t_command*, int);

/*
** double_left_red.c
*/

int     double_left_red(t_interpipe*);

/*
** get_fd_bis.c
*/

int     get_fd_output_bis(t_interpipe*, int, int, char*);
int     get_fd_input_bis(t_interpipe*, int, int, char*);

/*
** execute_interpipe.c
*/

int     execute_interpipe(t_command*, char***);

/*
** is_builtin.c
*/

int	is_builtin(t_command*, int, int);

#endif /* !MYSH_H_ */
