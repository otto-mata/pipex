/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tblochet <tblochet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:15:05 by tblochet          #+#    #+#             */
/*   Updated: 2024/12/31 09:12:09 by tblochet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char	*find_value_by_key(char *dict[], char *key)
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

/* int	main(int argc, char const *argv[], char *envp[])
{
	char	*path_var;
	char	**paths;
	char	*full_cmd_path;
	char	*infile;
	pid_t	pid;
	int		fd[2];
	char	*args[] = {"cat", (char *)0};

	if (!argc)
		return (1);
	if (!argv)
		return (1);
	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		usleep(500);
		close(fd[0]);
		path_var = find_value_by_key(envp, "PATH");
		paths = split(path_var, ':');
		full_cmd_path = command_full_path(argv[2], paths);
		execve(full_cmd_path, (char *const *)args, 0);
		return (0);
	}
	else
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		infile = safe_file_open(argv[1]);
		write(1, infile, strlen(infile));
		return (0);
	}
} */
void	error(void)
{
	perror("Error");
	exit(EXIT_FAILURE);
}
void	execute(char *argv, char **envp)
{
	char	**cmd;
	int		i;
	char	*path;

	i = -1;
	cmd = ft_split(argv, ' ');
	path = command_full_path(cmd[0], ft_split(find_value_by_key(envp, "PATH"),
				':'));
	if (!path)
	{
		while (cmd[++i])
			free(cmd[i]);
		free(cmd);
		error();
	}
	fprintf(stderr, "exec: %s\n", path);
	if (execve(path, cmd, envp) == -1)
		error();
}

void	child_process(int target_fd, char *cmd, char **envp, int *fd)
{
	dup2(fd[1], STDOUT_FILENO);
	dup2(target_fd, STDIN_FILENO);
	close(fd[0]);
	execute(cmd, envp);
}

/* Parent process that take the data from the pipe, change the output for the
 fileout and also close with the exec function */
void	parent_process(int target_fd, char *cmd, char **envp, int *fd)
{
	dup2(fd[0], STDIN_FILENO);
	dup2(target_fd, STDOUT_FILENO);
	close(fd[1]);
	execute(cmd, envp);
}

/* Main function that run the child and parent process or display an error
 message if arguments are wrong */
int	main(int argc, char **argv, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid;
	char	*file_in;
	char	*file_out;
	char	*cmd_1;
	char	*cmd_2;
	int		fo_fd;
	int		fi_fd;

	if (argc != 5)
	{
		printf("Usage: %s <file1> <cmd1> <cmd2> <file2>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	file_in = argv[1];
	file_out = argv[4];
	cmd_1 = argv[2];
	cmd_2 = argv[3];
	fo_fd = open(file_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fo_fd == -1)
		error();
	fi_fd = open(file_in, O_RDONLY);
	if (fi_fd == -1)
	{
		close(fo_fd);
		error();
	}
	if (pipe(pipe_fd) == -1)
	{
		close(fo_fd);
		close(fi_fd);
		error();
	}
	pid = fork();
	if (pid == -1)
	{
		close(fo_fd);
		close(fi_fd);
		error();
	}
	else if (pid == 0)
		child_process(fi_fd, cmd_1, envp, pipe_fd);
	else
		// fork2 
		parent_process(fo_fd, cmd_2, envp, pipe_fd);
	waitpid(pid, NULL, 0);
	return (0);
}

int	_main(int argc, char **argv, char **envp)
{
	int		new_fds[2];
	int		old_fds[2];
	pid_t	pid;
	int		fo;
	int		fi;
	int		i;

	fo = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fo == -1)
		error();
	fi = open(argv[1], O_RDONLY);
	if (fi == -1)
	{
		close(fo);
		error();
	}
	argv[argc - 1] = 0;
	argv = &argv[2];
	if (pipe(old_fds) < 0)
	{
		close(fo);
		close(fi);
		error();
	}
	dup2(fi, STDIN_FILENO);
	close(fi);
	i = -1;
	while (argv[++i])
	{
		if (argv[i + 1] != 0)
			pipe(new_fds);
		pid = fork();
		if (pid == 0)
		{
			if (i > 0)
			{
				dup2(old_fds[0], STDIN_FILENO);
				close(old_fds[0]);
				close(old_fds[1]);
			}
			if (argv[i + 1] != 0)
			{
				close(new_fds[0]);
				dup2(new_fds[1], STDOUT_FILENO);
				close(new_fds[1]);
			}
			else
			{
				dup2(new_fds[0], STDIN_FILENO);
				close(new_fds[0]);
				close(new_fds[1]);
				dup2(fo, STDOUT_FILENO);
				close(fo);
			}
			execute(argv[i], envp);
		}
		else
		{
			if (i > 0)
			{
				close(old_fds[0]);
				close(old_fds[1]);
			}
			if (argv[i + 1] != 0)
			{
				old_fds[0] = new_fds[0];
				old_fds[1] = new_fds[1];
			}
			// store PIDS for later 
			// waitpid(pid, 0, 0);
		}
	}
	close(old_fds[0]);
	close(old_fds[1]);
	return (0);
}

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
#include "get_next_line.h"
#include <stdbool.h>
int main_heredoc(int argc, char **argv, char **envp)
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
	randstr((tmp_name + ft_strlen(".heredoc.") ), 13);
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
