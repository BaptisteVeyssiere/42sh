/*
** get_red_tab.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2/test
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed Apr  6 17:14:13 2016 Baptiste veyssiere
** Last update Sat May 21 14:05:23 2016 Baptiste veyssiere
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	get_red_nbr(char *red)
{
  int	nbr;
  int	i;

  nbr = 1;
  i = -1;
  if (red == NULL || red[0] == 0)
    return (0);
  while (red[++i])
    if (red[i] == '|')
      ++nbr;
  return (nbr);
}

int	get_red_length(char *red, int i)
{
  int	j;

  j = -1;
  while (red[i + ++j] && red[i + j] != '|');
  if ((i + j) > 0 && red[i + j - 1] == ' ')
    --j;
  return (j);
}

char	*get_red(char *red, int *i)
{
  int		j;
  char		*str;
  int		length;

  while (red[++(*i)] && (red[*i] == '\t' || red[*i] == ' '));
  j = -1;
  length = get_red_length(red, *i);
  --(*i);
  if (!(str = malloc(length + 1)))
    return (NULL);
  str[length] = 0;
  while (++j < length)
    str[j] = red[++(*i)];
  if (red[*i] && red[*i] != '|')
    while (red[++(*i)] && red[*i] != '|');
  return (str);
}

char	**get_red_tab(char *red)
{
  int	red_nbr;
  int	i;
  int	index;
  char	**tab;

  index = -1;
  red_nbr = get_red_nbr(red);
  if ((tab = malloc(sizeof(char*) * (red_nbr + 1))) == NULL)
    return (NULL);
  tab[red_nbr] = NULL;
  i = -1;
  while (++i < red_nbr)
    if ((tab[i] = get_red(red, &index)) == NULL)
      return (NULL);
  return (tab);
}
