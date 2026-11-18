/*
** Line reader with one leftover buffer.
** The buffer is reset when the file descriptor changes, so history
** loading cannot leak into the interactive input stream.
*/

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "mysh.h"
#include "get_next_line.h"

static char	*g_buf = NULL;
static size_t	g_len = 0;
static size_t	g_pos = 0;
static int	g_fd = -1;

void	get_next_line_clear(void)
{
  free(g_buf);
  g_buf = NULL;
  g_len = 0;
  g_pos = 0;
  g_fd = -1;
}

static int	append_chunk(const char *chunk, size_t n)
{
  char		*next;

  next = malloc(g_len + n + 1);
  if (!next)
    return (-1);
  if (g_len)
    memcpy(next, g_buf, g_len);
  memcpy(next + g_len, chunk, n);
  g_len += n;
  next[g_len] = '\0';
  free(g_buf);
  g_buf = next;
  return (0);
}

static char	*take_line(size_t end, int drop_newline)
{
  size_t	size;
  char		*line;

  size = end - g_pos;
  line = malloc(size + 1);
  if (!line)
    return (NULL);
  if (size)
    memcpy(line, g_buf + g_pos, size);
  line[size] = '\0';
  g_pos = end + (drop_newline ? 1 : 0);
  if (g_pos >= g_len)
    {
      g_pos = 0;
      g_len = 0;
    }
  else if (g_pos > 4096)
    {
      memmove(g_buf, g_buf + g_pos, g_len - g_pos);
      g_len -= g_pos;
      g_pos = 0;
      g_buf[g_len] = '\0';
    }
  return (line);
}

static void	switch_fd(int fd)
{
  if (fd == g_fd)
    return ;
  free(g_buf);
  g_buf = NULL;
  g_len = 0;
  g_pos = 0;
  g_fd = fd;
}

char	*get_next_line(const int fd)
{
  char		chunk[128];
  ssize_t	got;
  size_t	i;

  if (fd < 0)
    return (NULL);
  switch_fd(fd);
  while (1)
    {
      i = g_pos;
      while (i < g_len)
	{
	  if (g_buf[i] == '\n')
	    return (take_line(i, 1));
	  ++i;
	}
      got = read(fd, chunk, sizeof(chunk));
      if (got < 0 && errno == EINTR)
	{
	  if (ctrc)
	    {
	      g_pos = 0;
	      g_len = 0;
	      return (NULL);
	    }
	  continue;
	}
      if (got < 0)
	return (NULL);
      if (got == 0)
	{
	  if (g_pos < g_len)
	    return (take_line(g_len, 0));
	  return (NULL);
	}
      if (append_chunk(chunk, (size_t)got) == -1)
	return (NULL);
    }
}
