/*
** main.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed May 25 17:14:35 2016 Baptiste veyssiere
** Last update Tue May 31 19:44:05 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include "mysh.h"
#include "get_next_line.h"

static int	my_shell(char **env_tmp)
{
  char		*command;
  char		**prompt;
  char		**env;
  int		ret;

  if ((prompt = get_prompt()) == NULL ||
      ((env = env_copy(env_tmp)) == NULL && env_tmp[0] != NULL) ||
      aff_prompt(prompt) == -1)
    return (-1);
  ret = 0;
  while ((command = get_next_line(0)))
    {
      if ((ret = execute_command(command, &env)) == -1)
	return (-1);
      free_prompt(prompt);
      if (!(prompt = get_prompt()) ||
	  aff_prompt(prompt) == -1)
	return (-1);
    }
  free_prompt(prompt);
  free_env(env);
  return (ret);
}

int		main(int ac, UNUSED char **av, char **env)
{
  if (ac != 1)
    return (EXIT_SUCCESS);
  return (my_shell(env));
  /* if ((ret = my_shell(env) == -1) */
  /*   return (EXIT_FAILURE); */
  /* return (EXIT_SUCCESS); */
}
