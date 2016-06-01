/*
** check_command.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Thu May 26 14:52:40 2016 Baptiste veyssiere
** Last update Wed Jun  1 03:25:22 2016 ilyas semmaoui
*/

typedef struct	s_counter
{
  int		i;
  int		redir;
  int		count;
  int		pipe;
  int		key;
  int		arg;
  int		except;
  int		error;
}		t_counter;

int		match(char c, char *str)
{
  int		i;

  i = -1;
  while (str && str[++i])
    if (str[i] == c)
      return (0);
  return (1);
}

void		init_counter(t_counter *cnt)
{
  cnt->i = -1;
  cnt->redir = 0;
  cnt->arg = 0;
  cnt->error = 0;
  cnt->pipe = 0;
  cnt->key = 0;
  cnt->count = 0;
  cnt->except = 0;
}

void		change_count_value(char *command, t_counter *cnt)
{
  if (match(command[cnt->i], "|&\t ") == 1)
    cnt->count = 0;
  if (match(command[cnt->i], "|\t ") == 1)
    cnt->pipe = 0;
  if (match(command[cnt->i], ";&\t ") == 1)
    cnt->except = 0;
  if ((command[cnt->i] == '|' && command[cnt->i + 1] == '|') ||
      (command[cnt->i] == '&' && command[cnt->i + 1] == '&') ||
      command[cnt->i] == ';')
    cnt->key = 0;
  if (match(command[cnt->i], "><|& \t;") == 1 &&
      (command[cnt->i + 1] == '\0' ||
       match(command[cnt->i + 1], "><|& \t;") == 0))
    cnt->arg += 1;
  if (command[cnt->i] == '>')
    cnt->key = 1;
}

int		check_pipe_and_args(char *command, t_counter *cnt)
{
  if (command[cnt->i] == ';')
    {
      if ((cnt->error == 1 && cnt->arg < 2) ||
	  (cnt->arg == 1 && cnt->redir > 0))
	return (1);
      cnt->arg = 0;
      cnt->error = 0;
      cnt->except = 1;
    }
  if (command[cnt->i] == '|' && command[cnt->i - 1] != '|' &&
      command[cnt->i + 1] != '|')
    {
      if ((cnt->error == 1 && cnt->arg < 2) ||
	  (cnt->arg == 1 && cnt->redir > 0))
	return (1);
      if (cnt->key == 1 && cnt->arg == 0)
	return (2);
      if (cnt->key == 1)
	return (3);
      cnt->pipe++;
      cnt->arg = 0;
      cnt->error = 0;
    }
  return (0);
}

int		check_redir_and_args(char *command, t_counter *cnt)
{
  if (match(command[cnt->i], "><") == 0)
    {
      if (command[cnt->i] == '<' && cnt->pipe > 0)
	return (4);
      if (cnt->arg == 0)
	cnt->error = 1;
      cnt->redir++;
      if (cnt->redir == 3 ||
	  (cnt->redir == 2 && match(command[cnt->i - 1], "\t ") == 0))
	return (2);
    }
  if ((cnt->pipe > 0 && command[cnt->i] == ';') ||
      (cnt->count > 0 && (cnt->pipe > 0 || command[cnt->i] == ';')))
    return (1);
  if ((command[cnt->i] == '|' && command[cnt->i + 1] == '|') ||
      (command[cnt->i] == '&' && command[cnt->i + 1] == '&'))
    {
      cnt->count++;
      if ((cnt->error == 1 && cnt->arg < 2) ||
	  (cnt->arg == 1 && cnt->redir > 0))
	return (1);
      cnt->arg = 0;
      cnt->error = 0;
    }
  return (0);
}

int		eof_check(t_counter *cnt)
{
  if (cnt->redir > 0)
    return (2);
  if (cnt->pipe > 0)
    return (1);
  if (cnt->except == 1)
    return (0);
  if (cnt->count > 0 || (cnt->error == 1 && cnt->arg < 2) ||
      (cnt->arg == 1 && cnt->redir > 0))
    return (1);
  return (0);
}

static int	check_command_loop(char *command)
{
  t_counter	cnt;
  int		ret;

  if (command[0] == '|')
    return (1);
  init_counter(&cnt);
  while (command && command[++cnt.i])
    {
      change_count_value(command, &cnt);
      if (((ret = check_pipe_and_args(command, &cnt)) > 0) ||
	  ((ret = 1) == 1 && cnt.pipe > 1) ||
	  ((ret = 2) == 2 && cnt.redir > 0 && match(command[cnt.i], "|&;") == 0) ||
	  ((ret = check_redir_and_args(command, &cnt)) > 0) ||
	  ((ret = 1) == 1 && (cnt.count >= 2 || (cnt.count > 0 && cnt.pipe > 0))))
	return (ret);
      if (match(command[cnt.i], "><\t ") == 1)
	cnt.redir = 0;
    }
  return (eof_check(&cnt));
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
      if (error == 3 && write(2, "Ambiguous output redirect.\n", 27) == -1)
	return (-1);
      if (error == 4 && write(2, "Ambiguous input redirect.\n", 26) == -1)
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
