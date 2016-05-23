/*
** mysh.h for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed May 18 21:30:46 2016 Baptiste veyssiere
** Last update Mon May 23 23:07:53 2016 Baptiste veyssiere
*/

#ifndef MYSH_H_
# define MYSH_H_

# include "get_next_line.h"

# define UNUSED __attribute__((unused))

typedef struct
{
  int           fd_input;
  int           fd_output;
  char          if_double_left;
  char          if_double_right;
  char          pipe;
  char          prev;
  char          *double_left_end;
  char          *str;
  char          **args;
}               t_interpipe;

typedef struct	s_and_or
{
  char		*str;
  t_interpipe	**command;
  int		pipe_nbr;
  char		and;
  char		or;
}		t_and_or;

typedef struct	s_tree
{
  t_and_or	**and_or;
}		t_tree;

/*
** get_tree.c
*/
int	get_tree(char*, char***);

/*
** get_command_tab.c
*/
char	**get_command_tab(char*);

/*
** tools.c
*/
int	my_strlen(char*);
void	my_strcpy(char*, char*);
int	my_perror(char*);
int	my_strcmp(char*, char*);

/*
** epure_str.c
*/
char	*epure_str(char*);
int	get_epure_str_length(char*);

/*
** get_and_or_tree.c
*/
t_tree	**get_and_or_tree(char**, int*);

/*
** get_and_or_tree_bis.c
*/
t_and_or	*get_and_or_command(char*, int*, int*);

/*
** get_red_tab.c
*/
char	**get_red_tab(char*);

/*
** get_pipe_nbr.c
*/
int	get_pipe_nbr(char*);

/*
** get_interpipe_tab.c
*/
t_interpipe	**get_interpipe_tab(char**);

/*
** sort_tree.c
*/
int	sort_tree(t_interpipe**, char**);

/*
** check_instruction.c
*/
char    **check_instruction(char*, char**);
int	check_if_builtins(char*);

/*
** get_redirection_nbr.c
*/
int	get_left_redirection_nbr(char*);
int	get_right_redirection_nbr(char*);

/*
** get_fd.c
*/
int	get_fd_output(t_interpipe*);
int	get_fd_input(t_interpipe*);
int	get_fd_out(char*, char);
int	get_fd_in(char*);

/*
** str_to_wordtab.c
*/
char	**str_to_wordtab(char*);

/*
** check_if_exist.c
*/
int	check_if_exist(char**, char**);

/*
** get_fd_bis.c
*/
int	get_fd_output_bis(t_interpipe*, int, int, char*);
int	get_fd_input_bis(t_interpipe*, int, int, char*);

/*
** slash_test.c
*/
int	slash_test(char*);

/*
** get_varenv.c
*/
char	*get_varenv(char**, char*);

/*
** get_varpath.c
*/
char	*str_to_word_path(char*, int);
char	**get_varpath(char*);
int	get_word_nbr_path(char*);

/*
** prompt.c
*/
void    aff_prompt(char**);
char    **get_prompt();
int     free_prompt(char**);

/*
** env_copy.c
*/
void    free_env(char**);
char    *copy_varenv(char*);
char    **env_copy(char**);

/*
** free_tools.c
*/
void	free_tree(t_tree**);
void	free_wordtab(char**);

/*
** tools_bis.c
*/
int	my_strcmp_strict(char*, char*);
int	my_perror_function(char*);
int	my_tablen(char**);
void	my_puterr(char*);
int	my_getnbr(char*);

/*
** do_instruction.c
*/
int	execute_tree(t_tree**, char***);
int	do_instruction(t_and_or*, char***, int);
int	exec_builtins(char**, char***);

/*
** execute_interpipe.c
*/
int	execute_interpipe(t_and_or*, char***, char*);

/*
** builtins.c
*/
char	**builtins(char**, char**, char*);

/*
** double_left_red.c
*/
int	double_left_red(t_interpipe*);

/*
** change_fd.c
*/
int	change_fd_on(int*, int*, t_and_or*, int);

/*
** is_builtin.c
*/
int	is_builtin(t_and_or*, int, int);

/*
** env.c
*/
int	env_builtin(char**, char*);

/*
** setenv.c
*/
char	**setenv_builtin(char**, char**, int*);

/*
** unsetenv.c
*/
char	**unsetenv_builtin(char**, char**, int*);

/*
** exit.c
*/
int	exit_builtin(char**, char**);

/*
** cd.c
*/
char	**cd_builtin(char**, char**, int*);

#endif /* !MYSH_H_ */
