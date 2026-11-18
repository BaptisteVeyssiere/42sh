/*
** 42sh entry point.
**
**   42sh              interactive shell, or a script on stdin
**   42sh -c command   run one command line
**   42sh file         run a script file
*/

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include "mysh.h"
#include "get_next_line.h"

int		ctrc = 0;
int		g_interactive = 0;

void		handler(int sign)
{
  (void)sign;
  ctrc = 1;
  write(1, "\n", 1);
}

static void	usage(int fd)
{
  char		*msg;

  msg = "Usage: 42sh [file]\n"
    "       42sh -c command\n";
  write(fd, msg, (size_t)my_strlen(msg));
}

static int	init_shell(t_datas *data, char **env)
{
  data->history = NULL;
  data->alias = NULL;
  data->home = NULL;
  data->profile = NULL;
  data->fd = -1;
  data->env = NULL;
  data->tmp = 0;
  data->last_status = 0;
  if (env && env[0])
    {
      data->env = env_copy(env);
      if (!data->env)
	return (-1);
    }
  data->alias = load_alias(NULL, data->env, NULL);
  data->home = get_varenv(data->env, "HOME");
  if (data->home)
    data->history = load_history(data, data->home, "default", data->history);
  return (0);
}

static int	show_prompt(char ***prompt)
{
  if (*prompt)
    free_prompt(*prompt);
  *prompt = get_prompt();
  if (!*prompt || aff_prompt(*prompt) == -1)
    return (-1);
  return (0);
}

static int	repl(t_datas *data)
{
  char		*command;
  char		**prompt;
  int		ret;

  prompt = NULL;
  ret = 0;
  if (g_interactive && signal(SIGINT, &handler) == SIG_ERR)
    return (1);
  if (g_interactive && show_prompt(&prompt) == -1)
    return (1);
  while ((command = get_next_line(0)) != NULL || (g_interactive && ctrc))
    {
      ctrc = 0;
      if (command)
	{
	  ret = execute_command(data, command);
	  free(command);
	  if (shell_should_exit())
	    {
	      ret = shell_exit_code();
	      break;
	    }
	  if (ret < 0)
	    {
	      ret = 1;
	      break;
	    }
	  data->last_status = ret;
	}
      if (g_interactive && show_prompt(&prompt) == -1)
	{
	  ret = 1;
	  break;
	}
    }
  free_prompt(prompt);
  return (ret);
}

static int	run_file(t_datas *data, const char *path);

static int	run_multiline(t_datas *data, const char *command)
{
  char		path[] = "/tmp/42shcXXXXXX";
  int		fd;
  int		ret;
  size_t	len;

  fd = mkstemp(path);
  if (fd == -1)
    return (1);
  len = strlen(command);
  if ((len && write(fd, command, len) != (ssize_t)len) ||
      (len == 0) ||
      (command[len - 1] != '\n' && write(fd, "\n", 1) != 1))
    {
      close(fd);
      unlink(path);
      return (1);
    }
  close(fd);
  ret = run_file(data, path);
  unlink(path);
  return (ret);
}

static int	run_file(t_datas *data, const char *path)
{
  int		fd;
  int		saved;
  int		ret;

  fd = open(path, O_RDONLY);
  if (fd == -1)
    {
      write(2, "42sh: ", 6);
      write(2, path, (size_t)my_strlen((char *)path));
      write(2, ": No such file or directory.\n",
	    my_strlen(": No such file or directory.\n"));
      return (1);
    }
  saved = dup(0);
  if (saved == -1 || dup2(fd, 0) == -1)
    {
      close(fd);
      if (saved != -1)
	close(saved);
      return (1);
    }
  close(fd);
  g_interactive = 0;
  ret = repl(data);
  if (dup2(saved, 0) == -1)
    ret = 1;
  close(saved);
  return (ret);
}

int		main(int ac, char **av, char **env)
{
  t_datas	data;
  int		ret;

  if (ac == 2 && (!strcmp(av[1], "-h") || !strcmp(av[1], "--help")))
    {
      usage(1);
      return (0);
    }
  if (ac > 3 || (ac == 3 && strcmp(av[1], "-c") != 0) ||
      (ac == 2 && av[1][0] == '-'))
    {
      usage(2);
      return (2);
    }
  if (init_shell(&data, env) == -1)
    return (1);
  if (ac == 3)
    {
      g_interactive = 0;
      if (strchr(av[2], '\n'))
	ret = run_multiline(&data, av[2]);
      else
	{
	  ret = execute_command(&data, av[2]);
	  if (shell_should_exit())
	    ret = shell_exit_code();
	  else if (ret < 0)
	    ret = 1;
	}
    }
  else if (ac == 2)
    ret = run_file(&data, av[1]);
  else
    {
      g_interactive = isatty(0);
      ret = repl(&data);
    }
  if (shell_should_exit())
    ret = shell_exit_code();
  free_all(&data, NULL);
  get_next_line_clear();
  if (ret < 0)
    return (1);
  return (ret & 255);
}
