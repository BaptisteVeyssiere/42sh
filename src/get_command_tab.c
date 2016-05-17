/*
** get_command_tab.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2/test
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed Apr  6 15:36:30 2016 Baptiste veyssiere
** Last update Wed Apr  6 16:21:13 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include <unistd.h>

int	get_command_nbr(char *command)
{
  int	nbr;
  int	i;

  nbr = 1;
  i = -1;
  if (command == NULL || command[0] == 0)
    return (0);
  while (command[++i])
    if (command[i] == ';')
      ++nbr;
  return (nbr);
}

int	get_command_length(char *command, int i)
{
  int	j;

  j = -1;
  while (command[i + ++j] && command[i + j] != ';');
  return (j);
}

char	*get_command(char *command, int *i)
{
  int		j;
  char		*str;
  int		length;

  while (command[++(*i)] && (command[*i] == '\t' || command[*i] == ' '));
  j = -1;
  length = get_command_length(command, *i);
  --(*i);
  if ((str = malloc(sizeof(char) * (length + 1))) == NULL)
    return (NULL);
  str[length] = 0;
  while (command[++(*i)] && command[*i] != ';')
    str[++j] = command[*i];
  return (str);
}

char	**get_command_tab(char *command)
{
  int	command_nbr;
  int	i;
  int	index;
  char	**tab;

  index = -1;
  command_nbr = get_command_nbr(command);
  if ((tab = malloc(sizeof(char*) * (command_nbr + 1))) == NULL)
    return (NULL);
  tab[command_nbr] = NULL;
  i = -1;
  while (++i < command_nbr)
    if ((tab[i] = get_command(command, &index)) == NULL)
      return (NULL);
  return (tab);
}
