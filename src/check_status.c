/*
** check_status.c for 42sh in /home/vigner_g/rendu/prog_shell/PSU_2015_42sh
**
** Made by vigner_g
** Login   <vigner_g@epitech.net>
**
** Started on  Sun Jun  5 00:12:05 2016 vigner_g
** Last update Sun Jun  5 00:15:01 2016 vigner_g
*/

#include	<sys/wait.h>
#include	<unistd.h>
#include	"mysh.h"

int	check_status(int pid, int *ret)
{
  int		status;
  int		signal;

  status = 0;
  if (waitpid(pid, &status, WUNTRACED | WCONTINUED) == -1)
    return (-1);
  if (WIFEXITED(status))
    *ret = WEXITSTATUS(status);
  if (WIFSIGNALED(status))
    {
      signal = WTERMSIG(status);
      if (WCOREDUMP(status) && (*ret = 139) == 139 &&
	  write(1,"segmentation fault (core dumped)\n", 33) == -1)
	return (-1);
      else if (signal == SIGSEGV &&
	       (*ret = 139) == 139 && write(1, "Segmentation fault\n", 19) == -1)
	return (-1);
      else if (signal == SIGFPE &&
	       (*ret = 136) == 136 && write(1, "Floating exception\n", 19) == -1)
	return (-1);
    }
  return (0);
}
