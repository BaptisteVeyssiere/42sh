/*
** execute_interpipe.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Sun May 29 01:09:08 2016 Baptiste veyssiere
** Last update Wed Jun  1 18:28:10 2016 vigner_g
*/

#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include "mysh.h"

static int	pipe_fd(t_command *command, int **fildes, int i)
{
  int		j;
  int		k;

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
	return (my_int_perror("Error while using dup2 function.\n", -1));
    }
  if (command->command[i]->pipe)
    {
      if (dup2(fildes[i][1], 1) == -1)
        return (my_int_perror("Error while using dup2 function.\n", -1));
    }
  return (0);
}

static int	wait_loop(t_interpipe *command, int *ret,
			  int **fildes, int *pid, int i)
{
  int		status;

  ret = ret;
  /* Changer ret en cas d'erreur (ne pas oublier) */
  if (is_builtin(command, 1) == 1)
    {
      status = 0;
      if (waitpid(pid[i], &status, WUNTRACED | WCONTINUED) < 0 ||
          status == 11 || WTERMSIG(status) == SIGSEGV)
        {
          write(1, "Segmentation fault\n", 19);
          return (-1);
	}
      if (command->pipe &&
          close(fildes[i][1]) == -1)
        return (my_int_perror("Error while using close function.\n", -1));
      if (command->prev &&
          close(fildes[i - 1][0]) == -1)
        return (my_int_perror("Error while using close function.\n", -1));
    }
  return (0);
}

static int	execute_loop(t_command *and_or, char ***env,
			     int **fildes, int *pid, t_datas *data)
{
  int		i;
  int		ret;

  i = 0;
  ret = 0;
  while (++i <= and_or->pipe_nbr)
    if (pipe(fildes[i - 1]) == -1)
      return (my_int_perror("Error while using pipe function.\n", -1));
  while (--i >= 0)
    if (is_builtin(and_or->command[i], 0) == 1 &&
	exec_builtins(and_or->command[i]->args, env, data))
      ret = 1;
    else if (is_builtin(and_or->command[i], 0) != 1)
      {
	if ((pid[i] = fork()) == -1)
	  return (my_int_perror("Error while using fork function.\n", -1));
	if (pid[i] == 0)
	  {
	    if (pipe_fd(and_or, fildes, i) == -1)
	      return (-1);
	    return (do_instruction(and_or, env, i, data));
	  }
      }
  while (++i <= and_or->pipe_nbr)
    if (wait_loop(and_or->command[i], &ret, fildes, pid, i) == -1)
      return (-1);
  return (ret);
}/* amener le data jusqu'a là */

int	execute_interpipe(t_command *and_or, char ***env, t_datas *data)
{
  int	i;
  int   **fildes;
  int   *pid;
  int	ret;

  if (((and_or->pipe_nbr &&
        !(fildes = malloc(sizeof(int*) * and_or->pipe_nbr)))) ||
      !(pid = malloc(sizeof(int) * (and_or->pipe_nbr + 1))))
    return (-1);
  i = -1;
  while (++i < and_or->pipe_nbr)
    if ((fildes[i] = malloc(sizeof(int) * 2)) == NULL)
      return (-1);
  if ((ret = execute_loop(and_or, env, fildes, pid, data)) == -1)
    return (-1);
  if (and_or->pipe_nbr)
    {
      i = -1;
      while (++i < and_or->pipe_nbr)
        free(fildes[i]);
      free(fildes);
    }
  free(pid);
  return (ret);
}/* amenier le data jusque là */
