/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   bonus.c                                              ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2024/11/29 17:15:05 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/01/05 20:02:36 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
/*                                                                            */
/* ************************************************************************** */


#include "pipex.h"

unsigned int	randuint(void)
{
	int				ufd;
	unsigned int	res;

	ufd = open("/dev/urandom", O_RDONLY);
	if (!ufd)
		exit(EXIT_FAILURE);
	if (read(ufd, (char *)&res, 4) != 4)
		exit(EXIT_FAILURE);
	close(ufd);
	return (res);
}

char	randclower(void)
{
	unsigned int	src;

	src = randuint();
	return (src % (26) + 'a');
}

char	randcupper(void)
{
	unsigned int	src;

	src = randuint();
	return (src % (26) + 'A');
}

char	*randstr(char *dest, size_t sz)
{
	size_t	i;

	i = 0;
	while (i < sz)
	{
		if (randuint() % 2)
			dest[i] = randclower();
		else
			dest[i] = randcupper();
		i++;
	}
	dest[i] = 0;
	return (dest);
}
#include <stdbool.h>

int	main_heredoc(int argc, char **argv, char **envp)
{
	char	*buff;
	bool	lim_match;
	char	tmp_name[24];
	size_t	lim_size;
	int		fi;
	int		read_size;

	if (argc != 2)
		return (0);
	lim_size = ft_strlen(argv[1]) + 1;
	ft_strlcpy(tmp_name, ".heredoc.", ft_strlen(".heredoc.") + 1);
	randstr((tmp_name + ft_strlen(".heredoc.")), 13);
	fi = open(tmp_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fi < 0)
		return (1);
	lim_match = 0;
	read_size = 0;
	// Replace by PIPE like bash > 5.1
	while (!lim_match)
	{
		ft_putstr_fd("\rheredoc> ", 2);
		buff = get_next_line(0);
		lim_match = ft_strncmp(buff, argv[1], ft_strlen(buff) - 1) == 0;
		lim_match = lim_match && (buff[0] != '\n');
		if (!lim_match)
			ft_putstr_fd(buff, fi);
		free(buff);
	}
	/*
	Regular PipeX's mandatory part's code goes here
	*/
	unlink(tmp_name);
	close(fi);
	return (0);
}
