/*
** Trim and collapse whitespace that is not quoted or escaped.
*/

#include <stdlib.h>
#include "mysh.h"

static int	blank_syntax(const char *str, int index)
{
  return ((str[index] == ' ' || str[index] == '\t') &&
	  syntactic(str, index));
}

char	*epure_str(char *str)
{
  char		*out;
  int		i;
  int		j;
  int		pending;

  if (!str)
    return (NULL);
  out = malloc((size_t)my_strlen(str) + 1);
  if (!out)
    return (NULL);
  i = 0;
  while (str[i] && blank_syntax(str, i))
    ++i;
  j = 0;
  pending = 0;
  while (str[i])
    {
      if (blank_syntax(str, i))
	{
	  pending = 1;
	  ++i;
	  continue;
	}
      if (pending)
	{
	  out[j++] = ' ';
	  pending = 0;
	}
      out[j++] = str[i++];
    }
  out[j] = '\0';
  return (out);
}
