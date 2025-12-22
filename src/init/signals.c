/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 18:28:42 by llechert          #+#    #+#             */
/*   Updated: 2025/12/22 18:28:43 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	rl_replace_line(const char *text, int clear_undo);

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	if (g_signal_received == 0 || g_signal_received == 1)
		rl_redisplay();
	g_signal_received = 1;
}

void	init_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_signal_received = 67;
	write(1, "\n", 1);
}

void	main_loop_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(130);
}
