/*
** check_command.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Thu May 26 14:52:40 2016 Baptiste veyssiere
** Last update Tue May 31 04:00:18 2016 ilyas semmaoui
*/

static int	check_command_loop(char *command)
{
  int		i;
  int		redir;
  int		count;
  int		pipe;
  int		except;

  i = -1;
  redir = 0;
  pipe = 0;
  count = 0;
  except = 0;
  if (command[0] == '|')
    return (1);
  while (command && command[++i])
    {
      if (command[i] == ';')
	except = 1;
      if (command[i] == '|' && command[i - 1] != '|' && command[i + 1] != '|')
	pipe++;
      if (pipe > 1)
	return (1);
      if (redir > 0 &&
	  (command[i] == '|' || command[i] == '&' || command[i] == ';'))
	return (2);
      if (command[i] == '>' || command[i] == '<')
	{
	  redir++;
	  if (redir == 3 ||
	      (redir == 2 && (command[i - 1] == ' ' || command[i - 1] == '\t')))
	    return (2);
	}
      if (command[i] != '>' && command[i] != '<' && command[i] != ' ' &&
	  command[i] != '\t')
	redir = 0;
      if (pipe > 0 && command[i] == ';')
	return (1);
      if (count > 0 && (pipe > 0 || command[i] == ';'))
	return (1);
      if ((command[i] == '|' && command[i + 1] == '|') ||
	  (command[i] == '&' && command[i + 1] == '&'))
	{
	  count++;
	}
      if (count >= 2 || (count > 0 && pipe > 0))
	return (1);
      if (command[i] != '|' && command[i] != '&' && command[i] != ' ' &&
	  command[i] != '\t')
	count = 0;
      if (command[i] != '|' && command[i] != ' ' && command[i] != '\t')
	pipe = 0;
      if (command[i] != ';' && command[i] != ' ' && command[i] != '\t' &&
	  command[i] != '&')
	except = 0;
    }
  if (redir > 0)
    return (2);
  if (pipe > 0)
    return (1);
  if (except == 1)
    return (0);
  if (count > 0)
    return (1);
  return (0);
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
