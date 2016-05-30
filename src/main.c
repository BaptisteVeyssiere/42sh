/*
** main.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed May 25 17:14:35 2016 Baptiste veyssiere
** Last update Sat May 28 19:56:15 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include "mysh.h"
#include "get_next_line.h"

static int	my_shell(char **env_tmp)
{
  char		*command;
  char		**prompt;
  char		**env;

  if ((prompt = get_prompt()) == NULL ||
      ((env = env_copy(env_tmp)) == NULL && env_tmp[0] != NULL))
    return (-1);
  aff_prompt(prompt);
  while ((command = get_next_line(0)))
    {
      if (execute_command(command, &env) == -1)
	return (-1);
      free_prompt(prompt);
      if (!(prompt = get_prompt()))
	return (-1);
      aff_prompt(prompt);
    }
  free_prompt(prompt);
  free_env(env);
  return (0);
}

int		main(int ac, UNUSED char **av, char **env)
{
  if (ac != 1)
    return (EXIT_SUCCESS);
  if (my_shell(env) == -1)
    return (EXIT_FAILURE);
  return (EXIT_SUCCESS);
}
