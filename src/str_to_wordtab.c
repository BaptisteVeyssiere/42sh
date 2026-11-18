/*
** Split a simple command into words.
** Quoted and escaped blanks stay inside the word. Quote characters
** themselves are kept so expansion can tell single quotes from double.
*/

#include <stdlib.h>
#include "mysh.h"

static char	*dup_span(const char *str, int start, int end)
{
  char		*word;
  int		i;

  word = malloc((size_t)(end - start) + 1);
  if (!word)
    return (NULL);
  i = 0;
  while (start < end)
    word[i++] = str[start++];
  word[i] = '\0';
  return (word);
}

char	**str_to_wordtab(char *str)
{
  char		**tab;
  int		count;
  int		i;
  int		n;
  int		start;
  int		end;

  if (!str)
    return (NULL);
  count = 0;
  i = 0;
  while (str[i])
    {
      while (str[i] && (str[i] == ' ' || str[i] == '\t') && syntactic(str, i))
	++i;
      if (!str[i])
	break;
      ++count;
      i = word_end(str, i);
    }
  tab = malloc(sizeof(char *) * (size_t)(count + 1));
  if (!tab)
    return (NULL);
  tab[count] = NULL;
  i = 0;
  n = 0;
  while (str[i])
    {
      while (str[i] && (str[i] == ' ' || str[i] == '\t') && syntactic(str, i))
	++i;
      if (!str[i])
	break;
      start = i;
      end = word_end(str, i);
      tab[n] = dup_span(str, start, end);
      if (!tab[n])
	return (NULL);
      ++n;
      i = end;
    }
  return (tab);
}
