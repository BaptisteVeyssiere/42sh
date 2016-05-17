/*
** tools_bis.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2/src
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Sat Apr  9 23:51:10 2016 Baptiste veyssiere
** Last update Sat Apr  9 23:53:00 2016 Baptiste veyssiere
*/

#include <unistd.h>
#include "mysh.h"

int	my_perror_function(char *err_msg)
{
  write(2, err_msg, my_strlen(err_msg));
  write(2, ": Command not found.\n", my_strlen(": Command not found.\n"));
  return (-1);
}

int	my_strcmp_strict(char *s1, char *s2)
{
  int   i;

  i = -1;
  while (s1[++i])
    if (s1[i] != s2[i])
      return (0);
  if (s2[i])
    return (0);
  return (1);
}

int	my_tablen(char **tab)
{
  int   i;

  i = -1;
  while (tab[++i] != NULL);
  return (i);
}

void	my_puterr(char *s)
{
  write(2, s, my_strlen(s));
}

int	my_getnbr(char *str)
{
  int   i;
  int   j;
  int   limit;
  int   nbr;

  i = 0;
  nbr = 0;
  limit = 0;
  if (str[0] == '-')
    limit = 1;
  while (str[i] != 0)
    i++;
  i--;
  j = 1;
  while (i >= limit)
    {
      nbr += j * (str[i--] - 48);
      j *= 10;
    }
  if (str[0] == '-')
    nbr *= -1;
  return (nbr);
}
