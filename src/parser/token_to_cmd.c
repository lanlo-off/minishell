/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 13:34:09 by llechert          #+#    #+#             */
/*   Updated: 2025/12/03 19:12:38 by llechert         ###   ########.fr       */
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
		exp = expanded(sub->val, sub->type, shell);//si on alloc dans expanded
		if (!exp)
			return (NULL);
		if (!res)//pour le premier passage
			res = ft_strdup(exp);
		else
			res = ft_strjoin(tmp, exp);
		free(tmp);
		free(exp);
		sub = sub->next;
	}
	return (res);
}

static bool	handle_redir_token(t_token *token, t_cmd *cmd, t_shell *shell)
{
	t_redir	*new_redir;
	t_redir	*tmp;
	
	new_redir = ft_calloc(1, sizeof(t_redir));
	if (!new_redir)
		return (false);
	new_redir->type = token->type;
	new_redir->file = concat_expand_token(token->next->subword, shell);//il faut recuperer le nom du fichier (et pour ca il faut concatener tous les subwords apres les avoir expand)
	if (!cmd->redirs)//s'il n'y a pas encore de redirections on dit que le new est le premier
		cmd->redirs = new_redir;
	else//sinon on rajoute cette redirection a la fin de la liste chainee des redirs
	{
		tmp = cmd->redirs;
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

	size = 0;
	i = 0;
	while (cmd->av && cmd->av[size])
		size++;
	new_av = ft_calloc(size + 2, sizeof(char *));
	if (!new_av)
		return (false);
	while (i < size)
	{
		new_av[i] = cmd->av[i];
		i++;
	}
	new_av[size] = concat_expand_token(token->subword, shell);
	if (!new_av[size])//attention car il peut etre NULL alors que ca fonctionne si le token est vide ?
		return (false);
	if (cmd->av)
		free(cmd->av);
	cmd->av = new_av;
	return (true);
}

bool	manage_word_and_redir(t_token *token, t_cmd *cmd, t_shell *shell)
{
	if (token->type == WORD)
		return (handle_word_token(token, cmd, shell));
	else//sous entendu c'est une redir car ca ne peut pas etre PIPE ici
		return (handle_redir_token(token, cmd, shell));//attention car on traite 2 tokens (le token redir et le token suivant qui donne le file)
}