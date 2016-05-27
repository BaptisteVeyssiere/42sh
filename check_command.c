/*
** check_command.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Thu May 26 14:52:40 2016 Baptiste veyssiere
** Last update Thu May 26 23:39:32 2016 Baptiste veyssiere
*/

static int	check_command_loop(char *command)
{
  return (1);
}

int	check_command(char *command)
{
  int	error;

  if ((error = check_command_loop(command)))
    {
      if (error == 1 && write(2, "Invalid null command.\n", 22) == -1)
	return (-1);
      if (error == 2 && write(2, "Missing name for redirect.\n", 28) == -1)
	return (-1);
      return (1);
    }
  return (0);
}

int	main(int ac, char **av)
{
  if (ac != 2)
    return (0);
  check_command(av[1]);
  return (0);
}
