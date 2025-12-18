/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:05:29 by llechert          #+#    #+#             */
/*   Updated: 2025/12/18 09:30:53 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int wait_children(t_cmd *cmd_lst)
{
	t_cmd *cmd;
	int code;

	if (!cmd_lst)
		return (0);
	cmd = cmd_lst;
	while (cmd->next)
	{
		if (cmd->pid > 0)
			waitpid(cmd->pid, &cmd->exit_status, 0);
		cmd = cmd->next;
	}
	if (cmd->pid > 0)
		waitpid(cmd->pid, &cmd->exit_status, 0);
	code = cmd->exit_status;
	if (cmd->pid > 0)
	{
		if (WIFEXITED(code))
			return (WEXITSTATUS(code));
		else if (WIFSIGNALED(code))
			return (128 + WTERMSIG(code));
		else
			return (1);
	}
	return (code);
}

int infinite_loop(t_shell *shell)
{
	while (1)
	{
		g_signal_received = 0;
		shell->av = readline("AU SUIVANT> ");
		if (g_signal_received)
			shell->exit_code = 130;
		if (!shell->av)
			return (clean_exit(shell), shell->exit_code);
		if (shell->av && *shell->av)
			add_history(shell->av);
		if (!lexer(shell, shell->av))
		{
			clean_post_lexer(shell);
			continue;
		}
		if (!parser(shell, &shell->token))
		{
			clean_post_parser(shell);
			continue;
		}
		if (!execution(shell, shell->cmds))
		{
			shell->exit_code = wait_children(shell->cmds);
			clean_post_parser(shell);
			continue;
		}
		if (shell->flag_exit)
			return (clean_exit(shell), shell->exit_code);
		if (shell->cmds) // si on a envoye une liste vide on veut pas changer l'exit code et il y a pas d'enfants a attendre
			shell->exit_code = wait_children(shell->cmds);
		prepare_next_loop(shell);
	}
	return (shell->exit_code);
}
