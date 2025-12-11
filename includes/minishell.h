/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 16:01:15 by llechert          #+#    #+#             */
/*   Updated: 2025/12/11 15:05:00 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "struct.h"
# include "../libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <editline/readline.h> // POUR MACOS

/*=============== EXIT CODES =============== */
# define EXIT_SUCCESS			0
# define EXIT_FAILURE			1
# define EXIT_SYNTAX_ERROR		2
# define EXIT_PERMISSION_DENIED	126
# define EXIT_CMD_NOT_FOUND		127
# define EXIT_SIGNAL			128
# define EXIT_CTRL_C			130
# define EXIT_CTRL_D			131


/*=============== COLORS =============== */
# define RESET "\033[0m"
# define GREEN "\033[1;32m"
# define RED "\033[1;31m"
# define GRAY "\033[1;90m"
# define CYAN "\033[1;36m"


/*=============== OTHERS =============== */
# define PATH_MAX 4096


/*=============== INIT =============== */
/*init.c*/
void	init_structs(t_shell *shell, char **envp);
bool	fill_cmd(t_cmd *cmd, t_shell *shell);
char	*get_path(char *cmd, t_env	*env);

/*get_env.c*/
t_env	*get_env(char **envp);


/*=============== LEXER =============== */
/*clean_lexer.c*/
void	clean_post_lexer(t_shell *shell);
void	clean_lexer_struct(t_lexer *lexer);

/*lexer.c*/
bool	lexer(t_shell *shell, char *line);

/*operators.c*/
bool	is_operator(char c);
bool	handle_operator(t_lexer *lexer, char *line, t_token **token_lst);

/*quotes.c*/
bool	check_quotes(t_token **token_list);

/*split_sub_utils.c*/
void	set_sub_manager(t_sub_manager *man, char *str, t_token *token);
bool	handle_default_sub(t_sub_manager *man);
bool	handle_squote_sub(t_sub_manager *man);
bool	handle_dquote_sub(t_sub_manager *man);

/*split_subwords.c*/
bool	split_subwords(t_token **token_list);
bool	create_sub(t_token *token, int end, int *length, t_sub_type type);

/*split_token.c*/
bool	append_to_buffer(t_lexer *lexer, char c);
bool	save_token(t_lexer *lexer, t_token **token, t_token_type type);



/*=============== PARSER =============== */
/*clean_parser.c*/
void	clean_cmd(t_cmd *cmd);
void	clean_post_parser(t_shell *shell);

/*expand_utils.c*/
void	get_next_useful_token(t_token **token, t_token_type type);
char	*expand_var(char *str, int *i, t_shell *shell);
char	*get_expanded_var(char *var, t_env **env, t_shell *shell);

/*expand.c*/
char	*join_and_free(char *s1, char *s2);
char	*append_until_doll(char *str, int *i, char *res);
char	*expanded(char *str, t_sub_type type, t_shell *shell);

/*parse.c*/
bool	parser(t_shell *shell, t_token **token_lst);

/*token_to_cmd.c*/
bool	manage_word_and_redir(t_token *token, t_cmd *cmd, t_shell *shell);

/*utils.c*/
t_sub_type	get_file_quote_type(t_subword *sub_lst);

/*=============== EXEC =============== */
/*exec.c*/
//static void	exec_cmd(t_cmd *cmd, t_env *env);
bool is_builtin(char *cmd);
void exec_builtin(t_shell *shell);

/*pipe.c*/
void exec_pipeline(t_shell *shell);

/*redirections.c*/
bool handle_redirections(t_cmd *cmd);

/*built-in*/
bool ft_pwd(char **av);
bool ft_cd(t_shell *shell);
bool ft_exit(t_shell *t_shell);
bool ft_export(t_shell *shell);
bool export_var(t_shell *shell, char *arg);
bool ft_unset(t_shell *shell);
bool ft_env(t_shell *shell);
bool ft_echo(char **args);

/*utils*/
int check_args(char **av, int nbArgs);

/*loop.c*/
int		infinite_loop(t_shell *shell);

/*clean_loop.c*/
void	prepare_next_loop(t_shell *shell);

/*exec0.c*/
bool	execution(t_shell *shell, t_cmd *cmd_lst);

/*loop.c*/
int		infinite_loop(t_shell *shell);

/*redir.c*/
bool	is_std_fd(int fd);
int		open_outfile(char *file, t_token_type type);
bool	handle_redir_out(t_cmd *cmd, t_redir *redir_lst);
int		open_infile(char *file);
bool	handle_redir_in(t_cmd *cmd, t_redir *redir_lst, t_shell *shell);

/*here_doc.c*/
bool	create_heredoc(t_cmd *cmd, t_redir *redir, t_shell *shell);

/*clean.c*/
void    clean_exit(t_shell *shell);
void    clean_shell(t_shell *shell);

/*free_lists.c*/
void free_env(t_env *env);

/*=============== DEBUG A VIRER AVANT PUSH =============== */
void	print_tokens_and_cmds(t_shell *shell);


#endif