/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytaya <ytaya@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 15:11:09 by ytaya             #+#    #+#             */
/*   Updated: 2022/03/11 09:42:40 by ytaya            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	init_handler(int sig)
{
	char	*result;

	(void) sig;
	if (g_cmd.in_herdoc)
		rl_on_new_line();
	else if (g_cmd.is_forked)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		write(1, "\n", 1);
		g_cmd.exit_code = 130;
		g_cmd.is_forked = 2;
	}
	else
	{
		rl_on_new_line();
		result = ft_strjoin("minishell : ", rl_line_buffer);
		rl_replace_line("", 0);
		result = ft_strjoin(result, "  ");
		write(1, result, ft_strlen(result));
		write(1, "\n", 1);
		g_cmd.exit_code = 1;
		rl_redisplay();
	}
}

void	setup_term(void)
{
	struct termios	t;

	tcgetattr(0, &t);
	t.c_lflag &= ~ECHOCTL;
	tcsetattr(0, TCSANOW, &t);
}

static void	quit_handler(int sig)
{
	(void) sig;
	if (g_cmd.is_forked == 1 && !g_cmd.in_herdoc)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
	}
	else if (g_cmd.in_herdoc)
		rl_on_new_line();
	else
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

void	signal_handler(void)
{
	signal(SIGINT, init_handler);
	signal(SIGQUIT, quit_handler);
}
