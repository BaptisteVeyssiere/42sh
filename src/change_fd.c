/*
** change_fd.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Sun Apr 10 00:00:48 2016 Baptiste veyssiere
** Last update Tue Apr 12 16:18:37 2016 Baptiste veyssiere
*/

#include <unistd.h>
#include "mysh.h"

int	change_fd_on(int *fd_input, int *fd_output, t_command *command, int i)
{
  if (command->command[i]->fd_input != 0)
    {
      if ((*fd_input = dup(0)) == -1 || close(0) == -1 ||
	  dup(command->command[i]->fd_input) == -1)
	return (-1);
    }
  if (command->command[i]->fd_output != 1)
    {
      if ((*fd_output = dup(1)) == -1 ||
	  close(1) == -1 ||
	  dup(command->command[i]->fd_output)  == -1)
	return (-1);
    }
  if (*fd_input == -1 || *fd_output == -1)
    return (-1);
  return (0);
}
