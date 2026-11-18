/*
** Quote, escape and operator classification for the shell parser.
*/

#include <stdlib.h>
#include "mysh.h"

/*
** Walk [0, pos). *quoted is non-zero when pos sits inside quotes.
** *escaped is non-zero when a backslash immediately escapes pos.
** Inside single quotes a backslash is literal. Inside double quotes it
** escapes the next character.
*/
static void	scan_state(const char *str, int pos, int *quoted, int *escaped)
{
  char		quote;
  int		pending;
  int		i;

  quote = 0;
  pending = 0;
  i = 0;
  while (str[i] && i < pos)
    {
      if (pending)
	{
	  pending = 0;
	  ++i;
	  continue;
	}
      if (quote == '\'')
	{
	  if (str[i] == '\'')
	    quote = 0;
	  ++i;
	  continue;
	}
      if (quote == '"')
	{
	  if (str[i] == '\\' && str[i + 1])
	    pending = 1;
	  else if (str[i] == '"')
	    quote = 0;
	  ++i;
	  continue;
	}
      if (str[i] == '\\')
	pending = 1;
      else if (str[i] == '\'' || str[i] == '"')
	quote = str[i];
      ++i;
    }
  *quoted = quote != 0;
  *escaped = pending;
}

int	syntactic(const char *str, int index)
{
  int	quoted;
  int	escaped;

  if (!str || index < 0 || !str[index])
    return (0);
  scan_state(str, index, &quoted, &escaped);
  return (!quoted && !escaped);
}

int	quote_error(const char *str)
{
  int	quoted;
  int	escaped;
  int	len;

  if (!str)
    return (0);
  len = my_strlen((char *)str);
  scan_state(str, len, &quoted, &escaped);
  if (quoted)
    return (1);
  if (escaped)
    return (2);
  return (0);
}

int	is_and_if(const char *str, int index)
{
  return (str[index] == '&' && str[index + 1] == '&' &&
	  syntactic(str, index) && syntactic(str, index + 1));
}

int	is_or_if(const char *str, int index)
{
  return (str[index] == '|' && str[index + 1] == '|' &&
	  syntactic(str, index) && syntactic(str, index + 1));
}

int	is_pipe(const char *str, int index)
{
  if (str[index] != '|' || !syntactic(str, index))
    return (0);
  if (is_or_if(str, index))
    return (0);
  if (index > 0 && is_or_if(str, index - 1))
    return (0);
  return (1);
}

static int	word_boundary(const char *str, int index)
{
  char		c;

  if (!syntactic(str, index))
    return (0);
  c = str[index];
  return (c == ' ' || c == '\t' || c == ';' || c == '|' ||
	  c == '&' || c == '<' || c == '>');
}

int	word_end(const char *str, int index)
{
  while (str[index] && !word_boundary(str, index))
    ++index;
  return (index);
}

char	*mask_quoted_operators(const char *str)
{
  char	*copy;
  int	i;

  copy = copy_str((char *)str);
  if (!copy)
    return (NULL);
  i = -1;
  while (copy[++i])
    {
      if (!syntactic(copy, i) &&
	  (copy[i] == '|' || copy[i] == '&' || copy[i] == ';' ||
	   copy[i] == '<' || copy[i] == '>'))
	copy[i] = 'a';
    }
  return (copy);
}
