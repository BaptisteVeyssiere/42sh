/*
** Interactive prompt. Printed only by the caller when stdin is a tty.
*/

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "mysh.h"

void	free_prompt(char **prompt)
{
  if (!prompt)
    return ;
  free(prompt[0]);
  free(prompt);
}

static char	*prompt_cwd(void)
{
  size_t	size;
  char		*buf;

  size = 64;
  while (1)
    {
      buf = malloc(size);
      if (!buf)
	return (NULL);
      if (getcwd(buf, size))
	return (buf);
      free(buf);
      if (errno != ERANGE)
	return (NULL);
      if (size > 1024 * 1024)
	return (NULL);
      size *= 2;
    }
}

char	**get_prompt(void)
{
  char		**prompt;

  prompt = malloc(sizeof(char *) * 2);
  if (!prompt)
    return (NULL);
  prompt[0] = prompt_cwd();
  if (!prompt[0])
    {
      prompt[0] = malloc(2);
      if (!prompt[0])
	{
	  free(prompt);
	  return (NULL);
	}
      prompt[0][0] = '?';
      prompt[0][1] = '\0';
    }
  prompt[1] = NULL;
  return (prompt);
}

int	aff_prompt(char **prompt)
{
  char		*path;
  char		*shown;
  int		i;
  int		slashes;

  if (!prompt || !prompt[0])
    return (-1);
  path = prompt[0];
  shown = path;
  slashes = 0;
  i = my_strlen(path);
  while (i > 0)
    {
      --i;
      if (path[i] == '/')
	{
	  ++slashes;
	  if (slashes == 2)
	    {
	      shown = path + i + 1;
	      break;
	    }
	}
    }
  if (write(1, shown, (size_t)my_strlen(shown)) == -1 ||
      write(1, "> ", 2) == -1)
    return (-1);
  return (0);
}
