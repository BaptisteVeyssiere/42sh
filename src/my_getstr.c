/*
** Decimal rendering of an int, used for $?.
*/

#include <stdlib.h>
#include <stddef.h>

char	*my_getstr(int nbr)
{
  char		tmp[16];
  char		*str;
  long		value;
  int		neg;
  int		i;
  int		j;

  value = nbr;
  neg = 0;
  if (value < 0)
    {
      neg = 1;
      value = -value;
    }
  i = 0;
  if (value == 0)
    tmp[i++] = '0';
  while (value > 0)
    {
      tmp[i++] = (char)('0' + (value % 10));
      value /= 10;
    }
  str = malloc((size_t)i + (size_t)neg + 1);
  if (!str)
    return (NULL);
  j = 0;
  if (neg)
    str[j++] = '-';
  while (i > 0)
    str[j++] = tmp[--i];
  str[j] = '\0';
  return (str);
}
