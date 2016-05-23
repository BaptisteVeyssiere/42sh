/*
** get_tree.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed May 18 21:45:30 2016 Baptiste veyssiere
** Last update Mon May 23 21:42:04 2016 Baptiste veyssiere
*/

#include <unistd.h>
#include <stdlib.h>
#include "mysh.h"

int		get_tree(char *str, char ***env)
{
  char		**tab;
  t_tree	**tree;
  int		i;
  int		j;
  int		check;

  check = 0;
  tree = NULL;
  if ((tab = get_command_tab(str)) == NULL)
    return (-1);
  if (!(tree = get_and_or_tree(tab, &check)) && !check)
    {
      free_wordtab(tab);
      return (-1);
    }
  else if (check)
    return (0);
  i = -1;
  while (tree[++i])
    {
      j = -1;
      while (tree[i]->and_or[++j])
	if (sort_tree(tree[i]->and_or[j]->command, *env))
	  return (0);
    }
  free_wordtab(tab);
  execute_tree(tree, env);
  free_tree(tree);
  return (0);
}
