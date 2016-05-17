/*
** check_instruction.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2/test
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Sat Apr  9 15:05:53 2016 Baptiste veyssiere
** Last update Tue Apr 12 16:18:53 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include <unistd.h>
#include "mysh.h"

int	check_if_builtins(char *command)
{
  char	**builtins;
  int	i;

  i = -1;
  if ((builtins = malloc(sizeof(char*) * 6)) == NULL)
    exit(-1);
  builtins[0] = "env";
  builtins[1] = "setenv";
  builtins[2] = "unsetenv";
  builtins[3] = "exit";
  builtins[4] = "cd";
  builtins[5] = NULL;
  while (builtins[++i] != NULL)
    if (my_strcmp(command, builtins[i]) == 1)
      return (1);
  return (0);
}

char	**check_instruction(char *instruction, char **env)
{
  char  **command;
  int   key;
  int   i;

  i = -1;
  key = 1;
  while (instruction[++i])
    if (instruction[i] != ' ' && instruction[i] != '\t')
      key = 0;
  if (key == 1)
    return (NULL);
  if ((command = str_to_wordtab(instruction)) == NULL ||
      check_if_exist(command, env) == -1)
    return (NULL);
  return (command);
}
