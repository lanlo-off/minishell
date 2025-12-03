/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 10:05:29 by llechert          #+#    #+#             */
/*   Updated: 2025/12/03 19:10:15 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	append_cmd_to_shell(t_shell *shell, t_cmd *cmd)
{
	(void) shell;
	(void) cmd;
	return (true);
}

static bool	create_cmds(t_shell *shell, t_token **token_lst)
{
	t_token	*token;
	t_cmd	*new;
	
	token = *token_lst;
	new = malloc(sizeof(t_cmd));
	if (!new)
		return (false);
	while (token)
	{
		if (token->type == PIPE)
		{
			append_cmd_to_shell(shell, new);//on free new dasn ce truc
			new = malloc(sizeof(t_cmd));
			if (!new)
				return (false);
		}
		else
		{
			if (!manage_word_and_redir(token, new, shell))
				return (false);
		}
		token = token->next;
	}
	return (append_cmd_to_shell(shell, new), true);
}

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
	if (!create_cmds(shell, token_lst))
	{
		ft_putstr_fd("ERROR CREATING CMDS\n", 2);
		return (false);
	}
	return (true);
}
