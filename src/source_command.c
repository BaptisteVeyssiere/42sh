/*
** Replace unquoted alias names before the line is parsed.
** A word is a candidate only when it looks like a bare name, so quoted
** text and operators are left untouched. Expansion is not re-scanned,
** which keeps an alias from looping on its own replacement.
*/

#include <stdlib.h>
#include "mysh.h"

static int	alias_candidate(char *word)
{
  int		i;

  if (!word || !word[0])
    return (0);
  i = -1;
  while (word[++i])
    {
      if (!((word[i] >= 'a' && word[i] <= 'z') ||
	    (word[i] >= 'A' && word[i] <= 'Z') ||
	    (word[i] >= '0' && word[i] <= '9') ||
	    word[i] == '_' || word[i] == '-'))
	return (0);
    }
  return (1);
}

static int	is_alias(char **word, t_alias *alias)
{
  while (alias != NULL)
    {
      if (my_strcmp_strict(*word, alias->alias))
	{
	  free(*word);
	  *word = alias->equivalent;
	  return (1);
	}
      alias = alias->next;
    }
  return (0);
}

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

static char	*replace_word(char *word, char *str, int save, int *i)
{
  char		*new_str;
  int		x;
  int		y;

  x = -1;
  if ((new_str = malloc(my_strlen(word) - alias_length(&str[save])
			+ 3 + my_strlen(str))) == NULL)
    return (NULL);
  while (++x < save)
    new_str[x] = str[x];
  new_str[x] = ' ';
  y = -1;
  while (word[++y])
    new_str[++x] = word[y];
  new_str[++x] = ' ';
  *i = x;
  save += alias_length(&str[save]) - 1;
  while (str[++save])
    new_str[++x] = str[save];
  new_str[++x] = '\0';
  free(str);
  return (new_str);
}

char	*replace_by_alias(char *str, t_alias *list)
{
  char		*new_str;
  char		*word;
  char		*updated;
  int		save;
  int		end;
  int		x;
  int		guard;
  int		command_pos;

  if ((new_str = copy_str(str)) == NULL)
    return (NULL);
  x = 0;
  guard = 0;
  command_pos = 1;
  while (new_str[x])
    {
      if ((new_str[x] == ' ' || new_str[x] == '\t') && syntactic(new_str, x))
	{
	  ++x;
	  continue;
	}
      if ((new_str[x] == ';' || new_str[x] == '|' || new_str[x] == '&') &&
	  syntactic(new_str, x))
	{
	  command_pos = 1;
	  ++x;
	  continue;
	}
      save = x;
      end = word_end(new_str, x);
      if (end <= save)
	break;
      if ((word = dup_span(new_str, save, end)) == NULL)
	{
	  free(new_str);
	  return (NULL);
	}
      if (command_pos && alias_candidate(word) && is_alias(&word, list))
	{
	  updated = replace_word(word, new_str, save, &x);
	  if (!updated)
	    return (NULL);
	  new_str = updated;
	  ++x;
	  command_pos = 0;
	  if (++guard > 64)
	    break;
	}
      else
	{
	  free(word);
	  x = end;
	  command_pos = 0;
	}
    }
  return (new_str);
}
