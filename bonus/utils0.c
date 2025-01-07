/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   utils.c                                              ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2025/01/05 18:45:24 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/01/05 21:58:58 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error(char const *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}

void	free_2d(void **ptr)
{
	size_t	len;
	size_t	i;

	len = 0;
	while (ptr[len++])
		;
	i = 0;
	while (i < len)
		free(ptr[i++]);
	free(ptr);
}

char	*value_of(char *dict[], char *key)
{
	char	**d2;
	size_t	key_len;

	if (!key || !dict)
		return (0);
	d2 = dict;
	key_len = ft_strlen(key);
	while (*d2)
	{
		if (ft_strncmp(key, *d2, key_len) == 0 && (*d2)[key_len] == '=')
			return (*d2 + key_len + 1);
		d2++;
	}
	return (0);
}

char	*command_full_path(char const *cmd, char **paths)
{
	char	*try_path;
	char	**p;
	char	*tmp;

	p = paths;
	while (*p)
	{
		tmp = ft_strjoin((char const *)*p, "/");
		try_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!try_path)
			break ;
		if (access(try_path, X_OK))
			free(try_path);
		else
			return (try_path);
		p++;
	}
	return (free_2d((void **)paths), (void *)0);
}

void	execute(char *argv, char **envp)
{
	char	**cmd;
	int		i;
	char	*path;

	i = -1;
	cmd = ft_split(argv, ' ');
	path = command_full_path(cmd[0], ft_split(value_of(envp, "PATH"), ':'));
	if (!path)
	{
		while (cmd[++i])
			free(cmd[i]);
		free(cmd);
		error("cmd");
	}
	if (execve(path, cmd, envp) == -1)
		error("exec");
}
