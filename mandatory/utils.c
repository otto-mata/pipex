/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   utils.c                                              ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2025/01/05 18:45:24 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/02/01 16:23:57 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
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
	if (!p || !cmd)
		return (free_2d((void **)paths), (void *)0);
	if (ft_strchr(cmd, '/') && !access(cmd, X_OK))
		return (free_2d((void **)paths), (char *)cmd);
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
	if (!cmd)
		exit(EXIT_FAILURE);
	path = command_full_path(cmd[0], ft_split(value_of(envp, "PATH"), ':'));
	if (!path)
	{
		ft_dprintf(2, "pipex: command not found: %s\n", cmd[0]);
		while (cmd[++i])
			free(cmd[i]);
		free(cmd);
		exit(EXIT_FAILURE);
	}
	if (execve(path, cmd, envp) == -1)
		ft_dprintf(2, "pipex: exec: %s failed\n", cmd[0]);
	free_2d((void **)cmd);
}
