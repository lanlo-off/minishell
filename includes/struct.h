/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 16:01:11 by llechert          #+#    #+#             */
/*   Updated: 2025/11/24 15:55:37 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include "minishell.h"
# include <stdbool.h>

/*=============== LEXER =============== */

typedef enum e_lex_state
{
	DEFAULT,
	DQUOTE,
	SQUOTE,
}	t_lex_state;

typedef struct s_lexer
{
	char		*source;//chaine entree dans minishell
	int			pos;//index
	t_lex_state	state;
	bool		lex_error;
}	t_lexer;

typedef enum e_token_type
{
	WORD,
	PIPE,
	HEREDOC,
	REDIR_IN,
	APPEND,
	REDIR_OUT,
}	t_token_type;

typedef enum e_sub_type
{
	SUB_UNQUOTED,
	SUB_SQUOTED,
	SUB_DQUOTED,
}	t_sub_type;

typedef struct s_subword
{
	t_sub_type	type;
	char		*val;
	t_subword	*next;
}	t_subword;

typedef struct s_token
{
	t_token_type	type;
	char			*full_token;
	t_subword		*subword;//null si pas un mot en nature
	struct s_token	*next;
}	t_token;

/*=============== ENV =============== */

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*prev;
	struct s_env	*next;
}	t_env;

/*=============== SHELL =============== */

typedef struct s_shell
{
	t_env	*env;
	char	**av;
	t_token	*token;
	int		exit_code;
}	t_shell;

#endif