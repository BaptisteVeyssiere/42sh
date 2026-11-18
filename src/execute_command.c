/*
** execute_command.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed May 25 17:35:13 2016 Baptiste veyssiere
** Last update Thu Jun  9 00:44:19 2016 Baptiste veyssiere
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
      if ((command[i]->left_red || command[i]->if_double_left) &&
	  command[i]->fd_input > 2)
	ret = close(command[i]->fd_input);
      if ((command[i]->right_red || command[i]->if_double_right) &&
	  command[i]->fd_output > 2)
	ret = close(command[i]->fd_output);
    }
  if (ret == -1)
    return (-1);
  return (0);
}

static int	prepare_heredoc(t_interpipe **command)
{
  int		i;

  i = -1;
  while (command[++i])
    {
      if (command[i]->if_double_left &&
	  double_left_red(command[i]) == -1)
	return (1);
    }
  return (0);
}

static int	execute_and_or(t_command *and_or, t_datas *data)
{
  int		ret;

  if (!and_or->command || !and_or->command[0] ||
      !and_or->command[0]->args || !and_or->command[0]->args[0])
    return (0);
  ret = check_var(and_or->command, data->env, data->last_status);
  if (ret)
    return (ret < 0 ? -1 : 1);
  if (prepare_heredoc(and_or->command))
    {
      close_files(and_or->command);
      return (1);
    }
  if ((ret = open_fd(and_or->command)) != 0)
    {
      close_files(and_or->command);
      return (1);
    }
  if ((ret = check_and_add_path(and_or->command, data->env)))
    {
      close_files(and_or->command);
      return (ret);
    }
  ret = execute_interpipe(and_or, data);
  if (close_files(and_or->command) == -1)
    return (-1);
  return (ret);
}

static int	should_run(t_command *command, int index, int status)
{
  if (index == 0)
    return (1);
  if (command->and && status == 0)
    return (1);
  if (command->or && status != 0)
    return (1);
  return (0);
}

static int	execute_subtree(t_command **and_or, t_datas *data)
{
  int		ret;
  int		i;

  i = -1;
  ret = data->last_status;
  while (and_or[++i] && and_or[i]->command && and_or[i]->command[0] &&
	 and_or[i]->command[0]->args && and_or[i]->command[0]->args[0])
    {
      if (!should_run(and_or[i], i, data->last_status))
	continue;
      ret = execute_and_or(and_or[i], data);
      if (ret == -1)
	return (-1);
      data->last_status = ret;
      if (shell_should_exit())
	return (shell_exit_code());
    }
  return (ret);
}

static int	execute_tree(t_tree **tree, t_datas *data)
{
  int		i;
  int		error;

  i = -1;
  error = data->last_status;
  while (tree[++i])
    {
      error = execute_subtree(tree[i]->and_or, data);
      if (error == -1 || shell_should_exit())
	return (error);
    }
  return (error);
}

static int	remember(t_datas *data, char *command)
{
  if (!data->env)
    return (0);
  if (add_a_command(&data->history, command) == -1)
    return (-1);
  if (save_in_file(data, command) == -1)
    return (-1);
  return (0);
}

int		execute_command(t_datas *data, char *str)
{
  char		*aliased;
  char		*command;
  int		error;
  t_tree	**tree;

  tree = NULL;
  aliased = replace_by_alias(str, data->alias);
  if (!aliased)
    return (-1);
  command = epure_str(aliased);
  free(aliased);
  if (!command)
    return (-1);
  if (!command[0])
    {
      free(command);
      return (0);
    }
  if ((error = check_command(command)))
    {
      free(command);
      if (error < 0)
	return (-1);
      data->last_status = error;
      return (error);
    }
  if (remember(data, command) == -1 ||
      (error = get_tree(&tree, command)) == -1)
    {
      free(command);
      return (-1);
    }
  free(command);
  if (fill_leaf(tree) == -1)
    {
      free_tree(tree);
      return (-1);
    }
  error = execute_tree(tree, data);
  free_tree(tree);
  if (shell_should_exit())
    return (shell_exit_code());
  if (error < 0)
    return (-1);
  data->last_status = error;
  return (error);
}
