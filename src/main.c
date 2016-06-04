/*
** main.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed May 25 17:14:35 2016 Baptiste veyssiere
** Last update Sat Jun  4 18:54:08 2016 vigner_g
*/

#include	<stdlib.h>
#include	<unistd.h>
#include	"mysh.h"
#include	"get_next_line.h"

static int	clone_env(char **env_tmp, t_datas *data)
{
  char		**env;

  if ((env = env_copy(env_tmp)) == NULL && env_tmp[0] != NULL)
    return (-1);
  data->env = env;
  data->fd = -1;
  return (0);
}

static int	my_shell(char **env_tmp)
{
  char		*command;
  char		**prompt;
  t_datas	data;
  int		ret;

  data.history = NULL;
  if ((data.home = get_varenv(env_tmp, "HOME")) != NULL)
    data.history = load_history(&data, data.home,
				"default", data.history);
  if (clone_env(env_tmp, &data) == -1 || (prompt = get_prompt()) == NULL ||
      aff_prompt(prompt) == -1)
      return (-1);
  ret = 0;
  while ((command = get_next_line(0)))
    {
	if ((ret = execute_command(&data, command)) == -1)
	  return (-1);
      free_prompt(prompt);
      if (!(prompt = get_prompt()) ||
	  aff_prompt(prompt) == -1)
	return (-1);
    }
  free_all(&data, prompt);
  return (ret);
}

int		main(int ac, UNUSED char **av, char **env)
{
  if (ac != 1)
    return (EXIT_SUCCESS);
  return (my_shell(env));
}
