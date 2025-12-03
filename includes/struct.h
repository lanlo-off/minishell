/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 16:01:11 by llechert          #+#    #+#             */
/*   Updated: 2025/12/03 19:02:23 by llechert         ###   ########.fr       */
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
	SQUOTE,
	DQUOTE,
}	t_lex_state;

typedef struct s_lexer
{
	char		*source;//chaine entree dans minishell
	int			pos;//index
	char		*buffer;
	int			buffer_size;
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

typedef struct s_subword t_subword;

struct s_subword
{
	t_sub_type	type;
	char		*val;
	t_subword	*next;
};

typedef struct s_token
{
	t_token_type	type;
	char			*full_token;
	t_subword		*subword;
	struct s_token	*next;
}	t_token;

typedef struct s_sub_manager//pour split les subwords
{
	char		*str;
	t_token		*token;
	int			pos;
	int			sub_len;
	t_lex_state	state;
}	t_sub_manager;

/*=============== PARSER =============== */

// typedef enum e_redir_type
// {
// 	HEREDOC,
// 	REDIR_IN,
// 	APPEND,
// 	REDIR_OUT,
// }	t_redir_type;

typedef struct s_redir t_redir;
struct s_redir
{
	t_token_type	type;
	char			*file;
	t_redir			*next;
};

typedef struct s_cmd
{
	char	**av;//liste des arguments avec un NULL pour terminer
	t_redir	*redirs;//liste chainee des redirections (il faut les faire dans l'ordre d'apparition)
}	t_cmd;

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
	char	*av;//input brut
	t_token	*token;
	t_cmd	**cmds;
	int		exit_code;
}	t_shell;

#endif