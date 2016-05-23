/*
** do_instruction.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2/test
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Sat Apr  9 18:20:17 2016 Baptiste veyssiere
** Last update Mon May 23 23:22:59 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "mysh.h"

int	exec_builtins(char **args, char ***env)
{
  char	check;
  char	key;

  key = 0;
  if (*env == NULL)
    key = 1;
  check = 0;
  if ((*env = builtins(args, *env, &check)) == NULL && key == 0)
    return (-1);
  if (check == 1)
    return (0);
  return (0);
}

int	exec_instruction(char builtin, char function,
			 t_interpipe *command, char ***env)
{
  if (builtin == 0 && function == 0)
    {
      my_perror_function(command->args[0]);
      exit(1);
    }
  else if (builtin)
    {
      exec_builtins(command->args, env);
      exit (0);
    }
  else if (function)
    {
      if (execve(command->args[0],
                 command->args, *env) == -1)
        return (-1);
    }
  return (0);
}

int	do_instruction(t_and_or *command, char ***env, int i)
{
  char	builtin;
  char	function;
  int	fd_input;
  int	fd_output;

  if (command->command[i] == NULL)
    return (0);
  fd_input = -2;
  fd_output = -2;
  if (command->command[i]->if_double_left)
    if (double_left_red(command->command[i]) == -1)
      return (-1);
  if (change_fd_on(&fd_input, &fd_output, command, i) == -1)
    return (-1);
  function = 1;
  builtin = 0;
  builtin = check_if_builtins(command->command[i]->args[0]);
  if (slash_test(command->command[i]->args[0]) == 0
      || access(command->command[i]->args[0], F_OK) != 0)
    function = 0;
  if (exec_instruction(builtin, function, command->command[i], env) == -1)
    return (-1);
  return (0);
}

int	execute_tree(t_tree **tree, char ***env)
{
  int	i;
  int	j;
  int	error;
  char	check_if_fail;
  char	tmp;

  i = -1;
  check_if_fail = 0;
  tmp = 0;
  while (tree[++i] != NULL)
    {
      j = -1;
      while (tree[i]->and_or[++j])
	if (j == 0 || (check_if_fail == 0 && tree[i]->and_or[j]->and == 1) ||
	    (check_if_fail == 1 && tree[i]->and_or[j]->or == 1))
	  {
	    tmp = check_if_fail;
	    if ((error = execute_interpipe(tree[i]->and_or[j], env, &check_if_fail)) != 0)
	      return (error);
	    if (tmp == 1 && check_if_fail == 1)
	      check_if_fail = 0;
	  }
	else if (j != 0 && check_if_fail == 0 && tree[i]->and_or[j]->or == 1)
	  while (tree[i]->and_or[j + 1])
	    ++j;
    }
  return (0);
}
