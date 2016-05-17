/*
** cd.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed Mar 30 16:41:56 2016 Baptiste veyssiere
** Last update Mon Apr 11 23:40:23 2016 Baptiste veyssiere
*/

#include <unistd.h>
#include <stdlib.h>
#include "mysh.h"

char	**cd_setenv(char **env, char *var, char *value)
{
  char	**tab;
  char	**copy;
  int	i;

  if ((tab = malloc(sizeof(char*) * 4)) == NULL ||
      (tab[0] = malloc(sizeof(char) * (my_strlen("setenv") + 1))) == NULL ||
      (tab[1] = malloc(sizeof(char) * (my_strlen(var) + 1))) == NULL ||
      (tab[2] = malloc(sizeof(char) * (my_strlen(value) + 1))) == NULL)
    return (NULL);
  tab[3] = NULL;
  my_strcpy("setenv", tab[0]);
  my_strcpy(var, tab[1]);
  my_strcpy(value, tab[2]);
  i = 0;
  if ((copy = setenv_builtin(env, tab, &i)) == NULL)
    return (NULL);
  free_wordtab(tab);
  return (copy);
}

char	**cd_home(char **env)
{
  char	*home;
  char	*pwd;

  if ((home = get_varenv(env, "HOME")) == NULL ||
      (pwd = get_varenv(env, "PWD")) == NULL)
    return (NULL);
  if (chdir(home) == -1)
    return (NULL);
  if ((env = cd_setenv(env, "OLDPWD", pwd)) == NULL ||
      (env = cd_setenv(env, "PWD", home)) == NULL)
    return (NULL);
  free(pwd);
  free(home);
  return (env);
}

char		**cd_prev(char **env, char **oldpwd)
{
  char		*pwd;
  char		*buf;

  buf = NULL;
  if ((pwd = getcwd(buf, 100)) == NULL)
    return (NULL);
  if (chdir(*oldpwd) == -1)
    return (NULL);
  if ((env = cd_setenv(env, "OLDPWD", pwd)) == NULL ||
      (env = cd_setenv(env, "PWD", *oldpwd)) == NULL)
    return (env);
  free(*oldpwd);
  *oldpwd = pwd;
  return (env);
}

char	**cd_basic(char **command, char **env)
{
  char	*pwd;
  char	*buf;

  buf = NULL;
  if ((pwd = getcwd(buf, 100)) == NULL)
    return (NULL);
  if (chdir(command[1]) == -1)
    return (NULL);
  buf = NULL;
  if ((env = cd_setenv(env, "OLDPWD", pwd)) == NULL ||
      (env = cd_setenv(env, "PWD", getcwd(buf, 100))) == NULL)
    return (NULL);
  free(buf);
  free(pwd);
  return (env);
}

char		**cd_builtin(char **env, char **command, int *check)
{
  static char	*oldpwd = NULL;
  char		*buf;

  if (my_strcmp_strict(command[0], "cd") == 0)
    return (env);
  *check = 1;
  buf = NULL;
  if (command[1] == NULL || my_strcmp_strict("~", command[1]))
    {
      if ((oldpwd = getcwd(buf, 100)) == NULL)
	return (NULL);
      if ((env = cd_home(env)) == NULL)
	return (NULL);
    }
  else if (my_strcmp_strict("-", command[1]))
    {
      if ((env = cd_prev(env, &oldpwd)) == NULL)
	return (NULL);
    }
  else if ((oldpwd = getcwd(buf, 100)) == NULL ||
	   (env = cd_basic(command, env)) == NULL)
    return (NULL);
  return (env);
}
