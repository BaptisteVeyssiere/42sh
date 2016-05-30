/*
** get_redirect_files.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Sat May 28 00:11:16 2016 Baptiste veyssiere
** Last update Sun May 29 00:49:30 2016 Baptiste veyssiere
*/

#include <stdlib.h>
#include <unistd.h>
#include "mysh.h"

static void	fill_redirect_leaf(char *var, char *command, int i, int nbr)
{
  command[i] = ' ';
  if (nbr == 2)
    command[i + 1] = ' ';
  *var = 1;
}

static void	get_redirect_type(t_interpipe *interpipe, char *command)
{
  int		i;

  i = -1;
  while (command[++i] && command[i] != '>' && command[i] != '<');
  if (!command[i])
    return ;
  if (command[i] == '>' && command[i + 1] == '>')
    fill_redirect_leaf(&(interpipe->if_double_right), command, i, 2);
  else if (command[i] == '>' && command[i + 1] != '>')
    fill_redirect_leaf(&(interpipe->right_red), command, i, 1);
  else if (command[i] == '<' && command[i + 1] == '<')
    fill_redirect_leaf(&(interpipe->if_double_left), command, i, 2);
  else if (command[i] == '<' && command[i + 1] != '<')
    fill_redirect_leaf(&(interpipe->left_red), command, i, 1);
}

static int	get_redirect_filename_length(char *command, int pos)
{
  int		length;

  length = 1;
  while (command[++pos] && command[pos] != ' ')
    ++length;
  return (length);
}

static char	*get_redirect_filename(char *command)
{
  char		*name;
  int		length;
  int		pos;
  int		i;

  i = -1;
  pos = -1;
  while (command[++pos] && command[pos] != ' ');
  while (command[++pos] && command[pos] == ' ');
  length = get_redirect_filename_length(command, pos);
  if (!(name = malloc(length + 1)))
    return (NULL);
  name[length] = 0;
  while (++i < length)
    {
      name[i] = command[pos + i];
      command[pos + i] = ' ';
    }
  return (name);
}

int	get_redirect_files(t_interpipe *interpipe, char *command)
{
  get_redirect_type(interpipe, command);
  if (((interpipe->left_red || interpipe->if_double_left) &&
       !(interpipe->input_file = get_redirect_filename(command))) ||
      ((interpipe->right_red || interpipe->if_double_right) &&
       !(interpipe->output_file = get_redirect_filename(command))))
    return (-1);
  get_redirect_type(interpipe, command);
  if ((interpipe->input_file == NULL &&
       (interpipe->left_red || interpipe->if_double_left) &&
       !(interpipe->input_file = get_redirect_filename(command))) ||
      (interpipe->output_file == NULL &&
       (interpipe->right_red || interpipe->if_double_right) &&
       !(interpipe->output_file = get_redirect_filename(command))))
    return (-1);
  return (0);
}
