/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 16:01:15 by llechert          #+#    #+#             */
/*   Updated: 2025/12/22 18:34:00 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "struct.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

extern int	g_signal_received;

/*=============== EXIT CODES =============== */
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define EXIT_SYNTAX_ERROR 2
# define EXIT_PERMISSION_DENIED 126
# define EXIT_CMD_NOT_FOUND 127
# define EXIT_SIGNAL 128
# define EXIT_CTRL_C 130
# define EXIT_CTRL_D 131

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
void		init_signals(void);
void		init_structs(t_shell *shell, char **envp);
bool		fill_cmd(t_cmd *cmd, t_shell *shell);
char		*get_path(char *cmd, t_env *env);

/*get_env.c*/
t_env		*get_env(char **envp);

/*=============== LEXER =============== */
/*clean_lexer.c*/
void		clean_post_lexer(t_shell *shell);
void		clean_lexer_struct(t_lexer *lexer);

/*lexer.c*/
bool		lexer(t_shell *shell, char *line);

/*operators.c*/
bool		is_operator(char c);
bool		handle_operator(t_lexer *lexer, char *line, t_token **token_lst);

/*quotes.c*/
bool		check_quotes(t_token **token_list);

/*split_sub_utils.c*/
void		set_sub_manager(t_sub_manager *man, char *str, t_token *token);
bool		handle_default_sub(t_sub_manager *man);
bool		handle_squote_sub(t_sub_manager *man);
bool		handle_dquote_sub(t_sub_manager *man);

/*split_subwords.c*/
bool		split_subwords(t_token **token_list);
bool		create_sub(t_token *token, int end, int *length, t_sub_type type);

/*split_token.c*/
bool		append_to_buffer(t_lexer *lexer, char c);
bool		save_token(t_lexer *lexer, t_token **token, t_token_type type);

/*=============== PARSER =============== */
/*clean_parser.c*/
void		clean_cmd(t_cmd *cmd);
void		clean_post_parser(t_shell *shell);

/*expand_utils.c*/
bool		has_quoted_subword(t_subword *sub);
void		get_next_useful_token(t_token **token, t_token_type type);
char		*expand_var(char *str, int *i, t_shell *shell);
char		*get_expanded_var(char *var, t_env **env, t_shell *shell);

/*expand.c*/
char		*join_and_free(char *s1, char *s2);
char		*append_until_doll(char *str, int *i, char *res);
char		*expanded(char *str, t_sub_type type, t_shell *shell);

/*parse.c*/
bool		parser(t_shell *shell, t_token **token_lst);

/*token_to_cmd.c*/
bool		manage_word_and_redir(t_token *token, t_cmd *cmd, t_shell *shell);

/*utils.c*/
t_sub_type	get_file_quote_type(t_subword *sub_lst);

/*=============== EXEC =============== */

/*built-in*/
int			ft_pwd(t_cmd *cmd);
int			ft_cd(t_cmd *cmd, t_shell *shell);
int			ft_exit(t_cmd *cmd, t_shell *t_shell);
int			ft_export(t_cmd *cmd, t_shell *shell);
bool		export_var(t_shell *shell, char *arg);
int			ft_unset(t_cmd *cmd, t_shell *shell);
int			ft_env(t_cmd *cmd, t_shell *shell);
int			ft_echo(char **args);

/*export2.c*/
t_env		*find_env_node(t_shell *shell, char *key);
void		sort_tab(char **tab);
void		print_and_free_tab(char **tab);
int			get_env_size(t_env *env);

/*clean_loop.c*/
void		prepare_next_loop(t_shell *shell);

/*exec_utils.c*/
bool		check_cmd(t_cmd *cmd);
bool		is_builtin(t_cmd *cmd);
int			exec_builtin(t_cmd *cmd, t_shell *shell);
void		close_fds(int fd_in, int fd_out);
void		close_fds_ptr(int *fd_in, int *fd_out);

/*exec.c*/
void		exec_cmd(t_cmd *cmd, char **envp, t_shell *shell);
bool		execution(t_shell *shell, t_cmd *cmd_lst);

/*here_doc.c*/
bool		create_heredoc(t_cmd *cmd, t_redir *redir, t_shell *shell, int pipefd[2]);

/*loop.c*/
int			infinite_loop(t_shell *shell);

/*redir.c*/
bool		is_std_fd(int fd);
int			open_outfile(char *file, t_token_type type, t_cmd *cmd);
bool		handle_redir_out(t_cmd *cmd, t_redir *redir_lst);
int			open_infile(char *file, t_cmd *cmd);
bool		handle_redir_in(t_cmd *cmd, t_redir *redir_lst, t_shell *shell, int pipefd[2]);

/*single_cmd_utils.c*/
void		single_builtin(t_cmd *cmd, int saved_in, int saved_out,
				t_shell *shell);
bool		fork_single_cmd(t_cmd *cmd, t_shell *shell);
bool		handle_fds_single_cmd(t_cmd *cmd, t_shell *shell);

/*utils*/
int			check_args(char **av, int nbArgs);
int			rl_sigint_hook(void);

/*=============== FREE_ERROR =============== */

/*clean.c*/
void		clean_exit(t_shell *shell);
void		clean_shell(t_shell *shell);

/*clean_fork.c*/
void		exit_fork(t_cmd *cmd, t_shell *shell);

/*free_lists.c*/
void		free_env(t_env *env);
void		heredoc_sigint_handler(int sig);

/*print_error.c*/
int			print_error(char *name, int err, t_err_type type, t_cmd *cmd);
int			print_error_parser(char *token, t_shell *shell);

/*=============== DEBUG A VIRER AVANT PUSH =============== */
void		print_tokens_and_cmds(t_shell *shell);

#endif