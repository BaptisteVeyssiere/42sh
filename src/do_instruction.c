/*
** Child-side execution: redirections, builtins, then execve.
*/

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "mysh.h"

static int	change_fd_on(t_command *command, int i)
{
  if (command->command[i]->fd_input != 0)
    {
      if (close(0) == -1 || dup(command->command[i]->fd_input) == -1)
	return (-1);
    }
  if (command->command[i]->fd_output != 1)
    {
      if (close(1) == -1 || dup(command->command[i]->fd_output) == -1)
	return (-1);
    }
  return (0);
}

static void	exec_error(char *path)
{
  const char	*msg;
  int		code;

  if (errno == ENOENT)
    {
      msg = ": Command not found.\n";
      code = 127;
    }
  else if (errno == EACCES)
    {
      msg = ": Permission denied.\n";
      code = 126;
    }
  else if (errno == ENOEXEC)
    {
      msg = ": Exec format error. Binary file not executable.\n";
      code = 126;
    }
  else
    {
      msg = ": ";
      write(2, path, (size_t)my_strlen(path));
      write(2, msg, 2);
      msg = strerror(errno);
      write(2, msg, (size_t)my_strlen((char *)msg));
      write(2, "\n", 1);
      exit(126);
    }
  write(2, path, (size_t)my_strlen(path));
  write(2, msg, (size_t)my_strlen((char *)msg));
  exit(code);
}

int	do_instruction(t_command *and_or, int i, t_datas *data)
{
  int		ret;

  if (change_fd_on(and_or, i) == -1)
    return (1);
  if (is_shell_builtin(and_or->command[i]->args[0]))
    {
      ret = exec_builtins(and_or->command[i]->args, &data->env, data);
      if (shell_should_exit())
	return (shell_exit_code());
      return (ret < 0 ? 1 : ret);
    }
  if (!and_or->command[i]->args[0][0])
    {
      write(2, ": Command not found.\n", 21);
      return (127);
    }
  execve(and_or->command[i]->args[0], and_or->command[i]->args, data->env);
  exec_error(and_or->command[i]->args[0]);
  return (127);
}
