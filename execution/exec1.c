/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-ghem <ael-ghem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 06:00:28 by ytaya             #+#    #+#             */
/*   Updated: 2022/03/11 14:11:51 by ael-ghem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	safe_close(int fd, int safe_fd)
{
	if (fd != safe_fd)
		close(fd);
}

int	pipe_handler(t_list *cmd, t_pipe *p)
{
	while (cmd)
	{
		p->cmd = get_cmd_args(((t_command *)cmd->content)->args);
		p->files = ((t_command *)cmd->content)->file;
		if (cmd->next != NULL && !pipe(p->fds))
			p->outfd = p->fds[1];
		else
		{
			p->fds[0] = 0;
			p->outfd = 1;
		}
		if (handle_files(p->files, &p->infd, &p->outfd) != -1
			&& g_cmd.is_forked != 3)
		{
			if (!check_builtin(p->cmd[0]))
				p->pids[p->i++] = execute_cmd(p->cmd, p, 0);
			else
				g_cmd.exit_code = execute_builtin_parent(p->cmd, p);
			safe_close(p->outfd, 1);
			safe_close(p->infd, 0);
			p->infd = p->fds[0];
		}
		cmd = cmd->next;
	}
	return (0);
}

int	execute(t_list *cmd)
{
	t_pipe	p;

	g_cmd.is_forked = 1;
	ft_bzero(&p, sizeof(t_pipe));
	p.pids = (pid_t *)xmalloc(sizeof(pid_t) * list_size(cmd) + 1);
	if (pipe_handler(cmd, &p) == -1)
		return (-1);
	while (p.j < p.i)
	{
		if (p.pids[p.j] == -1)
			printf("minishell : fork: Resource temporarily unavailable\n");
		waitpid(p.pids[p.j++], &p.status, 0);
		if (WTERMSIG(p.status) == SIGQUIT)
			write(2, "Quit : 3\n", 9);
	}
	if (g_cmd.is_forked == 1)
		g_cmd.exit_code = WEXITSTATUS(p.status);
	g_cmd.is_forked = 0;
	return (0);
}
