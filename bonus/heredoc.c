/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   heredoc.c                                            ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2025/01/05 21:41:09 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/01/05 22:57:25 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <fcntl.h>

static void	first_cmd(int read_from, char *cmd, char **envp, int *pip)
{
	dup2(read_from, STDIN_FILENO);
	dup2(pip[1], STDOUT_FILENO);
	close(pip[0]);
	close(pip[1]);
	close(read_from);
	dprintf(2,"First chhild\n");
	execute(cmd, envp);

}

static void	secnd_cmd(int write_to, char *cmd, char **envp, int *pip)
{
	dup2(write_to, STDOUT_FILENO);
	dup2(pip[0], STDIN_FILENO);
	close(pip[0]);
	close(pip[1]);
	close(write_to);
	execute(cmd, envp);
}

static void	validate(int *fo, int *p, char *o_file)
{
	*fo = open(o_file, O_APPEND | O_CREAT, 0644);
	if (*fo == -1)
		error("file_out");
	if (pipe(p) < 0)
	{
		close(*fo);
		error("pipe_val");
	}
}

static void	safer_fork(pid_t *pid, int fi, int fo)
{
	*pid = fork();
	if (*pid == -1)
	{
		close(fo);
		error("safer_fork");
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*buff;
	int		lim_match;
	int		heredoc_pipe[2];
	int		pipe_fd[2];
	pid_t	pid[2];
	int		fo;

	lim_match = 0;
	validate(&fo, heredoc_pipe, argv[5]);
	while (!lim_match)
	{
		ft_putstr_fd("\rheredoc> ", 2);
		buff = get_next_line(0);
		lim_match = ft_strncmp(buff, argv[2], ft_strlen(buff) - 1) == 0;
		lim_match = lim_match && (buff[0] != '\n');
		if (!lim_match)
			ft_putstr_fd(buff, heredoc_pipe[1]);
		free(buff);
	}
	close(heredoc_pipe[1]);
	if (pipe(pipe_fd) < 0)
		error("AAAH");
	safer_fork(&pid[0], heredoc_pipe[0], pipe_fd[1]);
	if (pid[0] == 0)
		first_cmd(heredoc_pipe[0], argv[3], envp, pipe_fd);
	else
	{
		safer_fork(&pid[1], pipe_fd[0], fo);
		if (pid[1] == 0)
			secnd_cmd(fo, argv[4], envp, pipe_fd);
	}

	close_pipe(pipe_fd);
	close_pipe(heredoc_pipe);
	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], NULL, 0);
	return (0);
}
