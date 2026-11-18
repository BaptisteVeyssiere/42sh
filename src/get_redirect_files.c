/*
** Detach redirection operators from a simple command.
** The operator and its word are blanked so the later word split keeps
** only the argv. Quoted blanks belong to the filename.
*/

#include <stdlib.h>
#include "mysh.h"

static char	*dup_span(const char *str, int start, int end)
{
  char		*name;
  int		i;

  name = malloc((size_t)(end - start) + 1);
  if (!name)
    return (NULL);
  i = 0;
  while (start < end)
    name[i++] = str[start++];
  name[i] = '\0';
  return (name);
}

static void	blank_span(char *str, int start, int end)
{
  while (start < end)
    str[start++] = ' ';
}

static int	store_redirect(t_interpipe *command, char *name,
			       int input, int doubled)
{
  if (input)
    {
      free(command->input_file);
      command->input_file = name;
      command->left_red = doubled ? 0 : 1;
      command->if_double_left = doubled ? 1 : 0;
    }
  else
    {
      free(command->output_file);
      command->output_file = name;
      command->right_red = doubled ? 0 : 1;
      command->if_double_right = doubled ? 1 : 0;
    }
  return (0);
}

int	get_redirect_files(t_interpipe *interpipe, char *command)
{
  int		i;
  int		start;
  int		end;
  int		doubled;
  char		*name;

  i = 0;
  while (command[i])
    {
      if ((command[i] == '<' || command[i] == '>') && syntactic(command, i))
	{
	  doubled = 0;
	  if (command[i + 1] == command[i] && syntactic(command, i + 1))
	    doubled = 1;
	  start = i + doubled + 1;
	  while (command[start] &&
		 (command[start] == ' ' || command[start] == '\t') &&
		 syntactic(command, start))
	    ++start;
	  if (!command[start])
	    return (-1);
	  end = word_end(command, start);
	  name = dup_span(command, start, end);
	  if (!name)
	    return (-1);
	  store_redirect(interpipe, name, command[i] == '<', doubled);
	  blank_span(command, i, end);
	  i = end;
	  continue;
	}
      ++i;
    }
  return (0);
}
