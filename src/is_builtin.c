/*
** is_builtin.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2/src
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Tue Apr 12 16:09:54 2016 Baptiste veyssiere
** Last update Tue Apr 12 16:11:40 2016 Baptiste veyssiere
*/

#include "mysh.h"

int	is_builtin(t_command *command, int i, int mode)
{
  if (mode == 0)
    {
      if (my_strcmp_strict(command->command[i]->args[0], "cd") ||
          my_strcmp_strict(command->command[i]->args[0], "exit") ||
          my_strcmp_strict(command->command[i]->args[0], "setenv") ||
          my_strcmp_strict(command->command[i]->args[0], "unsetenv"))
        return (1);
    }
  else if (mode == 1)
    {
      if (!my_strcmp_strict(command->command[i]->args[0], "cd") &&
          !my_strcmp_strict(command->command[i]->args[0], "exit") &&
          !my_strcmp_strict(command->command[i]->args[0], "setenv") &&
          !my_strcmp_strict(command->command[i]->args[0], "unsetenv"))
	return (1);
    }
  return (0);
}
