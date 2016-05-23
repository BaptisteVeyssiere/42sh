/*
** get_command_tab.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed May 18 21:51:05 2016 Baptiste veyssiere
** Last update Wed May 18 22:17:29 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include <unistd.h>
#include "mysh.h"

static int    	get_limiter_nbr(char *str)
{
  int		counter;
  int		i;

  counter = 0;
  i = -1;
  while (str[++i])
    if (str[i] == ';')
      ++counter;
  return (counter);
}

static int	get_command_length(char *str, int pos)
{
  int		i;
  int		length;

  i = pos;
  length = 0;
  while (str[++i] && str[i] != ';')
    ++length;
  return (length);
}

static char	*get_command(char *str, int *pos)
{
  char		*tabi;
  int		length;
  int		i;

  length = get_command_length(str, *pos);
  if (!(tabi = malloc(length + 1)))
    return (NULL);
  tabi[length] = 0;
  i = -1;
  while (str[++(*pos)] && str[*pos] != ';')
    tabi[++i] = str[*pos];
  if (!(tabi = epure_str(tabi)))
    return (NULL);
  return (tabi);
}

char	**get_command_tab(char *str)
{
  char	**tab;
  int	limiter_nbr;
  int	pos;
  int	i;

  limiter_nbr = get_limiter_nbr(str);
  if (!(tab = malloc(sizeof(char*) * (limiter_nbr + 2))))
    return (NULL);
  tab[limiter_nbr + 1] = NULL;
  pos = -1;
  i = -1;
  while (++i <= limiter_nbr)
    if (!(tab[i] = get_command(str, &pos)))
      return (NULL);
  return (tab);
}
