/*
** builtins.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Sun May 29 02:01:39 2016 Baptiste veyssiere
** Last update Thu Jun  2 12:19:05 2016 vigner_g
*/

#include <unistd.h>
#include "mysh.h"

int	is_builtin(t_interpipe *command, int mode)
{
  if (mode == 0)
    {
      if (my_strcmp_strict(command->args[0], "cd") ||
          my_strcmp_strict(command->args[0], "exit") ||
          my_strcmp_strict(command->args[0], "setenv") ||
          my_strcmp_strict(command->args[0], "unsetenv") ||
	  my_strcmp_strict(command->args[0], "profile") ||
	  my_strcmp_strict(command->args[0], "history") ||
          (my_strcmp_strict(command->args[0], "setenv") &&
	   command->args[1]) ||
          my_strcmp_strict(command->args[0], "unsetenv"))
        return (1);
    }
  else if (mode == 1)
    {
      if (!my_strcmp_strict(command->args[0], "cd") &&
          !my_strcmp_strict(command->args[0], "exit") &&
	  !my_strcmp_strict(command->args[0], "profile") &&
          (!my_strcmp_strict(command->args[0], "setenv") &&
	   (my_strcmp_strict(command->args[0], "setenv") && !(command->args[1]))) &&
	  !my_strcmp_strict(command->args[0], "unsetenv"))
	return (1);
    }
  return (0);
}/*pas a la norme */

int	exec_builtins(char **args, char ***env, t_datas *data)
{
  int	(*ptr[7])(char***, char**) =
    {
      cd_builtin,
      env_builtin,
      setenv_builtin,
      unsetenv_builtin,
      exit_builtin,
      echo_builtin,
      NULL
    };/* à déplacer */
  char	*name[7];
  int	i;

  name[0] = "cd";
  name[1] = "env";
  name[2] = "setenv";
  name[3] = "unsetenv";
  name[4] = "exit";
  name[5] = "echo";/* à déplacer */
  i = -1;
  if (args[0] != NULL && my_strcmp_strict(args[0], "profile"))
    profile(args, data);
  if (args[0] != NULL && my_strcmp_strict(args[0], "history"))
    aff_history(data->history);
  while (++i < 6 && !my_strcmp_strict(name[i], args[0]));
  if (i == 6)
    return (1);
  else
    return (ptr[i](env, args));
}
