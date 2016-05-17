/*
** setenv.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed Mar 30 16:40:11 2016 Baptiste veyssiere
** Last update Sun Apr 10 04:45:23 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include <unistd.h>
#include "mysh.h"

char	*new_var(char **command)
{
  char	*var;
  int	i;
  int	j;
  int	length;

  length = my_strlen(command[1]) + 1 + my_strlen(command[2]);
  if ((var = malloc(sizeof(char) * (length + 1))) == NULL)
    return (NULL);
  var[length] = 0;
  i = -1;
  while (command[1][++i] && command[1][i] != '=')
    var[i] = command[1][i];
  var[i] = '=';
  j = -1;
  if (command[2] != NULL)
    while (command[2][++j])
      var[++i] = command[2][j];
  return (var);
}

char	**reset_varenv(char **env, char **command, int index)
{
  char  **copy;
  int   length;
  int   i;

  if (env == NULL || env[0] == NULL)
    return (NULL);
  length = -1;
  while (env != NULL && env[++length] != NULL);
  if ((copy = malloc(sizeof(char*) * (length + 1))) == NULL)
    return (NULL);
  copy[length] = NULL;
  i = -1;
  while (env != NULL && ++i < length)
    if (i == index)
      {
	if ((copy[i] = new_var(command)) == NULL)
	  return (NULL);
      }
    else if ((copy[i] = copy_varenv(env[i])) == NULL)
      return (NULL);
  if (env != NULL && env[0] != NULL)
    free_env(env);
  return (copy);
}

char	**set_new_varenv(char **env, char **command)
{
  char  **copy;
  int   length;
  int   i;

  length = -1;
  if (env != NULL)
    while (env[++length] != NULL);
  else
    length = 0;
  if ((copy = malloc(sizeof(char*) * (length + 2))) == NULL)
    return (NULL);
  copy[length + 1] = NULL;
  i = -1;
  while (++i < length)
    if ((copy[i] = copy_varenv(env[i])) == NULL)
      return (NULL);
  if ((copy[i] = new_var(command)) == NULL)
    return (NULL);
  if (env != NULL && env[0] != NULL)
    free_env(env);
  return (copy);
}

char	**setenv_ret(char **env, char **command, int i, char *var)
{
  while (command[1][++i] && command[1][i] != '=')
    var[i] = command[1][i];
  var[i] = '=';
  var[++i] = 0;
  i = -1;
  if (env != NULL)
    while (env[++i] != NULL && !my_strcmp(var, env[i]));
  if (env == NULL || env[i] == NULL)
    {
      if ((env = set_new_varenv(env, command)) == NULL && i > -1)
	return (NULL);
    }
  else if ((env = reset_varenv(env, command, i)) == NULL)
    return (NULL);
  free(var);
  return (env);
}

char	**setenv_builtin(char **env, char **command, int *check)
{
  int	i;
  int	length;
  char	*var;

  if (my_strcmp_strict("setenv", command[0]) == 0)
    return (env);
  *check = 1;
  if ((i = -1) == -1 && my_tablen(command) == 1)
    {
      while (env != NULL && env[++i] != NULL)
	{
	  write(1, env[i], my_strlen(env[i]));
	  write(1, "\n", 1);
	}
      return (env);
    }
  if ((length = my_tablen(command)) < 2)
    my_puterr("setenv: Too few arguments.\n");
  if (length > 3)
    my_puterr("setenv: Too many arguments.\n");
  if (length < 2 || length > 3)
    return (env);
  if ((var = malloc(sizeof(char) * (my_strlen(command[1]) + 2))) == NULL)
    return (NULL);
  return (setenv_ret(env, command, i, var));
}
