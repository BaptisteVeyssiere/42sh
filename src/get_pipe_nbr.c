/*
** get_pipe_nbr.c for 42sh in /home/VEYSSI_B/rendu/Programmation_Shell/test/test_42tree
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Sat May 21 08:41:58 2016 Baptiste veyssiere
** Last update Sat May 21 09:33:32 2016 Baptiste veyssiere
*/

int	get_pipe_nbr(char *s)
{
  int   i;
  int   nbr;

  i = -1;
  nbr = 0;
  while (s[++i])
    if (s[i] == '|')
      ++nbr;
  return (nbr);
}
