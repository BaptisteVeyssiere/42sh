/*
** main.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Mon May 23 18:38:40 2016 Baptiste veyssiere
** Last update Mon May 23 18:44:42 2016 Baptiste veyssiere
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
	  if (get_tree(instruction, &env) == -1 ||
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
