/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 16:01:11 by llechert          #+#    #+#             */
/*   Updated: 2026/01/05 13:22:53 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include "minishell.h"
# include <stdbool.h>
# include <sys/types.h>

/*=============== LEXER =============== */

typedef enum e_lex_state
{
	DEFAULT,
	SQUOTE,
	DQUOTE,
}							t_lex_state;

typedef struct s_lexer
{
	char					*source;
	int						pos;
	char					*buffer;
	int						buffer_size;
	t_lex_state				state;
	bool					lex_error;
}							t_lexer;

typedef enum e_token_type
{
	WORD,
	PIPE,
	HEREDOC,
	REDIR_IN,
	APPEND,
	REDIR_OUT,
}							t_token_type;

typedef enum e_sub_type
{
	SUB_UNQUOTED,
	SUB_SQUOTED,
	SUB_DQUOTED,
}							t_sub_type;

typedef struct s_subword	t_subword;

struct						s_subword
{
	t_sub_type				type;
	char					*val;
	t_subword				*next;
};

typedef struct s_token
{
	t_token_type			type;
	char					*full_token;
	t_subword				*subword;
	struct s_token			*next;
}							t_token;

typedef struct s_sub_manager
{
	char					*str;
	t_token					*token;
	int						pos;
	int						sub_len;
	t_lex_state				state;
}							t_sub_manager;

/*=============== PARSER =============== */

typedef struct s_redir		t_redir;
struct						s_redir
{
	t_token_type			type;
	char					*file;
	int						hd_fd;
	t_sub_type				file_quote_type;
	t_redir					*next;
};

typedef struct s_cmd		t_cmd;
struct						s_cmd
{
	char					**av;
	t_redir					*redirs_in;
	t_redir					*redirs_out;
	char					*path;
	bool					path_to_free;
	int						fd_in;
	int						fd_out;
	pid_t					pid;
	int						exit_status;
	t_cmd					*next;
	t_cmd					*prev;
};

/*=============== ENV =============== */

typedef struct s_env
{
	char					*key;
	char					*value;
	struct s_env			*prev;
	struct s_env			*next;
}							t_env;

/*=============== SHELL =============== */

typedef struct s_shell
{
	t_env					*env;
	char					*av;
	t_token					*token;
	t_cmd					*cmds;
	bool					flag_exit;
	int						exit_code;
	char					**envp;
}							t_shell;

typedef enum e_err_type
{
	ERR_EXEC,
	ERR_REDIR,
	ERR_PIPE,
	ERR_FORK,
	ERR_CMD_NOT_FOUND,
	ERR_BUILTIN
}							t_err_type;

#endif