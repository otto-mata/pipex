/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   heredoc.c                                            ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2025/01/05 21:41:09 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/01/07 07:41:26 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
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
	*fo = open(o_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (*fo == -1)
		error("file_out");
	if (pipe(p) < 0)
	{
		close(*fo);
		error("pipe_val");
	}
}

static void	get_content(t_heredoc conf)
{
	char	*buff;
	int		lim_match;

	lim_match = 0;
	while (!lim_match)
	{
		ft_putstr_fd("\rheredoc> ", 2);
		buff = get_next_line(0);
		lim_match = ft_strncmp(buff, conf.limiter, ft_strlen(buff) - 1) == 0;
		lim_match = (lim_match && (buff[0] != '\n'));
		if (!lim_match)
			ft_putstr_fd(buff, conf.pipe[1]);
		free(buff);
	}
	close(conf.pipe[1]);
	get_next_line(-2);
}

int	heredoc(t_heredoc conf, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid[2];
	int		fo;

	validate(&fo, conf.pipe, conf.ofile);
	get_content(conf);
	if (pipe(pipe_fd) < 0)
		error("AAAH");
	safer_fork(&pid[0], conf.pipe[0], pipe_fd[1]);
	if (pid[0] == 0)
		first_cmd(conf.pipe[0], conf.cmd0, envp, pipe_fd);
	else
	{
		safer_fork(&pid[1], pipe_fd[0], fo);
		if (pid[1] == 0)
			secnd_cmd(fo, conf.cmd1, envp, pipe_fd);
	}
	close_pipe(pipe_fd);
	close_pipe(conf.pipe);
	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], NULL, 0);
	return (0);
}
