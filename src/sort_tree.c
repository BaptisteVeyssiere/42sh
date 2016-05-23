/*
** sort_tree.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2/test
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed Apr  6 23:16:46 2016 Baptiste veyssiere
** Last update Mon May 23 20:33:01 2016 Baptiste veyssiere
*/

#include <unistd.h>
#include <stdlib.h>
#include "mysh.h"

int	fill_node(t_interpipe *command, char prev, char pipe)
{
  int	nbr;

  nbr = get_left_redirection_nbr(command->str);
  if (nbr != 0 && prev == 1)
    return (my_perror("Ambiguous input redirect.\n"));
  else if (nbr > 1 || nbr < 0)
    return (-1);
  if (nbr == 1)
    {
      command->fd_input = get_fd_input(command);
      if (command->fd_input == -1)
        return (-1);
    }
  nbr = get_right_redirection_nbr(command->str);
  if (nbr != 0 && pipe == 1)
    return (my_perror("Ambiguous output redirect.\n"));
  else if (nbr > 1 || nbr < 0)
    return (-1);
  if (nbr == 1)
    {
      command->fd_output = get_fd_output(command);
      if (command->fd_output == -1)
        return (-1);
    }
  return (0);
}

int	sort_node(t_interpipe *command, char pipe, char prev)
{
  command->fd_input = 0;
  command->fd_output = 1;
  command->if_double_left = 0;
  command->if_double_right = 0;
  command->double_left_end = NULL;
  if (pipe == 1)
    command->pipe = 1;
  else
    command->pipe = 0;
  if (prev == 1)
    command->prev = 1;
  else
    command->prev = 0;
  if (fill_node(command, prev, pipe) == -1)
    return (-1);
  if (get_epure_str_length(command->str) == 0)
    return (my_perror("Invalid null command.\n"));
  return (0);
}

int	sort_tree(t_interpipe **command, char **env)
{
  int	i;
  char	pipe;
  char	prev;

  i = -1;
  while (command[++i])
    {
      pipe = 0;
      prev = 0;
      if (command[i + 1])
	pipe = 1;
      if (i > 0)
	prev = 1;
      if (sort_node(command[i], pipe, prev) == -1)
	return (-1);
      if (!(command[i]->args = check_instruction(command[i]->str, env)))
	return (-1);
    }
  return (0);
}
