/*
** double_left_red.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Sun Apr 10 00:49:54 2016 Baptiste veyssiere
** Last update Thu Jun  9 00:34:25 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "mysh.h"
#include "get_next_line.h"

static int	fill_file(int fd, char *end)
{
  char		*str;

  while ((str = get_next_line(0)) != NULL)
    {
      if (my_strcmp_strict(str, end))
	{
	  free(str);
	  return (0);
	}
      if (write(fd, str, my_strlen(str)) == -1 ||
	  write(fd, "\n", 1) == -1)
	{
	  free(str);
	  return (-1);
	}
      free(str);
    }
  return (0);
}

int	double_left_red(t_interpipe *command)
{
  int		fd;
  char		path[] = "/tmp/42shXXXXXX";

  if ((fd = mkstemp(path)) == -1)
    return (-1);
  if (fill_file(fd, command->input_file) == -1 || close(fd) == -1)
    {
      unlink(path);
      return (-1);
    }
  if ((fd = open(path, O_RDONLY)) == -1)
    {
      unlink(path);
      return (-1);
    }
  unlink(path);
  command->fd_input = fd;
  return (0);
}
