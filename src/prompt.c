/*
** prompt.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Tue Mar 29 19:14:47 2016 Baptiste veyssiere
** Last update Sat May 28 18:57:45 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include <unistd.h>
#include "mysh.h"

void	free_prompt(char **prompt)
{
  if (my_strcmp_strict(prompt[0], "pwd") == 0)
    free(prompt[0]);
  free(prompt);
}

char	**get_prompt()
{
  char	*host;
  char	**prompt;
  char	*buf;

  buf = NULL;
  if ((host = getcwd(buf, 100)) == NULL)
    return (NULL);
  if ((prompt = malloc(sizeof(char*) * 2)) == NULL)
    return (NULL);
  prompt[0] = host;
  prompt[1] = NULL;
  return (prompt);
}

void	aff_prompt(char **prompt)
{
  int	i;

  i = my_strlen(prompt[0]);
  while (--i > 0 && prompt[0][i] != 47);
  while (--i > 0 && prompt[0][i] != 47);
  if (i < 0)
    i = 0;
  if (i > 0)
    ++i;
  write(1, &(prompt[0][i]), my_strlen(&(prompt[0][i])));
  write(1, "> ", 2);
}
