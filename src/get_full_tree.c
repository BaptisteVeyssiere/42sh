/*
** main.c for test in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2/test
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Wed Apr  6 15:21:46 2016 Baptiste veyssiere
** Last update Mon Apr 11 02:22:16 2016 Baptiste veyssiere
*/

#include <unistd.h>
#include <stdlib.h>
#include "mysh.h"

int	get_pipe_nbr(char *s)
{
  int	i;
  int	nbr;

  i = -1;
  nbr = 0;
  while (s[++i])
    if (s[i] == '|')
      ++nbr;
  return (nbr);
}

t_command	*get_command_struct(char *s)
{
  t_command	*command;
  char		**tab;

  if ((command = malloc(sizeof(t_command))) == NULL)
    return (NULL);
  command->pipe_nbr = get_pipe_nbr(s);
  tab = get_red_tab(s);
  if (tab == NULL)
    return (NULL);
  if ((command->command = get_interpipe_tab(tab)) == NULL)
    return (NULL);
  free_wordtab(tab);
  return (command);
}

t_command	**get_tree(char **tab)
{
  int		i;
  t_command	**tree;

  i = -1;
  while (tab[++i] != NULL);
  if ((tree = malloc(sizeof(t_command*) * (i + 1))) == NULL)
    return (NULL);
  tree[i] = NULL;
  i = -1;
  while (tab[++i] != NULL)
    if ((tree[i] = get_command_struct(tab[i])) == NULL)
      return (NULL);
  return (tree);
}

void	free_tree(t_command **tree)
{
  int	i;
  int	j;

  i = -1;
  while (tree[++i] != NULL)
    {
      j = -1;
      while (tree[i]->command[++j] != NULL)
	{
	  free(tree[i]->command[j]->str);
	  if (tree[i]->command[j]->double_left_end != NULL)
	    free(tree[i]->command[j]->double_left_end);
	  free_wordtab(tree[i]->command[j]->args);
	  free(tree[i]->command[j]);
	}
      free(tree[i]->command);
      free(tree[i]);
    }
  free(tree);
}

int	get_full_tree(char *command, char ***env)
{
  char		**tab;
  t_command	**tree;

  if ((tab = get_command_tab(command)) == NULL)
    return (-1);
  if ((tree = get_tree(tab)) == NULL)
    return (-1);
  if (sort_tree(tree, *env) == -1)
    return (0);
  free_wordtab(tab);
  execute_tree(tree, env);
  free_tree(tree);
  return (0);
}
