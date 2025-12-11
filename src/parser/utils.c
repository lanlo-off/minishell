/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 12:27:25 by llechert          #+#    #+#             */
/*   Updated: 2025/12/11 11:45:29 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


/**
 * @brief Get the file quote type object : Quoted (SUB_SQUOTED ou SUB_DQUOTED) ou Unquoted
 * Sachant qu'il suffit que l'un des subwords soit S ou Dquoted pour que l'ensemble soit quoted
 * Sert a savoir s'il faut expand le contenu du heredoc ensuite (Quoted = non)
 * 
 * @param sub_lst 
 * @return t_sub_type 
 */
t_sub_type	get_file_quote_type(t_subword *sub_lst)
{
	t_subword	*tmp;

	if (!sub_lst)
		return (SUB_UNQUOTED);
	tmp = sub_lst;
	while (tmp)
	{
		if (tmp->type != SUB_UNQUOTED)
			return (tmp->type);
		tmp = tmp->next;
	}
	return (SUB_UNQUOTED);
}
