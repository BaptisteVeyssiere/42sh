/*
** check_if_exist.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Fri Apr  1 01:41:26 2016 Baptiste veyssiere
** Last update Mon May 23 20:26:23 2016 Baptiste veyssiere
*/

#include <unistd.h>
#include <stdlib.h>
#include "mysh.h"

char	*test_path(char *copy, char *tabi)
{
  char	*str;
  int	i;
  int	j;

  if ((str = malloc(sizeof(char) *
		    (my_strlen(copy) + my_strlen(tabi) + 1))) == NULL)
    return (NULL);
  i = -1;
  while (tabi[++i])
    str[i] = tabi[i];
  j = -1;
  --i;
  while (copy[++j])
    str[++i] = copy[j];
  str[++i] = 0;
  return (str);
}

char	*add_path(char **tab, char **command)
{
  int	i;
  char	*copy;

  i = -1;
  if (slash_test(command[0]) == 1 &&
      access(command[0], F_OK) != 0)
    return (command[0]);
  while (tab[++i] != NULL)
    {
      if ((copy = test_path(command[0], tab[i])) == NULL)
	return (NULL);
      if (access(copy, F_OK) == 0)
	{
	  free(command[0]);
	  return (copy);
	}
    }
  return (command[0]);
}

int	check_if_exist(char **command, char **env)
{
  char	*path;
  char	**tab;

  if (env == NULL)
    {
      if ((path = malloc(sizeof(char) * (my_strlen("/bin") + 1))) == NULL)
	return (-1);
      path[my_strlen("/bin")] = 0;
      my_strcpy("/bin", path);
    }
  else if ((path = get_varenv(env, "PATH")) == NULL)
    return (0);
  if ((tab = get_varpath(path)) == NULL)
    return (-1);
  if ((command[0] = add_path(tab, command)) == NULL)
    return (-1);
  free_wordtab(tab);
  free(path);
  return (0);
}
