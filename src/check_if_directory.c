/*
** check_if_directory.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Mon May 30 23:49:50 2016 Baptiste veyssiere
** Last update Tue May 31 22:56:53 2016 Baptiste veyssiere
*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "mysh.h"

int		check_if_directory(char *command)
{
  struct stat	buf;

  if (command[0] != '/')
    return (0);
  if (access(command, F_OK) == 0 &&
      stat(command, &buf) == -1)
    return (-1);
  if (S_ISDIR(buf.st_mode))
    {
      if (write(2, command, my_strlen(command)) == -1)
	return (-1);
      return (my_int_perror(": Permission denied.\n", 1));
    }
  return (0);
}

int		check_if_exist(char *command)
{
  struct stat	buf;

  if (access(command, F_OK))
    {
      if (write(2, command, my_strlen(command)) == -1)
	return (-1);
      return (my_int_perror(": No such file or directory.\n", 1));
    }
  if (stat(command, &buf) == -1)
    return (-1);
  if ((S_IRUSR & buf.st_mode) == 0)
    {
      if (write(2, command, my_strlen(command)) == -1)
        return (-1);
      return (my_int_perror(": Permission denied.\n", 1));
    }
  if (S_ISDIR(buf.st_mode) == 0)
    {
      if (write(2, command, my_strlen(command)) == -1)
        return (-1);
      return (my_int_perror(": Not a directory.\n", 1));
    }
  return (0);
}

int	check_exit(t_tree **tree)
{
  int	i;
  int	j;
  int	k;

  i = -1;
  while (tree[++i])
    {
      j = -1;
      while (tree[i]->and_or[++j])
	{
	  k = -1;
	  while (tree[i]->and_or[j]->command[++k]);
	  --k;
	  if (my_strcmp_strict(tree[i]->and_or[j]->command[k]->args[0], "exit"))
	    {
	      if (write(1, "exit\n", 5) == -1)
		return (1);
	      return (1);
	    }
	}
    }
  return (0);
}
