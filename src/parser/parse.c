/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 10:05:29 by llechert          #+#    #+#             */
/*   Updated: 2025/12/04 15:20:55 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	append_cmd_to_shell(t_shell *shell, t_cmd *new_cmd)
{
	t_cmd	*tmp;

	if (!shell || !new_cmd)
		return (false);
	new_cmd->next = NULL;//redondant a priori
	if (!shell->cmds)
	{
		shell->cmds = new_cmd;
		return (true);
	}
	tmp = shell->cmds;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_cmd;
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
			if (new && (new->av || new->redirs) && !append_cmd_to_shell(shell, new))
				return (clean_cmd(new), false);
			new = NULL;
		}
		else if (!manage_word_and_redir(token, new, shell))
			return (clean_cmd(new), false);
		if (token->type != WORD && token->type != PIPE)
			token = token->next;
		token = token->next;
	}
	if (new && (new->av || new->redirs))
		return (append_cmd_to_shell(shell, new));
	return (free(new), true);
}

// static bool	create_cmds(t_shell *shell, t_token **token_lst)
// {
// 	t_token	*token;
// 	t_cmd	*new;
	
// 	token = *token_lst;
// 	new = ft_calloc(1, sizeof(t_cmd));
// 	if (!new)
// 		return (false);
// 	while (token)
// 	{
// 		if (token->type == PIPE)
// 		{
// 			append_cmd_to_shell(shell, new);
// 			new = ft_calloc(1, sizeof(t_cmd));
// 			if (!new)
// 				return (false);
// 		}
// 		else
// 		{
// 			if (!manage_word_and_redir(token, new, shell))
// 				return (false);
// 		}
// 		token = token->next;
// 	}
// 	return (append_cmd_to_shell(shell, new), true);
// }

static bool	check_redir(t_token **token_lst)
{
	t_token	*tmp;
	bool	prev_redir;

	prev_redir = false;
	tmp = *token_lst;
	while (tmp)
	{
		if (prev_redir == true && tmp->type != WORD)
			return (ft_putstr_fd("ERROR NO WORD AFTER REDIR\n", 2), false);
		else if (tmp->type == WORD || tmp->type == PIPE)
			prev_redir = false;
		else
			prev_redir = true;
		tmp = tmp->next;
	}
	if (prev_redir)
		return (ft_putstr_fd("ERROR NO WORD AFTER REDIR\n", 2), false);
	return (true);
}

static bool	check_pipes(t_token **token_lst)
{
	t_token	*tmp;
	bool	prev_pipe;//true si le token precedent est un type (verifie pas de double pipe)

	prev_pipe = false;
	tmp = *token_lst;
	if (tmp->type == PIPE)
		return (ft_putstr_fd("ERROR PIPE IN FIRST\n", 2), false);
	tmp = tmp->next;
	while (tmp)
	{
		if (tmp->type == PIPE)
		{
			if (prev_pipe == true)
				return (ft_putstr_fd("ERROR DOUBLE PIPE\n", 2), false);
			prev_pipe = true;
		}
		else
			prev_pipe = false;
		tmp = tmp->next;
	}
	if (prev_pipe)
		return (ft_putstr_fd("ERROR PIPE IN LAST\n", 2), false);
	return (true);
}

bool	parser(t_shell *shell, t_token **token_lst)
{
	if (!shell || !token_lst || !*token_lst)
		return (true);
	if (!check_pipes(token_lst))//pas d'alloc ici
		return (false);//Message d'erreur dans check pipes
	if (!check_redir(token_lst))//pas d'alloc ici
		return (false);//Message d'erreur dans check pipes
	if (!create_cmds(shell, *token_lst))
	{
		ft_putstr_fd("ERROR CREATING CMDS\n", 2);
		return (false);
	}
	return (true);
}
