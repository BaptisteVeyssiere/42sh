/*
** alias_tools.c for sh in /home/scutar_n/rendu/PSU/PSU_2015_42sh/src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun  5 03:13:20 2016 Nathan Scutari
** Last update Sun Jun  5 03:20:12 2016 Nathan Scutari
*/

#include <stdlib.h>
#include <unistd.h>
#include "mysh.h"

int	alias_length(char *str)
{
  int	i;

  i = -1;
  while (str[++i] && str[i] != ' ' && str[i] != ';' && str[i] != '\t' &&
	 str[i] != '|' && str[i] != '&');
  return (i);
}

char	*copy_str(char *str)
{
  int	x;
  char	*cpy;

  if ((cpy = malloc(my_strlen(str) + 1)) == NULL)
    return (NULL);
  x = -1;
  while (str[++x])
    cpy[x] = str[x];
  cpy[x] = 0;
  return (cpy);
}

void	free_alias(t_alias *list)
{
  t_alias	*tmp;

  while (list)
    {
      tmp = list;
      list = list->next;
      if (tmp->source >= 0)
	close(tmp->source);
      free(tmp->alias);
      free(tmp->equivalent);
      free(tmp);
    }
}

int	cut_alias(char *str, int save, int to, char **alias)
{
  int	x;

  if (to < save)
    {
      if ((*alias = malloc(1)) == NULL)
	return (1);
      (*alias)[0] = '\0';
      return (0);
    }
  if ((*alias = malloc(to - save + 2)) == NULL)
    return (1);
  x = -1;
  while (save + ++x <= to)
    (*alias)[x] = str[x + save];
  (*alias)[x] = 0;
  return (0);
}
