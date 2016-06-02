/*
** open_fd.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Sun May 29 00:28:56 2016 Baptiste veyssiere
** Last update Sun May 29 00:49:00 2016 Baptiste veyssiere
*/

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "mysh.h"

static int	input_open(char *file)
{
  int		fd;
  mode_t	mode;

  mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  fd = open(file, O_RDONLY, mode);
  if (fd == -1)
    {
      write(1, file, my_strlen(file));
      write(1, ": No such file or directory.\n",
            my_strlen(": No such file or directory.\n"));
      return (-2);
    }
  return (fd);

}

static int	output_open(char *file, int red_nbr)
{
  int		fd;
  mode_t	mode;

  mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  if (red_nbr == 2)
    fd = open(file, O_CREAT | O_APPEND | O_WRONLY, mode);
  else
    fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, mode);
  return (fd);
}

int	open_fd(t_interpipe **command)
{
  int	i;
  char	if_double;

  i = -1;
  while (command[++i])
    {
      if_double = 0;
      if (command[i]->left_red)
	{
	  if ((command[i]->fd_input = input_open(command[i]->input_file)) < 0)
	    return (command[i]->fd_input);
	}
      if (command[i]->right_red || command[i]->if_double_right)
	{
	  if (command[i]->if_double_right)
	    if_double = 1;
	  if ((command[i]->fd_output =
	       output_open(command[i]->output_file, (1 + if_double))) == -1)
	    return (-1);
	}
    }
  return (0);
}