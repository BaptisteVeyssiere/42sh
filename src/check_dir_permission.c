/*
** check_dir_permission.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_42sh
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Sun Jun  5 00:37:16 2016 Baptiste veyssiere
** Last update Sun Jun  5 00:40:35 2016 Baptiste veyssiere
*/

#include <pwd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "mysh.h"

int	check_dir_permission(char *file, char permission)
{
  struct stat	buf;
  struct passwd	*owner;
  char		*dir;
  int		ret;

  dir = NULL;
  ret = 0;
  if (file[0] == '/' && file[my_strlen(file) - 1] != '/' && permission == 'w')
    {
      dir = my_getdir(file);
      if (!dir || stat(dir, &buf) == -1 || !(owner = getpwuid(buf.st_uid)))
	ret = -1;
      else if (!(S_IWUSR & buf.st_mode) ||
	       (!(S_IWGRP & buf.st_mode) &&
		my_strcmp_strict("root", owner->pw_name)))
	{
	  if (write(2, file, my_strlen(file)) == -1)
	    ret = -1;
	  else
	    ret = my_int_perror(": Permission denied.\n", 1);
	}
    }
  free(dir);
  return (ret);
}
