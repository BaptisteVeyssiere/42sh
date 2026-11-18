/*
** aff_history.c for 42sh in /home/vigner_g/rendu/prog_shell/PSU_2015_42sh
**
** Made by vigner_g
** Login   <vigner_g@epitech.net>
**
** Started on  Thu Jun  2 11:30:25 2016 vigner_g
** Last update Sun Jun  5 17:31:32 2016 vigner_g
*/

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	"mysh.h"

static int	digits_only(const char *str)
{
  int		i;

  if (!str || !str[0])
    return (0);
  i = -1;
  while (str[++i])
    if (str[i] < '0' || str[i] > '9')
      return (0);
  return (1);
}

int		ret_history(t_datas *data, t_history *history, char *str)
{
  int		count;
  int		index;
  int		number;
  t_history	*tmp;
  char		*spec;

  if (!str || str[0] != '!')
    return (my_int_perror("Event not found\n", 1));
  spec = str + 1;
  count = 0;
  tmp = history;
  while (tmp != NULL)
    {
      ++count;
      tmp = tmp->next;
    }
  if (!count)
    return (my_int_perror("Event not found\n", 1));
  if ((spec[0] == '!' && spec[1] == '\0') || spec[0] == '\0')
    index = count;
  else if (digits_only(spec))
    index = my_getnbr(spec);
  else
    return (my_int_perror("Event not found\n", 1));
  if (index < 1 || index > count)
    return (my_int_perror("Event not found\n", 1));
  tmp = history;
  number = count;
  while (tmp != NULL && number != index)
    {
      tmp = tmp->next;
      --number;
    }
  if (tmp == NULL)
    return (my_int_perror("Event not found\n", 1));
  return (execute_command(data, tmp->command));
}

int		aff_history(t_history *history)
{
  t_history	*tmp;
  int		i;

  i = 1;
  tmp = history;
  while (tmp != NULL && tmp->next != NULL)
    tmp = tmp->next;
  while (tmp != NULL)
    {
      if ((printf("%i %s\n", i, tmp->command)) == -1)
	return (-1);
      tmp = tmp->prev;
      i += 1;
    }
  return (0);
}
