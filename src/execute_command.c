/*
** execute_command.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed May 25 17:35:13 2016 Baptiste veyssiere
** Last update Wed Jun  1 18:15:08 2016 vigner_g
*/

#include <stdlib.h>
#include <unistd.h>
#include "mysh.h"

static int	execute_and_or(t_command *and_or, char ***env, t_datas *data)
{
  int		ret;

  if ((ret = open_fd(and_or->command)) == -2)
    return (-1);
  else if (ret == -1)
    return (1);
  if ((ret = check_and_add_path(and_or->command, *env)))
    return (ret);
  return (execute_interpipe(and_or, env, data));
}

static int	execute_subtree(t_command **and_or, char ***env, t_datas *data)
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
	  if ((ret = execute_and_or(and_or[i], env, data)) == -1)
	    return (-1);
	}
    }
  return (0);
}

static int	execute_tree(t_tree **tree, char ***env, t_datas *data)
{
  int		i;
  int		error;

  i = -1;
  while (tree[++i])
    {
      error = execute_subtree(tree[i]->and_or, env, data);
      if (error == -1)
	return (-1);
    }
  return (0);
}

static int	free_and_ret(char *command)
{
  free(command);
  return (0);
}
/* cette fonction a été modifiée */

int		execute_command(t_datas *data, char *str, char ***env)
{
  char		*command;
  int		error;
  t_tree	**tree;

  if (!(command = epure_str(str)))
    return (-1);
  if (!command[0])
    return (free_and_ret(command));
  if ((data->history = add_a_command(data->history, command)) == NULL ||
      save_in_file(data->fd, command) == -1)
    return (-1);
  tree = NULL;
  if ((error = get_tree(&tree, command)) == -1)
    return (-1);
  if (error == 1)
    return (0);
  /* free(command); */
  if (fill_leaf(tree) == -1 ||
      execute_tree(tree, env, data) == -1)
    return (-1);
  free_tree(tree);
  return (0);
}
