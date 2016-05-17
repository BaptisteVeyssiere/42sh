/*
** get_interpipe_tab.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2/test
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed Apr  6 17:19:37 2016 Baptiste veyssiere
** Last update Tue Apr 12 16:19:35 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include <unistd.h>
#include "mysh.h"

t_interpipe	*get_interpipe_struct(char *s)
{
  t_interpipe	*interpipe;

  if ((interpipe = malloc(sizeof(t_interpipe))) == NULL)
    return (NULL);
  if ((interpipe->str = malloc(sizeof(char) * (my_strlen(s) + 1))) == NULL)
    return (NULL);
  interpipe->str[my_strlen(s)] = 0;
  my_strcpy(s, interpipe->str);
  return (interpipe);
}

t_interpipe	**get_interpipe_tab(char **tab)
{
  t_interpipe	**interpipe;
  int		i;

  i = -1;
  while (tab[++i] != NULL);
  if ((interpipe = malloc(sizeof(t_interpipe*) * (i + 1))) == NULL)
    return (NULL);
  interpipe[i] = NULL;
  i = -1;
  while (tab[++i] != NULL)
    if ((interpipe[i] = get_interpipe_struct(tab[i])) == NULL)
      return (NULL);
  return (interpipe);
}
