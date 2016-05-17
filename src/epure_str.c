/*
** epure_str.c for mysh in /home/VEYSSI_B/rendu/Programmation_Shell/PSU_2015_minishell2/test
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Fri Apr  8 01:52:43 2016 Baptiste veyssiere
** Last update Fri Apr  8 01:58:48 2016 Baptiste veyssiere
*/

int	get_epure_str_length(char *str)
{
  int	i;
  int	length;

  i = -1;
  while (str[++i] && (str[i] == '\t' || str[i] == ' '));
  if (str[i] == 0)
    return (0);
  length = 0;
  while (str[++i])
    ++length;
  return (length);
}
