/*
** exit.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed Mar 30 16:35:16 2016 Baptiste veyssiere
** Last update Mon Apr 11 20:28:57 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include <unistd.h>
#include "mysh.h"

int	check_formation(char *str)
{
  int	i;

  i = -1;
  while (str[++i])
    {
      if (str[i] > '9' || str[i] < '0')
	{
	  if (i != 0 || str[i] != '-')
	    return (-1);
	}
    }
  return (0);
}

int	error_exit(char **command)
{
  if (check_formation(command[1]) == -1)
    {
      my_perror("exit: Badly formed number.\n");
      return (1);
    }
  if (my_tablen(command) != 2)
    {
      my_perror("exit: Expression Syntax.\n");
      return (1);
    }
  return (0);
}

int	exit_builtin(char **command, char **env)
{
  char	*temp;
  int	nbr;

  temp = "exit";
  if (my_strcmp_strict(temp, command[0]) == 0)
    return (0);
  if (command[1] == NULL)
    nbr = 0;
  else
    {
      if (error_exit(command) == 1)
	return (1);
      nbr = my_getnbr(command[1]);
    }
  if (env != NULL)
    free_env(env);
  free_wordtab(command);
  write(1, "exit\n", 5);
  exit(nbr);
  return (1);
}
