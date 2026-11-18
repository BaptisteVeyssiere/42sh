/*
** cd builtin. Updates PWD and OLDPWD from the physical working directory.
*/

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "mysh.h"

static int	cd_error(const char *path, const char *msg)
{
  if (path && path[0] &&
      (write(2, path, (size_t)my_strlen((char *)path)) == -1 ||
       write(2, ": ", 2) == -1))
    return (1);
  write(2, msg, (size_t)my_strlen((char *)msg));
  return (1);
}

static char	*cwd_dup(void)
{
  size_t	size;
  char		*buf;

  size = 64;
  while (1)
    {
      buf = malloc(size);
      if (!buf)
	return (NULL);
      if (getcwd(buf, size))
	return (buf);
      free(buf);
      if (errno != ERANGE || size > 1024 * 1024)
	return (NULL);
      size *= 2;
    }
}

static int	env_assign(char ***env, const char *key, const char *value)
{
  char		*argv[4];

  argv[0] = (char *)"setenv";
  argv[1] = (char *)key;
  argv[2] = (char *)value;
  argv[3] = NULL;
  return (setenv_builtin(env, argv));
}

static int	chdir_error(const char *path)
{
  if (errno == ENOENT)
    return (cd_error(path, "No such file or directory.\n"));
  if (errno == ENOTDIR)
    return (cd_error(path, "Not a directory.\n"));
  return (cd_error(path, "Permission denied.\n"));
}

int	cd_builtin(char ***env, char **command)
{
  int		argc;
  char		*target;
  char		*owned;
  char		*oldpwd;
  char		*now;
  int		print;

  argc = my_tablen(command);
  if (argc > 2)
    return (cd_error(NULL, "cd: Too many arguments.\n"));
  owned = NULL;
  print = 0;
  if (argc == 1 || my_strcmp_strict(command[1], "~") ||
      my_strcmp_strict(command[1], "--"))
    {
      owned = get_varenv(*env, "HOME");
      if (!owned)
	return (cd_error(NULL, "cd: HOME not set.\n"));
      target = owned;
    }
  else if (my_strcmp_strict(command[1], "-"))
    {
      owned = get_varenv(*env, "OLDPWD");
      if (!owned)
	return (cd_error(NULL, "cd: OLDPWD not set.\n"));
      target = owned;
      print = 1;
    }
  else
    target = command[1];
  oldpwd = cwd_dup();
  if (!oldpwd)
    {
      free(owned);
      return (cd_error(NULL, "cd: Cannot determine current directory.\n"));
    }
  if (chdir(target) == -1)
    {
      free(owned);
      free(oldpwd);
      return (chdir_error(target));
    }
  now = cwd_dup();
  if (!now || env_assign(env, "OLDPWD", oldpwd) == -1 ||
      env_assign(env, "PWD", now) == -1)
    {
      free(owned);
      free(oldpwd);
      free(now);
      return (1);
    }
  if (print &&
      (write(1, now, (size_t)my_strlen(now)) == -1 || write(1, "\n", 1) == -1))
    {
      free(owned);
      free(oldpwd);
      free(now);
      return (1);
    }
  free(owned);
  free(oldpwd);
  free(now);
  return (0);
}
