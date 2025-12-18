/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 10:05:29 by llechert          #+#    #+#             */
/*   Updated: 2025/12/16 17:17:57 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	append_cmd_to_shell(t_shell *shell, t_cmd *new_cmd)
{
	t_cmd	*tmp;

	if (!shell || !new_cmd)
		return (false);
	if (!fill_cmd(new_cmd, shell))
		return (false);
	if (!shell->cmds)
	{
		shell->cmds = new_cmd;
		return (true);
	}
	tmp = shell->cmds;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_cmd;
	new_cmd->prev = tmp;
	return (true);
}

static bool	create_cmds(t_shell *shell, t_token *token)
{
	t_cmd	*new;

	new = NULL;
	while (token)
	{
		if (!new && token->type != PIPE)
		{
			new = ft_calloc(1, sizeof(t_cmd));
			if (!new)
				return (false);
		}
		if (token->type == PIPE)
		{
			if (new && (new->av || new->redirs_in || new->redirs_out)
				&& !append_cmd_to_shell(shell, new))
				return (clean_cmd(new), false);
			new = NULL;
		}
		else if (!manage_word_and_redir(token, new, shell))
			return (clean_cmd(new), false);
		get_next_useful_token(&token, token->type);
	}
	if (new && (new->av || new->redirs_in || new->redirs_out))
		return (append_cmd_to_shell(shell, new));
	return (free(new), true);
}

static bool	check_redir(t_token **token_lst, t_shell *shell)
{
	t_token	*tmp;
	bool	prev_redir;

	prev_redir = false;
	tmp = *token_lst;
	while (tmp)
	{
		if (prev_redir == true && tmp->type != WORD)
			return (print_error_parser("<<, <, > or >>", shell), false);
		else if (tmp->type == WORD || tmp->type == PIPE)
			prev_redir = false;
		else
			prev_redir = true;
		tmp = tmp->next;
	}
	if (prev_redir)
		return (print_error_parser("<<, <, > or >>", shell), false);
	return (true);
}

static bool	check_pipes(t_token **token_lst, t_shell *shell)
{
	t_token	*tmp;
	bool	prev_pipe;

	prev_pipe = false;
	tmp = *token_lst;
	if (tmp->type == PIPE)
		return (print_error_parser("|", shell), false);
	tmp = tmp->next;
	while (tmp)
	{
		if (tmp->type == PIPE)
		{
			if (prev_pipe == true)
				return (print_error_parser("|", shell), false);
			prev_pipe = true;
		}
		else
			prev_pipe = false;
		tmp = tmp->next;
	}
	if (prev_pipe)
		return (print_error_parser("|", shell), false);
	return (true);
}

bool	parser(t_shell *shell, t_token **token_lst)
{
	if (!shell || !token_lst || !*token_lst)
		return (true);
	if (!check_pipes(token_lst, shell))
		return (false);
	if (!check_redir(token_lst, shell))
		return (false);
	if (!create_cmds(shell, *token_lst))
	{
		shell->exit_code = 1;
		return (false);
	}
	return (true);
}
