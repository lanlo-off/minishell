/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 13:34:09 by llechert          #+#    #+#             */
/*   Updated: 2026/01/05 14:13:58 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*concat_expand_token(t_subword *subword, t_shell *shell)
{
	char		*res;
	char		*exp;
	char		*tmp;
	t_subword	*sub;

	sub = subword;
	res = NULL;
	while (sub)
	{
		tmp = res;
		exp = expanded(sub->val, sub->type, shell);
		if (!exp)
			return (NULL);
		if (!res)
			res = ft_strdup(exp);
		else
			res = ft_strjoin(tmp, exp);
		free(tmp);
		free(exp);
		sub = sub->next;
	}
	return (res);
}

static bool	handle_redir_in_token(t_token *token, t_cmd *cmd, t_shell *shell)
{
	t_redir	*new_redir;
	t_redir	*tmp;

	new_redir = ft_calloc(1, sizeof(t_redir));
	if (!new_redir)
		return (false);
	new_redir->type = token->type;
	new_redir->file_quote_type = get_file_quote_type(token->next->subword);
	new_redir->file = concat_expand_token(token->next->subword, shell);
	new_redir->hd_fd = -1;
	if (!cmd->redirs_in)
		cmd->redirs_in = new_redir;
	else
	{
		tmp = cmd->redirs_in;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_redir;
	}
	return (true);
}

static bool	handle_redir_out_token(t_token *token, t_cmd *cmd, t_shell *shell)
{
	t_redir	*new_redir;
	t_redir	*tmp;

	new_redir = ft_calloc(1, sizeof(t_redir));
	if (!new_redir)
		return (false);
	new_redir->type = token->type;
	new_redir->file = concat_expand_token(token->next->subword, shell);
	new_redir->hd_fd = -1;
	if (!cmd->redirs_out)
		cmd->redirs_out = new_redir;
	else
	{
		tmp = cmd->redirs_out;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_redir;
	}
	return (true);
}

static bool	handle_word_token(t_token *token, t_cmd *cmd, t_shell *shell)
{
	int		size;
	char	**new_av;
	int		i;
	char	*expanded;

	size = 0;
	i = 0;
	while (cmd->av && cmd->av[size])
		size++;
	expanded = concat_expand_token(token->subword, shell);
	if ((!expanded || !expanded[0]) && !has_quoted_subword(token->subword))
		return (free(expanded), true);
	new_av = ft_calloc(size + 2, sizeof(char *));
	if (!new_av)
		return (free(expanded), false);
	while (i < size)
	{
		new_av[i] = cmd->av[i];
		i++;
	}
	new_av[size] = expanded;
	if (cmd->av)
		free(cmd->av);
	cmd->av = new_av;
	return (true);
}

bool	manage_word_and_redir(t_token *token, t_cmd *cmd, t_shell *shell)
{
	if (token->type == WORD)
		return (handle_word_token(token, cmd, shell));
	else if (token->type == REDIR_IN || token->type == HEREDOC)
		return (handle_redir_in_token(token, cmd, shell));
	else
		return (handle_redir_out_token(token, cmd, shell));
}
