/*
** builtins.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2/test
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Sat Apr  9 19:13:04 2016 Baptiste veyssiere
** Last update Mon Apr 11 20:04:25 2016 Baptiste veyssiere
*/

#include <unistd.h>
#include "mysh.h"

char	**builtins(char **command, char **env, char *check)
{
  char	env_key;
  int	key;

  key = 0;
  env_key = 0;
  if (env == NULL)
    env_key = 1;
  if (env_builtin(env, command[0]) != 0)
    *check = 1;
  else if (command[0] != NULL)
    {
      if ((env = setenv_builtin(env, command, &key)) == NULL && env_key == 0)
	return (NULL);
      else if ((env = unsetenv_builtin(env, command, &key)) == NULL
	       && env_key == 0)
	return (NULL);
      else if (exit_builtin(command, env) != 0)
	*check = 1;
      else if ((env = cd_builtin(env, command, &key)) == NULL && env_key == 0)
	return (NULL);
    }
  if (key == 1)
    *check = 1;
  return (env);
}
