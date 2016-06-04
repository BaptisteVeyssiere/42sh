/*
** execute_command.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed May 25 17:35:13 2016 Baptiste veyssiere
** Last update Sat Jun  4 17:32:42 2016 vigner_g
*/

#include <stdlib.h>
#include <unistd.h>
#include "mysh.h"

static int	close_files(t_interpipe **command)
{
  int		i;
  int		ret;

  i = -1;
  ret = 0;
  while (command[++i])
    {
      if (command[i]->input_file)
	ret = close(command[i]->fd_input);
      if (command[i]->output_file)
	ret = close(command[i]->fd_output);
    }
  if (ret == -1)
    return (-1);
  return (0);
}

static int	execute_and_or(t_command *and_or, t_datas *data)
{
  int		ret;

  if ((ret = open_fd(and_or->command)) == -2)
    return (-1);
  else if (ret == -1)
    return (1);
  if ((ret = check_and_add_path(and_or->command, data->env)))
    return (ret);
  ret = execute_interpipe(and_or, data);
  if (close_files(and_or->command) == -1)
    return (-1);
  return (ret);
}

static int	execute_subtree(t_command **and_or, t_datas *data)
{
  char		ret;
  int		i;

  i = -1;
  ret = 0;
  while (and_or[++i] && and_or[i]->command[0] &&
	 and_or[i]->command[0]->args[0] && and_or[i]->command[0]->args[0][0])
    {
      if (ret == 0 && and_or[i]->or == 1)
	return (0);
      if ((!ret && !i) ||
	  (!ret && and_or[i]->and == 1) ||
	  (ret && and_or[i]->or == 1))
	{
	  if ((ret = execute_and_or(and_or[i], data)) == -1)
	    return (-1);
	}
    }
  return (ret);
}

static int	execute_tree(t_tree **tree, t_datas *data)
{
  int		i;
  int		error;

  i = -1;
  while (tree[++i])
    {
      error = execute_subtree(tree[i]->and_or, data);
      if (error == -1)
	return (-1);
    }
  if (check_exit(tree))
    {
      free_tree(tree);
      exit(error);
    }
  return (error);
}

static int	free_and_ret(char *command)
{
  free(command);
  return (0);
}

int		execute_command(t_datas *data, char *str)
{
  char		*command;
  int		error;
  t_tree	**tree;

  if (!(command = epure_str(str)))
    return (-1);
  if ((error = check_command(command)))
    return (error);
  if (!command[0])
    return (free_and_ret(command));
  if ((data->history = add_a_command(data->history, command)) == NULL ||
      save_in_file(data, command) == -1)
    return (-1);
  tree = NULL;
  if ((error = get_tree(&tree, command)) == -1)
    return (-1);
  if (error == 1)
    return (0);
  if (fill_leaf(tree) == -1)
    return (-1);
  error = execute_tree(tree, data);
  free_tree(tree);
  return (error);
}
