/*
** tools.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2/test
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed Apr  6 18:58:07 2016 Baptiste veyssiere
** Last update Tue Apr 12 17:15:29 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include <unistd.h>

void	my_strcpy(char *s1, char *s2)
{
  int   i;

  i = -1;
  while (s1[++i])
    s2[i] = s1[i];
  s2[i] = 0;
}

void	free_wordtab(char **tab)
{
  int   i;

  i = -1;
  while (tab[++i] != NULL)
    free(tab[i]);
  free(tab);
}

int	my_strlen(char *s)
{
  int	i;

  if (s == NULL)
    return (0);
  i = -1;
  while (s[++i]);
  return (i);
}

int	my_perror(char *str)
{
  write(2, str, my_strlen(str));
  return (-1);
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
