/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   utils1.c                                             ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2025/01/07 07:05:31 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/01/16 04:12:30 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	safer_fork(pid_t *pid, int fi, int fo)
{
	*pid = fork();
	if (*pid == -1)
	{
		close(fo);
		close(fi);
		error("pipex: fork");
	}
}
