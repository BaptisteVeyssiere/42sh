/*
** Expand $VAR, ${VAR} and $? and strip quotes from one word.
*/

#include <stdlib.h>
#include <unistd.h>
#include "mysh.h"

static int	grow(char **buf, int *len, int *cap, char ch)
{
  char		*next;
  int		size;

  if (*len + 2 <= *cap)
    {
      (*buf)[(*len)++] = ch;
      (*buf)[*len] = '\0';
      return (0);
    }
  size = (*cap > 0) ? (*cap * 2) : 32;
  next = malloc((size_t)size);
  if (!next)
    return (-1);
  if (*buf && *len)
    {
      int	i;

      i = -1;
      while (++i < *len)
	next[i] = (*buf)[i];
    }
  free(*buf);
  *buf = next;
  *cap = size;
  (*buf)[(*len)++] = ch;
  (*buf)[*len] = '\0';
  return (0);
}

static int	grow_str(char **buf, int *len, int *cap, const char *add)
{
  int		i;

  if (!add)
    return (0);
  i = -1;
  while (add[++i])
    if (grow(buf, len, cap, add[i]) == -1)
      return (-1);
  return (0);
}

static int	name_char(char c, int first)
{
  if (c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
    return (1);
  if (!first && c >= '0' && c <= '9')
    return (1);
  return (0);
}

static int	append_value(char **buf, int *len, int *cap,
			     char **env, const char *name)
{
  char		*value;

  value = get_varenv(env, (char *)name);
  if (!value)
    {
      if (write(2, name, (size_t)my_strlen((char *)name)) == -1 ||
	  write(2, ": Undefined variable.\n", 22) == -1)
	return (-1);
      return (1);
    }
  if (grow_str(buf, len, cap, value) == -1)
    {
      free(value);
      return (-1);
    }
  free(value);
  return (0);
}

static int	expand_dollar(const char *word, int *index, char **buf,
			      int *len, int *cap, char **env, int status)
{
  int		start;
  int		end;
  char		*name;
  char		*rendered;
  int		ret;
  int		i;

  ++(*index);
  if (word[*index] == '?')
    {
      rendered = my_getstr(status);
      if (!rendered || grow_str(buf, len, cap, rendered) == -1)
	{
	  free(rendered);
	  return (-1);
	}
      free(rendered);
      ++(*index);
      return (0);
    }
  if (word[*index] == '{')
    {
      start = ++(*index);
      while (word[*index] && word[*index] != '}')
	++(*index);
      if (word[*index] != '}')
	{
	  write(2, "Bad substitution.\n", 18);
	  return (1);
	}
      end = *index;
      ++(*index);
    }
  else if (name_char(word[*index], 1))
    {
      start = *index;
      while (name_char(word[*index], 0))
	++(*index);
      end = *index;
    }
  else
    {
      if (grow(buf, len, cap, '$') == -1)
	return (-1);
      return (0);
    }
  name = malloc((size_t)(end - start) + 1);
  if (!name)
    return (-1);
  i = 0;
  while (start < end)
    name[i++] = word[start++];
  name[i] = '\0';
  if (!name[0])
    {
      free(name);
      write(2, "Bad substitution.\n", 18);
      return (1);
    }
  ret = append_value(buf, len, cap, env, name);
  free(name);
  return (ret);
}

int	expand_token(char **token, char **env, int status, int do_expand)
{
  char		*buf;
  int		len;
  int		cap;
  int		i;
  char		quote;
  int		pending;
  int		ret;

  if (!token || !*token)
    return (0);
  buf = NULL;
  len = 0;
  cap = 0;
  i = 0;
  quote = 0;
  pending = 0;
  while ((*token)[i])
    {
      if (pending)
	{
	  if (grow(&buf, &len, &cap, (*token)[i]) == -1)
	    return (-1);
	  pending = 0;
	  ++i;
	  continue;
	}
      if (quote == '\'')
	{
	  if ((*token)[i] == '\'')
	    quote = 0;
	  else if (grow(&buf, &len, &cap, (*token)[i]) == -1)
	    return (-1);
	  ++i;
	  continue;
	}
      if (quote == '"')
	{
	  if ((*token)[i] == '\\' && (*token)[i + 1])
	    pending = 1;
	  else if ((*token)[i] == '"')
	    quote = 0;
	  else if (do_expand && (*token)[i] == '$')
	    {
	      ret = expand_dollar(*token, &i, &buf, &len, &cap, env, status);
	      if (ret)
		{
		  free(buf);
		  return (ret);
		}
	      continue;
	    }
	  else if (grow(&buf, &len, &cap, (*token)[i]) == -1)
	    return (-1);
	  ++i;
	  continue;
	}
      if ((*token)[i] == '\\' && (*token)[i + 1])
	{
	  pending = 1;
	  ++i;
	  continue;
	}
      if ((*token)[i] == '\'' || (*token)[i] == '"')
	{
	  quote = (*token)[i];
	  ++i;
	  continue;
	}
      if (do_expand && (*token)[i] == '$')
	{
	  ret = expand_dollar(*token, &i, &buf, &len, &cap, env, status);
	  if (ret)
	    {
	      free(buf);
	      return (ret);
	    }
	  continue;
	}
      if (grow(&buf, &len, &cap, (*token)[i]) == -1)
	return (-1);
      ++i;
    }
  if (!buf)
    {
      buf = malloc(1);
      if (!buf)
	return (-1);
      buf[0] = '\0';
    }
  free(*token);
  *token = buf;
  return (0);
}

int	check_var(t_interpipe **command, char **env, int last_value)
{
  int		i;
  int		j;
  int		ret;

  i = -1;
  while (command && command[++i])
    {
      if (command[i]->input_file)
	{
	  ret = expand_token(&command[i]->input_file, env, last_value,
			     command[i]->if_double_left ? 0 : 1);
	  if (ret)
	    return (ret);
	}
      if (command[i]->output_file)
	{
	  ret = expand_token(&command[i]->output_file, env, last_value, 1);
	  if (ret)
	    return (ret);
	}
      j = -1;
      while (command[i]->args && command[i]->args[++j])
	{
	  ret = expand_token(&command[i]->args[j], env, last_value, 1);
	  if (ret)
	    return (ret);
	}
    }
  return (0);
}
