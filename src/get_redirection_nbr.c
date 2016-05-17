/*
** get_redirection_nbr.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2/test
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed Apr  6 23:55:45 2016 Baptiste veyssiere
** Last update Tue Apr 12 00:17:41 2016 Baptiste veyssiere
*/

#include <unistd.h>
#include "mysh.h"

int	check_missing_red(char *s)
{
  int	i;
  int	key;

  key = 0;
  i = -1;
  while (s[++i])
    if ((s[i] == '>' && s[i + 1] != '>') ||
	(s[i] == '>' && s[i + 1] == '>' && s[i + 2] != '>') ||
	(s[i] == '<' && s[i + 1] != '<') ||
	(s[i] == '<' && s[i + 1] == '<' && s[i + 2] != '<'))
      {
	if ((s[i - 1] == '>' && s[i] == '>' && s[i + 1] == '>') ||
	    (s[i - 1] == '<' && s[i] == '<' && s[i + 1] == '<'))
	    return (1);
	if ((s[i] == '>' && s[i + 1] == '>') ||
	    (s[i] == '<' && s[i + 1] == '<'))
	  ++i;
	if (++key > 1)
	  return (1);
      }
    else if (key == 1 && s[i] && s[i] != ' ' && s[i] != '\t' &&
	     s[i] != '<' && s[i] != '>')
      key = 0;
  return (0);
}

int	get_left_redirection_nbr(char *s)
{
  int	i;
  int	nbr;

  i = -1;
  nbr = 0;
  if (check_missing_red(s) == 1)
    return (my_perror("Missing name for redirect.\n"));
  while (s[++i])
    {
      if ((s[i] == '<' && s[i + 1] != '<') ||
	  (s[i] == '<' && s[i + 1] == '<' && s[i + 2] != '<'))
	{
	  if (s[i + 1] == '<')
	    ++i;
	  ++nbr;
	}
    }
  if (nbr > 1)
    write(1, "Ambiguous input redirect.\n",
	  my_strlen("Ambiguous input redirect.\n"));
  return (nbr);
}

int	get_right_redirection_nbr(char *s)
{
  int   i;
  int   nbr;

  i = -1;
  nbr = 0;
  while (s[++i])
    {
      if ((s[i] == '>' && s[i + 1] != '>') ||
          (s[i] == '>' && s[i + 1] == '>' && s[i + 2] != '>'))
        {
          if (s[i + 1] == '>')
            ++i;
          ++nbr;
        }
    }
  if (nbr > 1)
    write(1, "Ambiguous input redirect.\n",
	  my_strlen("Ambiguous input redirect.\n"));
  return (nbr);
}
