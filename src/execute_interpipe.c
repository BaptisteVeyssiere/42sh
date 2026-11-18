/*
** Run one and/or component, forking a process per pipeline stage.
** State-changing builtins stay in the shell only when they are alone.
*/

#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "mysh.h"

static int	close_pipes(int (*pipes)[2], int count)
{
  int		i;

  i = -1;
  while (++i < count)
    {
      if (pipes[i][0] > 2)
	close(pipes[i][0]);
      if (pipes[i][1] > 2)
	close(pipes[i][1]);
    }
  return (0);
}

static void	child_setup(int (*pipes)[2], int count, int index)
{
  int		i;

  if (index > 0 && dup2(pipes[index - 1][0], 0) == -1)
    exit(1);
  if (index < count && dup2(pipes[index][1], 1) == -1)
    exit(1);
  i = -1;
  while (++i < count)
    {
      close(pipes[i][0]);
      close(pipes[i][1]);
    }
}

static int	wait_children(int *pid, int count, int fallback)
{
  int		i;
  int		ret;

  ret = fallback;
  i = -1;
  while (++i < count)
    {
      if (pid[i] > 0 && check_status(pid[i], &ret) == -1)
	return (-1);
    }
  return (ret);
}

static int	run_parent(t_command *and_or, t_datas *data)
{
  return (exec_builtins(and_or->command[0]->args, &data->env, data));
}

int	execute_interpipe(t_command *and_or, t_datas *data)
{
  int		count;
  int		pipes_n;
  int		(*pipes)[2];
  int		*pid;
  int		i;
  int		ret;

  count = and_or->pipe_nbr + 1;
  if (count == 1 && is_builtin(and_or->command[0], 0))
    return (run_parent(and_or, data));
  pipes_n = count - 1;
  pipes = NULL;
  pid = malloc(sizeof(int) * (size_t)count);
  if (!pid || (pipes_n && !(pipes = malloc(sizeof(*pipes) * (size_t)pipes_n))))
    {
      free(pid);
      return (-1);
    }
  i = -1;
  while (++i < count)
    pid[i] = 0;
  i = -1;
  while (++i < pipes_n)
    {
      if (pipe(pipes[i]) == -1)
	{
	  close_pipes(pipes, i);
	  free(pipes);
	  free(pid);
	  return (my_int_perror("Error while using pipe function.\n", -1));
	}
    }
  i = -1;
  while (++i < count)
    {
      data->tmp = i;
      pid[i] = fork();
      if (pid[i] == -1)
	{
	  close_pipes(pipes, pipes_n);
	  free(pipes);
	  free(pid);
	  return (my_int_perror("Error while using fork function.\n", -1));
	}
      if (pid[i] == 0)
	{
	  signal(SIGINT, SIG_DFL);
	  signal(SIGQUIT, SIG_DFL);
	  if (pipes_n)
	    child_setup(pipes, pipes_n, i);
	  exit(do_instruction(and_or, i, data));
	}
    }
  close_pipes(pipes, pipes_n);
  ret = wait_children(pid, count, 0);
  free(pipes);
  free(pid);
  return (ret);
}
