/*
** unsetenv.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed Mar 30 16:41:28 2016 Baptiste veyssiere
** Last update Sun Apr 10 04:22:12 2016 Baptiste veyssiere
*/

#include <unistd.h>
#include <stdlib.h>
#include "mysh.h"

char	**unset_var(char **env, int index)
{
  char  **copy;
  int   length;
  int   i;
  int	minus;

  if (env[0] == NULL)
    return (NULL);
  length = -1;
  while (env[++length] != NULL);
  if ((copy = malloc(sizeof(char*) * length)) == NULL)
    return (NULL);
  copy[length - 1] = NULL;
  i = -1;
  minus = 0;
  while (++i < length)
    {
      if (i == index)
	minus = 1;
      if (i != index && (copy[i - minus] = copy_varenv(env[i])) == NULL)
	return (NULL);
    }
  free_env(env);
  return (copy);
}

char	**unsetenv_ret(char **env, char **command, int i, char *var)
{
  while (command[1][++i] && command[1][i] != '=')
    var[i] = command[1][i];
  var[i] = '=';
  var[++i] = 0;
  i = -1;
  if (env != NULL)
    while (env[++i] != NULL && !my_strcmp(var, env[i]));
  if (env[i] == NULL)
    {
      free(var);
      return (env);
    }
  else if ((env = unset_var(env, i)) == NULL)
    return (NULL);
  free(var);
  return (env);
}

char	**unsetenv_builtin(char **env, char **command, int *check)
{
  int   i;
  char  *var;

  if (my_strcmp_strict("unsetenv", command[0]) == 0)
    return (env);
  *check = 1;
  if (my_tablen(command) != 2)
    {
      if (my_tablen(command) < 2)
	my_puterr("unsetenv: Too few arguments.\n");
      else
	my_puterr("unsetenv: Too many arguments.\n");
      return (env);
    }
  if (env == NULL)
    return (env);
  i = -1;
  if ((var = malloc(sizeof(char) * (my_strlen(command[1]) + 2))) == NULL)
    return (NULL);
  return (unsetenv_ret(env, command, i, var));
}
