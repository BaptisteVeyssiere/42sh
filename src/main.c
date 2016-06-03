/*
** main.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed May 25 17:14:35 2016 Baptiste veyssiere
** Last update Fri Jun  3 16:52:32 2016 vigner_g
*/

#include	<stdlib.h>
#include	<unistd.h>
#include	"mysh.h"
#include	"get_next_line.h"

static int	my_shell(char **env_tmp)
{
  char		*command;
  char		**prompt;
  char		**env;
  t_datas	data;
  int		ret;

  data.history = NULL;
  if ((data.home = get_varenv(env_tmp, "HOME")) == NULL)
    return (my_int_perror("get_varenv failed\n", -1));
  data.history = load_history(&data, data.home,
			      "default", data.history);
  if ((prompt = get_prompt()) == NULL ||
      ((env = env_copy(env_tmp)) == NULL && env_tmp[0] != NULL) ||
      aff_prompt(prompt) == -1)
    return (-1);
  ret = 0;
  while ((command = get_next_line(0)))
    {
	if ((ret = execute_command(&data, command, &env)) == -1)
	  return (-1);
      free_prompt(prompt);
      if (!(prompt = get_prompt()) ||
	  aff_prompt(prompt) == -1)
	return (-1);
    }
  free_prompt(prompt);
  free_history(data.history);
  close(data.fd);
  free(data.home);
  free_env(env);
  return (ret);
}

int		main(int ac, UNUSED char **av, char **env)
{
  if (ac != 1)
    return (EXIT_SUCCESS);
  return (my_shell(env));
}
