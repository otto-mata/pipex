/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   main.c                                               ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2025/01/07 07:28:24 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/01/07 07:40:14 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	usage(char *execnm, int type)
{
	if (type == 0)
		ft_printf("Usage: %s <ifile> <cmd0> <cmd1> ... <cmdN> <ofile>\n",
			execnm);
	else if (type == 1)
		ft_printf("Usage: %s here_doc <limiter> <cmd0> <cmd1> <ofile>\n",
			execnm);
	return (EXIT_FAILURE);
}

int	main(int argc, char **argv, char **envp)
{
	int			type;
	t_heredoc	hd;

	type = 0;
	if (argc >= 2)
		type = (ft_strcmp(argv[1], "here_doc") == 0);
	if (argc < 5 || (type == 1 && argc != 6))
		return (usage(argv[0], type));
	if (type == 1)
	{
		hd.cmd0 = argv[3];
		hd.cmd1 = argv[4];
		hd.limiter = argv[2];
		hd.ofile = argv[5];
		return (heredoc(hd, envp));
	}
	else
		return (pipeline(argc, argv, envp));
}
