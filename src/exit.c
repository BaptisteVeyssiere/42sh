/*
** exit builtin. A valid exit is recorded and honored by the main loop
** after the current command, so later commands in the same line do not run
** and allocated state can be freed. In a forked pipeline the child process
** exits immediately from do_instruction.
*/

#include <unistd.h>
#include "mysh.h"

static int	g_do_exit = 0;
static int	g_exit_code = 0;

int	shell_should_exit(void)
{
  return (g_do_exit);
}

int	shell_exit_code(void)
{
  return (g_exit_code);
}

static int	check_formation(char *str)
{
  int		i;

  i = -1;
  if (!str[0])
    return (-1);
  while (str[++i])
    {
      if (str[i] > '9' || str[i] < '0')
	{
	  if (i != 0 || str[i] != '-')
	    return (-1);
	}
    }
  if (str[0] == '-' && !str[1])
    return (-1);
  return (0);
}

static int	error_exit(char **command)
{
  if (check_formation(command[1]) == -1)
    return (my_int_perror("exit: Badly formed number.\n", 1));
  if (my_tablen(command) != 2)
    return (my_int_perror("exit: Expression Syntax.\n", 1));
  return (0);
}

int	exit_builtin(char ***env, char **command)
{
  int		nbr;

  (void)env;
  if (!command[1])
    nbr = 0;
  else
    {
      if (error_exit(command) == 1)
	return (1);
      nbr = my_getnbr(command[1]);
    }
  if (g_interactive && write(1, "exit\n", 5) == -1)
    return (1);
  g_do_exit = 1;
  g_exit_code = nbr;
  return (nbr);
}
