/*
** aff_history.c for 42sh in /home/vigner_g/rendu/prog_shell/PSU_2015_42sh
**
** Made by vigner_g
** Login   <vigner_g@epitech.net>
**
** Started on  Thu Jun  2 11:30:25 2016 vigner_g
** Last update Sat Jun  4 14:31:31 2016 vigner_g
*/

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	"mysh.h"

int		ret_history(t_history *history, char *str)
{
  int		nb;
  int		i;
  t_history	*tmp;

  str += 1;
  i = 1;
  tmp = history;
  if (str != 0)
    {
      nb = my_getnbr(str);
      while (tmp != NULL && i != nb)
	{
	  tmp = tmp->next;
	  i += 1;
	}
      if (i != nb || tmp == NULL)
	return (my_int_perror("Event not found\n", 0));
      if (write(0, tmp->command, my_strlen(tmp->command)) == -1 ||
	  write(0, "\n", 1) == -1)
	return (-1);
    }
  return (0);
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
      printf("%i %s\n", i, tmp->command);
      tmp = tmp->prev;
      i += 1;
    }
  return (0);
}
