/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:05:29 by llechert          #+#    #+#             */
/*   Updated: 2025/12/17 12:13:19 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	wait_children(t_cmd *cmd_lst)
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
	if (cmd->pid > 0)/* Si un enfant a été attendu, interpréter le status; sinon, renvoyer tel quel (builtin ou erreur avant fork) */
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
		if (!shell->av)
		{
			clean_exit(shell);
			return (shell->exit_code);
		}
		// ft_putstr_fd(CYAN "========================================" RESET "\n", 1);
		if (shell->av && *shell->av)
			add_history(shell->av);
		if (!lexer(shell, shell->av)) // si pb, on imprime erreur dans lexer
		{
			clean_post_lexer(shell); // on prepare la prochaine boucle en faisant free
			continue;                // et passe a la boucle suivante
		}
		if (!parser(shell, &shell->token)) // on imprime l'erreur si besoin dans la fonction
		{
			clean_post_parser(shell); // inclut clean lexer dedans !
			continue;
		}
		if (!execution(shell, shell->cmds))
		{
			shell->exit_code = wait_children(shell->cmds);//sinon shell recupere pas le code
			clean_post_parser(shell); // inclut clean lexer dedans !
			continue;
		}
		if (shell->flag_exit)
		{
			clean_exit(shell);
			return (shell->exit_code);
		}
		if (shell->cmds)//si on a envoye une liste vide on veut pas changer l'exit code
			shell->exit_code = wait_children(shell->cmds);
		// print_tokens_and_cmds(shell);
		// printf("%s\n", shell->av);
		prepare_next_loop(shell);
	}
	return (shell->exit_code);
}
