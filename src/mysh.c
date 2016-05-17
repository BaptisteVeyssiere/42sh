/*
** mysh.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Tue Mar 29 16:27:35 2016 Baptiste veyssiere
** Last update Mon Apr 11 23:43:33 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include "mysh.h"

int	mysh(char **envp)
{
  char  *instruction;
  char	**prompt;
  char	**env;

  if ((prompt = get_prompt()) == NULL ||
      ((env = env_copy(envp)) == NULL && envp[0] != NULL))
    return (-1);
  aff_prompt(prompt);
  while ((instruction = get_next_line(0)) != NULL)
    {
      if (instruction[0] != 0)
	{
	  if (get_full_tree(instruction, &env) == -1 ||
	      free_prompt(prompt) ||
	      (prompt = get_prompt()) == NULL)
	    return (-1);
	  aff_prompt(prompt);
	}
      else
	aff_prompt(prompt);
      free(instruction);
    }
  free_prompt(prompt);
  free_env(env);
  return (0);
}

int	main(UNUSED int ac, UNUSED char **av, char **env)
{
  return (mysh(env));
}
