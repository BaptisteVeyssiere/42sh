/*
** execute_command.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed May 25 17:35:13 2016 Baptiste veyssiere
** Last update Tue May 31 17:52:07 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include <unistd.h>
#include "mysh.h"

static int	execute_and_or(t_command *and_or, char ***env)
{
  int		ret;

  if ((ret = open_fd(and_or->command)) == -2)
    return (-1);
  else if (ret == -1)
    return (1);
  if ((ret = check_and_add_path(and_or->command, *env)))
    return (ret);
  return (execute_interpipe(and_or, env));
}

static int	execute_subtree(t_command **and_or, char ***env)
{
  char		ret;
  int		i;

  i = -1;
  ret = -1;
  while (and_or[++i])
    {
      if (ret == 0 && and_or[i]->or == 1)
	return (0);
      if (ret == -1 ||
	  (ret == 0 && and_or[i]->and == 1) ||
	  (ret == 1 && and_or[i]->or == 1))
	{
	  if ((ret = execute_and_or(and_or[i], env)) == -1)
	    return (-1);
	}
    }
  return (ret);
}

static int	execute_tree(t_tree **tree, char ***env)
{
  int		i;
  int		error;

  i = -1;
  while (tree[++i])
    {
      error = execute_subtree(tree[i]->and_or, env);
      if (error == -1)
	return (-1);
    }
  return (error);
}

int		execute_command(char *str, char ***env)
{
  char		*command;
  int		error;
  t_tree	**tree;

  if (!(command = epure_str(str)))
    return (-1);
  if (!command[0])
    {
      free(command);
      return (0);
    }
  tree = NULL;
  if ((error = get_tree(&tree, command)) == -1)
    return (-1);
  if (error == 1)
    return (0);
  free(command);
  if (fill_leaf(tree) == -1)
    return (-1);
  error = execute_tree(tree, env);
  free_tree(tree);
  return (error);
}
