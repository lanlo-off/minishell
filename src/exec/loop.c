/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:05:29 by llechert          #+#    #+#             */
/*   Updated: 2025/12/09 22:38:56 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	wait_children(t_cmd *cmd_lst)
{
	t_cmd	*cmd;
	int		code;

	if (!cmd_lst)
		return (0);
	cmd = cmd_lst;
	while (cmd->next)
	{
		waitpid(cmd->pid, cmd->exit_status, 0);
		cmd = cmd->next;
	}
	waitpid(cmd->pid, cmd->exit_status, 0);//pour le dernier
	code = cmd->exit_status;//on recupere le dernier exit code avant de le rendre intelligible ci dessous
	if (WIFEXITED(code))
		return (WEXITSTATUS(code));
	else if (WIFSIGNALED(code))
		return (128 + WTERMSIG(code));
	else
		return (1);//pq return 1 et pas 0 a cet endroit ?
}

int	infinite_loop(t_shell *shell)
{
	while (1)
	{
		//check le signal a intervalles frequents ?
		shell->av = readline("AU SUIVANT> ");
		if (shell->av && *shell->av)
			add_history(shell->av);
		if(!lexer(shell, shell->av))//si pb, on imprime erreur dans lexer
		{
			clean_post_lexer(shell);//on prepare la prochaine boucle en faisant free
			continue ;//et passe a la boucle suivante
		}
		if (!parser(shell, &shell->token))//on imprime l'erreur si besoin dans la fonction
		{
			clean_post_parser(shell);//inclut clean lexer dedans !
			continue ;
		}
		if (!execution(shell, shell->cmds, shell->env))
			continue ;
		shell->exit_code = wait_children(shell->cmds);
		// printf("%s\n", shell->av);
		// print_tokens_and_cmds(shell);
		prepare_next_loop(shell);
	}
	return (shell->exit_code);
}
