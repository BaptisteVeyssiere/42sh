/*
** verif_command.c for 42sh in /home/semmao_i/rendu/PSU_2015_42sh
** 
** Made by ilyas semmaoui
** Login   <semmao_i@epitech.net>
** 
** Started on  Wed Jun  1 18:50:01 2016 ilyas semmaoui
** Last update Wed Jun  1 18:54:35 2016 ilyas semmaoui
*/

#include "mysh.h"

void	change_count_value(char *command, t_counter *cnt)
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

int	check_pipe_and_args(char *command, t_counter *cnt)
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

int	check_redir_and_args(char *command, t_counter *cnt)
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

int	eof_check(t_counter *cnt)
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
