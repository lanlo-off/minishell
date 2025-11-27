/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:04:45 by llechert          #+#    #+#             */
/*   Updated: 2025/11/27 18:31:51 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"





// bool	handle_quotes(t_lexer *lexer, char *line)
// {
// 	int	i;

// 	i = lexer->pos;
// 	if (line[i] == '\'' && lexer->state == DEFAULT)
// 	{
// 		if (!check_quotes(lexer, '\''))
// 			return(false);
// 		lexer->state = SQUOTE;
// 	}
// 	else if (line[i] == '\'' && lexer->state == SQUOTE)
// 		lexer->state = DEFAULT;
// 	else if (line[i] == '"' && lexer->state == DEFAULT)
// 		lexer->state = DQUOTE;
// 	else if (line[i] == '"' && lexer->state == DQUOTE)
// 		lexer->state = DEFAULT;
// }