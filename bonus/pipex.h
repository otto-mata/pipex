/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   pipex.h                                              ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2025/01/05 18:45:30 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/01/15 23:05:24 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "../libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pipes		t_pipes;
typedef struct s_heredoc	t_heredoc;
struct						s_pipes
{
	int						new[2];
	int						old[2];
	int						fi;
	int						fo;
};

struct						s_heredoc
{
	char					*ofile;
	char					*limiter;
	char					*cmd0;
	char					*cmd1;
	int						pipe[2];
};

char						*value_of(char *dict[], char *key);
char						*command_full_path(char const *cmd, char **paths);
void						free_2d(void **ptr);
void						execute(char *argv, char **envp);
void						error(char const *s);
void						close_pipe(int *pfd);
void						pipcpy(int *pipd, int *pips);
void						dup_close(int *pfd, int target, int origin);
void						safer_fork(pid_t *pid, int fi, int fo);
int							heredoc(t_heredoc conf, char **envp);
int							pipeline(int argc, char **argv, char **envp);

#endif
