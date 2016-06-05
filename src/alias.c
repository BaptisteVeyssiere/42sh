/*
** alias.c for 42sh in /home/scutar_n/rendu/PSU/PSU_2015_42sh
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Wed Jun  1 20:29:06 2016 Nathan Scutari
** Last update Sun Jun  5 03:21:24 2016 Nathan Scutari
*/

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "get_next_line.h"
#include "mysh.h"

static char	*get_home_path(char **env)
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

static int	get_alias_arg(char *str, int x, char **alias)
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

static void		create_alias(t_alias **list, char *alias, char *equivalent)
{
  t_alias	*elem;

  if ((elem = malloc(sizeof(t_alias))) == NULL)
    return ;
  elem->alias = alias;
  elem->equivalent = equivalent;
  elem->next = *list;
  *list = elem;
}

static void	check_line(char *line, t_alias **list)
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

t_alias	*load_alias(char *path, char **env, t_alias *old)
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
