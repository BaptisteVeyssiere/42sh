/*
** execute_interpipe.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Tue Apr 12 00:32:26 2016 Baptiste veyssiere
** Last update Tue Apr 12 23:42:56 2016 Baptiste veyssiere
*/

#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include "mysh.h"

int	close_fd(int index, int fd, int *fildes[2], int length)
{
  int   i;
  int   j;

  i = -1;
  while (++i < length)
    {
      j = -1;
      while (++j < 2)
        {
          if (i != index || j != fd)
            if (close(fildes[i][j]) == -1)
              return (-1);
        }
    }
  return (0);
}

int	pipe_fd(t_command *command, int **fildes, int i)
{
  int	j;
  int	k;

  j = -1;
  while (++j < command->pipe_nbr)
    {
      k = -1;
      while (++k < 2)
	if ((command->command[i]->pipe == 0 || (j != i || k != 1)) &&
	    (command->command[i]->prev == 0 || (j != (i - 1) || k != 0)))
	  if (close(fildes[j][k]) == -1)
	    return (-1);
    }
  if (command->command[i]->prev)
    {
      if (dup2(fildes[i - 1][0], 0) == -1)
	return (my_perror("error on dup2\n"));
    }
  if (command->command[i]->pipe)
    {
      if (dup2(fildes[i][1], 1) == -1)
	return (my_perror("error on dup2\n"));
    }
  return (0);
}

int	wait_loop(t_command *command, int i, int **fildes, int *pid)
{
  int	status;

  if (is_builtin(command, i, 1) == 1)
    {
      status = 0;
      if (waitpid(pid[i], &status, WUNTRACED | WCONTINUED) < 0 ||
	  status == 11 || WTERMSIG(status) == SIGSEGV)
	{
	  write(1, "Segmentation fault\n", 19);
	  return (-1);
	}
      if (command->command[i]->pipe &&
	  close(fildes[i][1]) == -1)
	return (my_perror("error on close\n"));
      if (command->command[i]->prev &&
	  close(fildes[i - 1][0]) == -1)
	return (my_perror("error on close\n"));
    }
  return (0);
}

int	execute_loop(t_command *command, char ***env, int **fildes, int *pid)
{
  int	i;

  i = 0;
  while (++i <= command->pipe_nbr)
    if (pipe(fildes[i - 1]) == -1)
      return (my_perror("error on pipe\n"));
  while (--i >= 0)
    if (is_builtin(command, i, 0) == 1)
      exec_builtins(command->command[i]->args, env);
    else
      {
	if ((pid[i] = fork()) == -1)
	  return (my_perror("error on fork\n"));
	if (pid[i] == 0)
	  {
	    if (pipe_fd(command, fildes, i) == -1 ||
		do_instruction(command, env, i) == -1)
	      return (-1);
	    return (0);
	  }
      }
  while (++i <= command->pipe_nbr)
    if (wait_loop(command, i, fildes, pid) == -1)
      return (-1);
  return (0);
}

int	execute_interpipe(t_command *command, char ***env)
{
  int   i;
  int   **fildes;
  int   *pid;

  if (command == NULL || command->command[0] == NULL)
    return (0);
  if (((command->pipe_nbr &&
        (fildes = malloc(sizeof(int*) * command->pipe_nbr)) == NULL)) ||
      (pid = malloc(sizeof(int) * (command->pipe_nbr + 1))) == NULL)
    return (-1);
  i = -1;
  while (++i < command->pipe_nbr)
    if ((fildes[i] = malloc(sizeof(int) * 2)) == NULL)
      return (-1);
  if (execute_loop(command, env, fildes, pid) == -1)
    return (-1);
  if (command->pipe_nbr)
    {
      i = -1;
      while (++i < command->pipe_nbr)
        free(fildes[i]);
      free(fildes);
    }
  free(pid);
  return (0);
}
