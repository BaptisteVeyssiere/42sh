/*
** alias.c for 42sh in /home/scutar_n/rendu/PSU/PSU_2015_42sh
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Wed Jun  1 20:29:06 2016 Nathan Scutari
** Last update Sun Jun  5 11:26:01 2016 vigner_g
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "get_next_line.h"
#include "mysh.h"

char	*get_home_path(char **env)
{
  int	x;
  int	y;
  char	*file;
  char	*home;
  char	*path;

  file = ".42shrc";
  if ((home = get_varenv(env, "HOME")) == NULL ||
      (path = malloc(my_strlen(file) + my_strlen(home) + 2)) == NULL)
    return (NULL);
  x = -1;
  while (home[++x])
    path[x] = home[x];
  x -= 1;
  if (path[x] != '/')
    path[++x] = '/';
  y = -1;
  while (file[++y])
    path[++x] = file[y];
  path[++x] = 0;
  return (path);
}

int	cut_alias(char *str, int save, int to, char **alias)
{
  int	x;

  if (to - save < 1)
    return (1);
  if ((*alias = malloc(to - save + 2)) == NULL)
    return (1);
  x = -1;
  while (save + ++x <= to)
    (*alias)[x] = str[x + save];
  (*alias)[x] = 0;
  return (0);
}



int	get_alias_arg(char *str, int x, char **alias)
{
  int	save;

  x -= 1;
  while (str[++x] && (str[x] == ' ' || str[x] == '\t'));
  if (!str[x])
    return (0);
  if (str[x] == '\'')
    {
      save = x + 1;
      while (str[++x] && str[x] != '\'');
      if (!str[x])
	return (0);
      if (cut_alias(str, save, x - 1, alias))
	return (0);
      return (x + 1);
    }
  save = x;
  while (str[++x] && str[x] != ' ' && str[x] != '\t');
  if (cut_alias(str, save, x - 1, alias))
    return (0);
  return (x);
}

void		create_alias(t_alias **list, char *alias, char *equivalent)
{
  t_alias	*elem;

  if ((elem = malloc(sizeof(t_alias))) == NULL)
    return ;
  elem->alias = alias;
  elem->equivalent = equivalent;
  elem->next = *list;
  *list = elem;
}

void	check_line(char *line, t_alias **list)
{
  char	*alias;
  char	*equivalent;
  int	x;

  x = -1;
  while (line[++x] == ' ' || line[x] == '\t');
  if (!line[x])
    return ;
  if (my_strcmp("alias ", &line[x]) == 0)
    return ;
  x -= 1;
  while (line[++x] && line[x] != ' ' && line[x] != '\t');
  if (!line[x])
    return ;
  if ((x = get_alias_arg(line, x, &alias)) == 0)
    return ;
  if ((x = get_alias_arg(line, x, &equivalent)) == 0)
    return ;
  while (line[++x] == ' ' || line[x] == '\t');
  if (line[x])
    return ;
  create_alias(list, alias, equivalent);
}

void		free_alias(t_alias *list)
{
  t_alias	*tmp;

  while (list)
    {
      tmp = list;
      list = list->next;
      free(tmp->alias);
      free(tmp->equivalent);
      free(tmp);
    }
}

t_alias		*load_alias(char *path, char **env, t_alias *old)
{
  char		*line;
  int		fd;
  t_alias	*alias;

  alias = NULL;
  if (!path && (path = get_home_path(env)) == NULL)
    return (NULL);
  if ((fd = open(path, O_RDONLY)) == -1)
    return (NULL);
  while ((line = get_next_line(fd)) != NULL)
    {
      check_line(line, &alias);
      free(line);
    }
  if (!alias)
    return (old);
  else if (old)
    {
      close(old->source);
      free_alias(old);
    }
  alias->source = fd;
  return (alias);
}

char	*get_a_word(char *str, int *x)
{
  int	save;
  char	*word;
  int	y;
  int	i;

  i = *x - 1;
  save = *x;
  while (str[++i] && str[i] != ' ' && str[i] != ';' && str[i] != '\t' &&
	 str[i] != '|' && str[i] != '&');
  if ((word = malloc(i - save + 1)) == NULL)
    return (NULL);
  save -= 1;
  y = -1;
  while (++save < i)
    word[++y] = str[save];
  word[++y] = 0;
  *x = i - 1;
  return (word);
}

int	is_alias(char **word, t_alias *alias)
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

int	alias_length(char *str)
{
  int	i;

  i = -1;
  while (str[++i] && str[i] != ' ' && str[i] != ';' && str[i] != '\t' &&
	 str[i] != '|' && str[i] != '&');
  return (i);
}

char	*replace_word(char *word, char *str, int save, int *i)
{
  char	*new_str;
  int	x;
  int	y;

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
  new_str[++x] = 0;
  free(str);
  return (new_str);
}

char	*copy_str(char *str)
{
  int	x;
  char	*cpy;

  if ((cpy = malloc(my_strlen(str) + 1)) == NULL)
    return (NULL);
  x = -1;
  while (str[++x])
    cpy[x] = str[x];
  cpy[x] = 0;
  return (cpy);
}

char	*replace_by_alias(char *str, t_alias *list)
{
  char	*new_str;
  char	*word;
  int	save;
  int	x;

  x = -1;
  if ((new_str = copy_str(str)) == NULL)
    return (NULL);
  while (new_str[++x])
    {
      if (new_str[x] != ';' && new_str[x] != ' ' && new_str[x] != '\t' &&
	  new_str[x] != '|' && new_str[x] != '&')
	{
	  save = x;
	  if ((word = get_a_word(new_str, &x)) && is_alias(&word, list))
	    {
	      if ((new_str = replace_word(word, new_str, save, &x)) == NULL)
		return (NULL);
	    }
	  else if (word)
	    free(word);
	}
    }
  return (new_str);
}
