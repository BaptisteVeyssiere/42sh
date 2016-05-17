/*
** env.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed Mar 30 16:30:09 2016 Baptiste veyssiere
** Last update Tue Apr  5 14:25:35 2016 Baptiste veyssiere
*/

#include <unistd.h>
#include "mysh.h"

int	env_builtin(char **env, char *command)
{
  char	*temp;
  int	i;

  temp = "env";
  i = -1;
  while (temp[++i])
    if (command[i] != temp[i])
      return (0);
  if (command[i])
    return (0);
  if (env == NULL)
    return (1);
  i = -1;
  while (env[++i] != NULL)
    {
      write(1, env[i], my_strlen(env[i]));
      write(1, "\n", 1);
    }
  return (1);
}
