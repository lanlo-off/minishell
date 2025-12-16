/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:55:40 by llechert          #+#    #+#             */
/*   Updated: 2025/12/16 17:28:35 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	set_normal_fds(t_cmd *cmd, int pipefd[2])
{
	if (!cmd->prev && !cmd->next)
	{
		cmd->fd_in = STDIN_FILENO;
		cmd->fd_out = STDOUT_FILENO;
	}
	if (cmd->next)
	{
		if (pipe(pipefd) == -1)
		{
			print_error(NULL, errno, ERR_PIPE, cmd);
			return (false);
		}
		if (cmd->prev) // si pas la premiere cmd
			cmd->fd_in = cmd->prev->fd_out;
		else // si premiere commande
			cmd->fd_in = STDIN_FILENO;
		cmd->fd_out = pipefd[1];
	}
	else if (cmd->prev)
	{
		cmd->fd_in = cmd->prev->fd_out;
		cmd->fd_out = STDOUT_FILENO;
	}
	return (true);
}

void	exec_cmd(t_cmd *cmd, char **envp, t_shell *shell)
{
	if (is_builtin(cmd))
	{
		cmd->exit_status = exec_builtin(cmd, shell);
		exit_fork(cmd, shell); // on free tout + close fd_in et out ? puis fait un exit dedans pour kill l'enfant
	}
	// else if (cmd->fd_in >= 0 && cmd->fd_out >= 0)
	execve(cmd->path, cmd->av, envp);
	print_error(cmd->path, errno, ERR_EXEC, cmd);
	// perror("execve");
	exit_fork(cmd, shell); // exit fork aussi car faut kill ce processus ??
}

bool	do_cmd(t_cmd *cmd, t_shell *shell, int pipefd[2])
{
	cmd->pid = fork();
	if (cmd->pid == -1)
		return (false); // exit_bad_fork(cmd), quel comportement ? Fin de la
						// commande et on continue ? Fin de la chaine de cmd ?
	else if (cmd->pid == 0)
	{
		(dup2(cmd->fd_in, STDIN_FILENO), dup2(cmd->fd_out, STDOUT_FILENO));
		// close_fds(pipefd[0], pipefd[1]);
		// close_fds(cmd->fd_in, cmd->fd_out);
		close_fds_ptr(&pipefd[0], &pipefd[1]);
		close_fds_ptr(&cmd->fd_in, &cmd->fd_out);
		exec_cmd(cmd, shell->envp, shell);
		return (false); // si j'arrive ici c'est qu'execve a fail
	}
	else
	{
		if (pipefd[1] >= 0 && !is_std_fd(pipefd[1]))
			close_fds_ptr(&pipefd[1], NULL);//close(pipefd[1]);
		if (cmd->prev && cmd->prev->fd_out != STDOUT_FILENO)
			close_fds_ptr(&cmd->prev->fd_out, NULL);//close(cmd->prev->fd_out);
		if (cmd->fd_in >= 0 && cmd->fd_in != STDIN_FILENO)
			close_fds_ptr(&cmd->fd_in, NULL);//close(cmd->fd_in);
		if (cmd->next)               // si on n'est pas a la derniere commande
			cmd->fd_out = pipefd[0];
				// on ecrit dans le pipe pour envoyer a la prochaine commande
		return (true);
	}
	return (true);
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
	cmd = cmd_lst;
	if (!cmd->next && !cmd->prev)
		return (handle_single_cmd(cmd, shell));
	while (cmd)
	{
		if (!set_normal_fds(cmd, pipefd))               
		//Trouver un moyer d'arreter toutes les commandes
			return (false);                             
			// car faut pas faire les autres commandes quand on a fail le pipe ? Mais comment faire pour ne pas faire les premieres commandes dont le pipe n'a pas fail ?
		if (!handle_redir_in(cmd, cmd->redirs_in, shell)
			|| !handle_redir_out(cmd, cmd->redirs_out)
			|| !check_cmd(cmd) || !do_cmd(cmd, shell, pipefd))
		{
			cmd = cmd->next;
			continue ;
		}
		cmd = cmd->next;
	}
	return (true);
}
