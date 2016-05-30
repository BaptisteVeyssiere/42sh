/*
** echo.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Mon May 30 01:16:00 2016 Baptiste veyssiere
** Last update Mon May 30 01:20:43 2016 Baptiste veyssiere
*/

#include <unistd.h>
#include "mysh.h"

int	echo_builtin(UNUSED char ***env, char **command)
{
  int	i;

  i = 0;
  if (!command[1] && write(1, "\n", 1) == -1)
    return (-1);
  else if (command[1])
    while (command[++i])
      if (write(1, command[i], my_strlen(command[i])) == -1)
	return (-1);
  return (0);
}
