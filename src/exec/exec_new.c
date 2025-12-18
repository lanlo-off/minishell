/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:55:40 by llechert          #+#    #+#             */
/*   Updated: 2025/12/18 13:16:47 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	set_normal_fds(t_cmd *cmd, int pipefd[2])
{
	if (cmd->fd_out < 0) 
		cmd->fd_out = STDOUT_FILENO;
	if (cmd->fd_in < 0)
		cmd->fd_in = STDIN_FILENO;
	if (cmd->next)
	{
		if (pipe(pipefd) == -1)
		{
			print_error(NULL, errno, ERR_PIPE, cmd);
			return (false);
		}
		cmd->fd_out = pipefd[1];
	}
	return (true);
}

void	exec_cmd(t_cmd *cmd, char **envp, t_shell *shell)
{
	if (cmd->av && cmd->av[0] && !cmd->av[0][0])//cmd existe mais le premier av est vide
	{
		print_error("", 0, ERR_CMD_NOT_FOUND, cmd);
		exit_fork(cmd, shell);
	}
	if (is_builtin(cmd))
	{
		cmd->exit_status = exec_builtin(cmd, shell);
		exit_fork(cmd, shell); // on free tout + close fd_in et out ? puis fait un exit dedans pour kill l'enfant
	}
	execve(cmd->path, cmd->av, envp);
	print_error(cmd->path, errno, ERR_EXEC, cmd);
	// perror("execve");
	exit_fork(cmd, shell); // exit fork aussi car faut kill ce processus ??
}

bool	do_cmd(t_cmd *cmd, t_shell *shell, int pipefd[2])
{
	g_signal_received = 2;
	cmd->pid = fork();
	if (cmd->pid == -1)
		return (print_error(NULL, errno, ERR_FORK, cmd), false);
	else if (cmd->pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		(dup2(cmd->fd_in, STDIN_FILENO), dup2(cmd->fd_out, STDOUT_FILENO));
		close_fds_ptr(&pipefd[0], &pipefd[1]);
		close_fds_ptr(&cmd->fd_in, &cmd->fd_out);
		exec_cmd(cmd, shell->envp, shell);
		return (false); // si j'arrive ici c'est qu'execve a fail
	}
	else
	{
		if (cmd->next)
		{
			close_fds_ptr(&pipefd[1], NULL);
			cmd->next->fd_in = pipefd[0];
		}
		close_fds_ptr(&cmd->fd_in, &cmd->fd_out);
		close_fds_ptr(&pipefd[0], &pipefd[1]);
		return (true);
	}
}

static bool	handle_single_cmd(t_cmd *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = 0;
	saved_stdout = 0;
	if (!cmd)
		return (true);
	if (!handle_fds_single_cmd(cmd, shell))
		return (false);
	if (!is_builtin(cmd)) //si on doit gerer une seule cmd hors builtin
		return (fork_single_cmd(cmd, shell));
	else                                                      
		//Si on doit gerer un seul builtin
		single_builtin(cmd, saved_stdin, saved_stdout, shell);
			//void qui exec puis remplit cmd->exit_status;
	return (cmd->exit_status == 0);                           
	// return 1 = true si ca a fonctionne, 0 = false sinon
}

/**
 * @brief boucle d'execution des commandes successives
 * Chaque boucle se decompose :
 * 1
	- Set les fds classiques et pipe (si pipe fail on arrete toute la chaine de cmd)
 * 2 - Modifie les fds en cas de redirection
 * 3 - Check la cmd (path entier, nom de la commande etc...)
 * 4 - Tente de lancer la commande
 * On passe toujours a la cmd suivante meme si fail une des etapes 
 * SAUF POUR ERREUR DE PIPE QUI ARRETE TOUTE LA CHAINE DE CMD
 *
 * @param shell
 * @param cmd_lst
 * @param env
 * @return true
 * @return false
 */
bool	execution(t_shell *shell, t_cmd *cmd_lst)
{
	t_cmd	*cmd;
	int		pipefd[2];

	if (!cmd_lst)
		return (true);
	pipefd[0] = -1;
	pipefd[1] = -1;
	cmd = cmd_lst;
	if (!cmd->next && !cmd->prev)
		return (handle_single_cmd(cmd, shell));
	while (cmd)
	{
		if (!set_normal_fds(cmd, pipefd))
			return (false);
		if (!handle_redir_in(cmd, cmd->redirs_in, shell, pipefd)
			|| !handle_redir_out(cmd, cmd->redirs_out)
			|| !check_cmd(cmd) || !do_cmd(cmd, shell, pipefd))
		{
			close_fds_ptr(&pipefd[0], &pipefd[1]);
			close_fds_ptr(&cmd->fd_in, &cmd->fd_out);
			cmd = cmd->next;
			continue ;
		}
		cmd = cmd->next;
	}
	return (true);
}
