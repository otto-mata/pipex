/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   pipe_utils.c                                         ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2025/01/05 21:39:32 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/01/05 21:39:50 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_pipe(int *pfd)
{
	close(pfd[0]);
	close(pfd[1]);
}

void	pipcpy(int *pipd, int *pips)
{
	close_pipe(pipd);
	pipd[0] = pips[0];
	pipd[1] = pips[1];
}

void	dup_close(int *pfd, int target, int origin)
{
	if (dup2(origin, target) < 0)
		error("dup_close()");
	close_pipe(pfd);
}
