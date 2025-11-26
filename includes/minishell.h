/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 16:01:15 by llechert          #+#    #+#             */
/*   Updated: 2025/11/26 18:29:39 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "struct.h"
# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

/*=============== EXIT CODES =============== */
# define EXIT_SUCCESS 			0
# define EXIT_FAILURE 			1
# define EXIT_SYNTAX_ERROR		2
# define EXIT_PERMISSION_DENIED 126
# define EXIT_CMD_NOT_FOUND		127
# define EXIT_SIGNAL			128
# define EXIT_CTRL_C 			130
# define EXIT_CTRL_D 			131

/*=============== INIT =============== */

/*init.c*/
void	init_structs(t_shell *shell, char **envp);

/*get_env.c*/
t_env	*get_env(char **envp);


/*=============== LEXER =============== */
/*lexer.c*/
bool	lexer(t_shell *shell, char *line);
bool	append_to_buffer(t_lexer *lexer, char c);
bool	save_token(t_lexer *lexer, t_token **token, t_token_type type);

/*lexer_utils.c*/
void	token_add_last(t_token *new, t_token **list);
bool	is_operator(char c);
bool	check_quotes(t_lexer *lexer, char c);

/*operators.c*/
bool	handle_operator(t_lexer *lexer, char *line, t_token **token_lst);

/*=============== PARSER =============== */

/*=============== EXEC =============== */
/*loop.c*/
int	infinite_loop(t_shell *shell);

#endif