/*
** str_to_wordtab.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Fri May 27 15:36:42 2016 Baptiste veyssiere
** Last update Fri May 27 23:44:33 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include <unistd.h>

static int	get_word_length(char *str, int pos)
{
  int		length;

  length = 0;
  while (str[++pos] && str[pos] != ' ' && str[pos] != '\t')
    ++length;
  return (length);
}

static int	get_word_nbr(char *str)
{
  int		nbr;
  int		i;

  i = -1;
  nbr = 0;
  while (str[++i])
    if (i == 0 || ((str[i] == ' ' || str[i] == '\t') &&
		   (str[i + 1] && str[i + 1] != ' ' && str[i + 1] != '\t')))
      ++nbr;
  return (nbr);
}

static char	*get_word_from_str(char *str, int *pos)
{
  int		i;
  int		length;
  char		*tabi;

  i = -1;
  length = get_word_length(str, *pos);
  if (!(tabi = malloc(length + 1)))
    return (NULL);
  tabi[length] = 0;
  while (++i < length)
    tabi[i] = str[++(*pos)];
  ++(*pos);
  return (tabi);
}

char	**str_to_wordtab(char *str)
{
  char	**tab;
  int	i;
  int	pos;
  int	word_nbr;

  word_nbr = get_word_nbr(str);
  if (!(tab = malloc(sizeof(char*) * (word_nbr + 1))))
    return (NULL);
  tab[word_nbr] = NULL;
  i = -1;
  pos = -1;
  while (++i < word_nbr)
    if (!(tab[i] = get_word_from_str(str, &pos)))
      return (NULL);
  return (tab);
}
