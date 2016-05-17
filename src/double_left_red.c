/*
** double_left_red.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Sun Apr 10 00:49:54 2016 Baptiste veyssiere
** Last update Tue Apr 12 18:03:16 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "mysh.h"

void	fill_file(int fd, char *end)
{
  char	*str;

  while ((str = get_next_line(0)) != NULL &&
	 my_strcmp_strict(str, end) == 0)
    {
      write(fd, str, my_strlen(str));
      write(fd, "\n", 1);
      free(str);
    }
}

int	double_left_red(t_interpipe *command)
{
  int		fd;
  mode_t	mode;

  mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  if ((fd = open("/tmp/mysh", O_CREAT | O_TRUNC | O_WRONLY, mode)) == -1)
    return (-1);
  fill_file(fd, command->double_left_end);
  if (close(fd) == -1)
    return (-1);
  if ((fd = open("/tmp/mysh", O_RDONLY)) == -1)
    return (-1);
  command->fd_input = fd;
  return (0);
}
