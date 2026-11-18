/*
** Collect a child status and map fatal signals to shell statuses.
*/

#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "mysh.h"

static int	report_signal(int sig, int status)
{
  const char	*text;
  int		len;

  if (sig == SIGINT || sig == SIGPIPE || sig == SIGQUIT)
    return (0);
  if (sig == SIGSEGV)
    text = "Segmentation fault";
  else if (sig == SIGFPE)
    text = "Floating exception";
  else
    return (0);
  len = my_strlen((char *)text);
  if (write(2, text, (size_t)len) == -1)
    return (0);
  if (WCOREDUMP(status) && write(2, " (core dumped)", 14) == -1)
    return (0);
  write(2, "\n", 1);
  return (0);
}

int	check_status(int pid, int *ret)
{
  int		status;
  pid_t		waited;
  int		sig;

  status = 0;
  while (1)
    {
      waited = waitpid(pid, &status, 0);
      if (waited == -1 && errno == EINTR)
	continue;
      if (waited == -1)
	return (-1);
      break;
    }
  if (WIFEXITED(status))
    {
      *ret = WEXITSTATUS(status);
      return (0);
    }
  if (WIFSIGNALED(status))
    {
      sig = WTERMSIG(status);
      *ret = 128 + sig;
      report_signal(sig, status);
    }
  return (0);
}
