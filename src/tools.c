/*
** tools.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed May 18 22:07:31 2016 Baptiste veyssiere
** Last update Mon May 23 20:49:29 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include <unistd.h>

int	my_strlen(char *s)
{
  int	i;

  i = -1;
  if (!s)
    return (0);
  while (s[++i]);
  return (i);
}

int	my_perror(char *str)
{
  write(2, str, my_strlen(str));
  return (-1);
}

void	my_strcpy(char *s1, char *s2)
{
  int   i;

  i = -1;
  while (s1[++i])
    s2[i] = s1[i];
  s2[i] = 0;
}

int	my_strcmp(char *s1, char *s2)
{
  int   i;

  i = -1;
  while (s1[++i])
    {
      if (s1[i] != s2[i])
        return (0);
    }
  return (1);
}
