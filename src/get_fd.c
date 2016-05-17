/*
** get_fd.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2/test
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Fri Apr  8 02:00:07 2016 Baptiste veyssiere
** Last update Sun Apr 10 02:20:14 2016 Baptiste veyssiere
*/

#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "mysh.h"

int	get_fname(char *str, int i)
{
  int	length;

  length = 0;
  while (str[i + ++length] &&
	 str[i + length] != ' ' && str[i + length] != '\t');
  return (length);
}

int		get_fd_out(char *str, char d)
{
  int		fd;
  mode_t	mode;

  if (str[0] == 0)
    return (my_perror("Missing name for redirect.\n"));
  mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  if (d)
    fd = open(str, O_CREAT | O_APPEND | O_WRONLY, mode);
  else
    fd = open(str, O_CREAT | O_TRUNC | O_WRONLY, mode);
  free(str);
  return (fd);
}

int	get_fd_in(char *str)
{
  int   fd;
  mode_t        mode;

  if (str[0] == 0)
    return (my_perror("Missing name for redirect.\n"));
  mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  fd = open(str, O_RDONLY, mode);
  if (fd == -1)
    {
      write(1, str, my_strlen(str));
      write(1, ": No such file or directory.\n",
	    my_strlen(": No such file or directory.\n"));
    }
  free(str);
  return (fd);
}

int	get_fd_output(t_interpipe *command)
{
  int	i;
  int	length;
  char	*str;

  i = -1;
  while (command->str[++i] && command->str[i] != '>');
  if (command->str[i] == 0)
    return (-1);
  if (command->str[i + 1] == '>')
    command->if_double_right = 1;
  command->str[i] = ' ';
  while (command->str[++i] && (command->str[i] == '>'
			       || command->str[i] == ' ' || command->str[i] == '\t'))
    command->str[i] = ' ';
  length = get_fname(command->str, i);
  if ((str = malloc(sizeof(char) * (length + 1))) == NULL)
    return (-1);
  return (get_fd_output_bis(command, i, length, str));
}

int	get_fd_input(t_interpipe *command)
{
  int   i;
  int   length;
  char  *str;

  i = -1;
  while (command->str[++i] && command->str[i] != '<');
  if (command->str[i] == 0)
    return (-1);
  if (command->str[i + 1] == '<')
    command->if_double_left = 1;
  command->str[i] = ' ';
  while (command->str[++i] && (command->str[i] == '<'
                               || command->str[i] == ' ' ||
			       command->str[i] == '\t'))
    command->str[i] = ' ';
  length = get_fname(command->str, i);
  if ((str = malloc(sizeof(char) * (length + 1))) == NULL)
    return (-1);
  return (get_fd_input_bis(command, i, length, str));
}
