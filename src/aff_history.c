/*
** aff_history.c for 42sh in /home/vigner_g/rendu/prog_shell/PSU_2015_42sh
**
** Made by vigner_g
** Login   <vigner_g@epitech.net>
**
** Started on  Thu Jun  2 11:30:25 2016 vigner_g
** Last update Thu Jun  2 11:53:34 2016 vigner_g
*/

#include	<stdio.h>
#include	<stdlib.h>
#include	"mysh.h"

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
