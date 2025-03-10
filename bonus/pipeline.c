/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   pipeline.c                                           ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2025/01/05 20:02:15 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/01/16 04:10:28 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	setup(t_pipes *fds, char *fo_name, char *fi_name)
{
	fds->fo = open(fo_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fds->fo == -1)
		error("file_out");
	fds->fi = open(fi_name, O_RDONLY);
	if (fds->fi == -1)
	{
		close(fds->fo);
		error("file_in");
	}
	if (pipe(fds->old) < 0)
	{
		close(fds->fo);
		close(fds->fi);
		error("pipe");
	}
	if (dup2(fds->fi, STDIN_FILENO) < 0)
		error("dup2[fi=>STDIN]");
	close(fds->fi);
}

static void	fork_work(int i, t_pipes fds, char **argv, char **envp)
{
	if (i > 0)
		dup_close(fds.old, STDIN_FILENO, fds.old[0]);
	if (argv[i + 1] != 0)
		dup_close(fds.new, STDOUT_FILENO, fds.new[1]);
	else
	{
		close_pipe(fds.new);
		dup2(fds.fo, STDOUT_FILENO);
		close(fds.fo);
	}
	execute(argv[i], envp);
}

static void	close_all(t_pipes fds)
{
	close(fds.fi);
	close(fds.fo);
}

static void	exec_cmds(t_pipes fds, pid_t *pid, char **argv, char **envp)
{
	int	i;

	i = -1;
	while (argv[++i])
	{
		if (argv[i + 1] != 0)
			pipe(fds.new);
		pid[i] = fork();
		if (pid[i] < 0)
		{
			close_all(fds);
			error("pipex: fork");
		}
		else if (pid[i] == 0)
			fork_work(i, fds, argv, envp);
		else
		{
			if (i > 0)
				close_pipe(fds.old);
			if (argv[i + 1] != 0)
				pipcpy(fds.old, fds.new);
		}
	}
}

int	pipeline(int argc, char **argv, char **envp)
{
	t_pipes	fds;
	pid_t	pid[1024];
	int		i;

	setup(&fds, argv[argc - 1], argv[1]);
	argv[argc - 1] = 0;
	argv = &argv[2];
	ft_bzero(pid, 1024 * sizeof(pid_t));
	exec_cmds(fds, pid, argv, envp);
	i = -1;
	while (++i < argc - 2)
		waitpid(pid[i], 0, 0);
	return (close_all(fds), 0);
}
