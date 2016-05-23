/*
** epure_str.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed May 18 22:08:49 2016 Baptiste veyssiere
** Last update Sat May 21 09:56:03 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include <unistd.h>

static int	get_new_length(char *str)
{
  int		i;
  int		length;

  i = -1;
  length = 0;
  while (str[++i] && (str[i] == ' ' || str[i] == '\t'));
  --i;
  while (str[++i])
    if ((str[i] != ' ' && str[i] != '\t') ||
	((str[i] == ' ' || str[i] == '\t') &&
	 (str[i + 1] && str[i + 1] != ' ' && str[i + 1] != '\t')))
      ++length;
  return (length);
}

char	*epure_str(char *old)
{
  int	new_length;
  char	*new;
  int	i;
  int	j;

  new_length = get_new_length(old);
  if (!(new = malloc(new_length + 1)))
    return (NULL);
  i = -1;
  while (++i <= new_length)
    new[i] = 0;
  i = -1;
  j = -1;
  while (old[++i] && (old[i] == ' ' || old[i] == '\t'));
  --i;
  while (old[++i])
    if ((old[i] != ' '&& old[i] != '\t') ||
        ((old[i] == ' ' || old[i] == '\t') &&
         (old[i + 1] && old[i + 1] != ' ' && old[i + 1] != '\t')))
      new[++j] = old[i];
  free(old);
  return (new);
}

int	get_epure_str_length(char *str)
{
  int   i;
  int   length;

  i = -1;
  while (str[++i] && (str[i] == '\t' || str[i] == ' '));
  if (str[i] == 0)
    return (0);
  length = 0;
  while (str[++i])
    ++length;
  return (length);
}
