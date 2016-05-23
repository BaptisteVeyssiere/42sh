/*
** free_tools.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Mon May 23 18:39:33 2016 Baptiste veyssiere
** Last update Mon May 23 20:53:15 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include "mysh.h"

void	free_wordtab(char **tab)
{
  int   i;

  i = -1;
  while (tab[++i])
    free(tab[i]);
  free(tab);
}

static void	free_tree_command(t_interpipe **command)
{
  int		i;

  i = -1;
  while (command[++i])
    {
      free(command[i]->str);
      free_wordtab(command[i]->args);
      if (command[i]->double_left_end)
	free(command[i]->double_left_end);
      free(command[i]);
    }
  free(command);
}

void	free_tree(t_tree **tree)
{
  int   i;
  int   j;

  i = -1;
  while (tree[++i])
    {
      j = -1;
      while (tree[i]->and_or[++j])
        {
          free(tree[i]->and_or[j]->str);
	  free_tree_command(tree[i]->and_or[j]->command);
          free(tree[i]->and_or[j]);
        }
      free(tree[i]->and_or);
      free(tree[i]);
    }
  free(tree);
}
