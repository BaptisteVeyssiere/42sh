/*
** get_fd_bis.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2/src
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Sun Apr 10 02:01:32 2016 Baptiste veyssiere
** Last update Sun Apr 10 02:20:49 2016 Baptiste veyssiere
*/

#include "mysh.h"

int	get_fd_output_bis(t_interpipe *command, int i, int length, char *str)
{
  int	j;

  str[0] = 0;
  str[length] = 0;
  --i;
  j = -1;
  while (command->str[++i] && command->str[i] != ' ' && command->str[i] != '\t')
    {
      str[++j] = command->str[i];
      command->str[i] = ' ';
    }
  return (get_fd_out(str, command->if_double_right));
}

int	get_fd_input_bis(t_interpipe *command, int i, int length, char *str)
{
  int	j;

  str[0] = 0;
  str[length] = 0;
  --i;
  j = -1;
  while (command->str[++i] && command->str[i] != ' ' && command->str[i] != '\t')
    {
      str[++j] = command->str[i];
      command->str[i] = ' ';
    }
  if (command->if_double_left == 0)
    return (get_fd_in(str));
  else
    {
      command->double_left_end = str;
      if (str[0] == 0)
        return (my_perror("Missing name for redirect.\n"));
    }
  return (0);
}
