/*
** get_and_or_tree_bis.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Mon May 23 20:53:44 2016 Baptiste veyssiere
** Last update Mon May 23 20:56:09 2016 Baptiste veyssiere
*/

#include <unistd.h>
#include <stdlib.h>
#include "mysh.h"

static int	get_and_or_command_length(char *tabi, int pos)
{
  int           length;
  int           i;

  length = 0;
  i = pos;
  if (tabi[i + 1] == ' ')
    ++i;
  while (tabi[++i] && (tabi[i] != '|' || tabi[i + 1] != '|') &&
         (tabi[i] != '&' || tabi[i + 1] != '&'))
    ++length;
  if (i > 0 && tabi[i - 1] == ' ')
    --length;
  return (length);
}

t_and_or	*get_and_or_command(char *tabi, int *pos, int *check)
{
  int           i;
  t_and_or      *and_or;
  int           length;
  char          **tab;

  i = -1;
  if (!(and_or = malloc(sizeof(t_and_or))))
    return (NULL);
  and_or->and = 0;
  and_or->or = 0;
  if (*pos >= 0 && tabi[*pos])
    {
      if (tabi[*pos] == '|')
        and_or->or = 1;
      if (tabi[*pos] == '&')
        and_or->and = 1;
    }
  length = get_and_or_command_length(tabi, *pos);
  if (!length && *pos >= 0)
    {
      *check = 1;
      my_perror("Invalid null command.\n");
      return (NULL);
    }
  if (!(and_or->str = malloc(length + 1)))
    return (NULL);
  and_or->str[length] = 0;
  if ((*pos == -1 || (tabi[*pos])) && tabi[*pos + 1] == ' ')
    ++(*pos);
  while (++i < length)
    and_or->str[i] = tabi[++(*pos)];
  ++(*pos);
  if (tabi[*pos])
    while (tabi[++(*pos)] && (tabi[*pos] == '|' || tabi[*pos] == '&'));
  if (tabi[*pos] && tabi[*pos + 1] != ' ')
    --(*pos);
  and_or->pipe_nbr = get_pipe_nbr(and_or->str);
  if (!(tab = get_red_tab(and_or->str)) ||
      !(and_or->command = get_interpipe_tab(tab)))
    return (NULL);
  free_wordtab(tab);
  return (and_or);
}
