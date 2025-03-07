/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   mandatory.c                                          ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2025/01/05 18:45:05 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/02/01 16:23:26 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	first_cmd(int target_fd, char *cmd, char **envp, int *fd)
{
	if (dup2(fd[1], STDOUT_FILENO) < 0)
		error("pipex: dup2");
	if (dup2(target_fd, STDIN_FILENO) < 0)
		error("pipex: dup2");
	close(fd[1]);
	close(fd[0]);
	close(target_fd);
	execute(cmd, envp);
}

static void	secnd_cmd(int target_fd, char *cmd, char **envp, int *fd)
{
	if (dup2(fd[0], STDIN_FILENO) < 0)
		error("pipex: dup2");
	if (dup2(target_fd, STDOUT_FILENO) < 0)
		error("pipex: dup2");
	close(fd[0]);
	close(fd[1]);
	close(target_fd);
	execute(cmd, envp);
}

static void	validate(int *fi, int *fo, int *p, char **argv)
{
	*fo = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fo == -1)
		error("pipex: outfile");
	*fi = open(argv[1], O_RDONLY);
	if (*fi == -1)
	{
		close(*fo);
		error("pipex: infile");
	}
	if (pipe(p) < 0)
	{
		close(*fo);
		close(*fi);
		error("pipex: pipe");
	}
}

static void	safer_fork(pid_t *pid, int fi, int fo)
{
	*pid = fork();
	if (*pid == -1)
	{
		close(fo);
		close(fi);
		error("pipex: fork");
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid[2];
	int		fo_fd;
	int		fi_fd;

	if (argc != 5)
	{
		ft_printf("Usage: %s <file1> <cmd1> <cmd2> <file2>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	validate(&fi_fd, &fo_fd, (int *)pipe_fd, argv);
	safer_fork(pid, fi_fd, fo_fd);
	if (pid[0] == 0)
		first_cmd(fi_fd, argv[2], envp, pipe_fd);
	else
	{
		safer_fork(&pid[1], fi_fd, fo_fd);
		if (pid[1] == 0)
			secnd_cmd(fo_fd, argv[3], envp, pipe_fd);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], NULL, 0);
	return (close(fo_fd), close(fi_fd), 0);
}
