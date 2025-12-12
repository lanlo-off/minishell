/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:55:40 by llechert          #+#    #+#             */
/*   Updated: 2025/12/12 18:45:26 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	close_fds(int fd_in, int fd_out)
{
	if (fd_in >= 0 && !is_std_fd(fd_in))
		close(fd_in);
	if (fd_out >= 0 && is_std_fd(fd_out))
		close(fd_out);
}

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
			perror("pipe");
			return (false);
		}
		if (cmd->prev)//si pas la premiere cmd
			cmd->fd_in = cmd->prev->fd_out;
		else//si premiere commande
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
/**
 * @brief Pour verifier si on l'envoie a execve et sinon imprimer le message d'erreur
 * 
 * @param cmd 
 * @return true 
 * @return false 
 */
static bool	check_cmd(t_cmd *cmd)
{
	if (!cmd->av)
	{
		perror("Could not split cmd");
		return (false);
	}
	if (!cmd->av[0])
	{
		perror("Command empty");
		return (false);
	}
	if (!cmd->path)
	{
		ft_putendl_fd("path not found", 2);
		return (false);
	}
	return (true);
}

static void	exec_cmd(t_cmd *cmd, char **envp, t_shell *shell)
{
	if (is_builtin(cmd))
	{
		cmd->exit_status = exec_builtin(cmd, shell);
		exit_fork(cmd, shell);//on free tout + close fd_in et out ? puis fait un exit dedans pour kill l'enfant
	}
	else if (cmd->fd_in >= 0 && cmd->fd_out >= 0)
		execve(cmd->path, cmd->av, envp);
	perror("execve");
	//exit_error(tab_cmds, args, args->nb_cmd, 127);
}

static bool	do_cmd(t_cmd *cmd, t_shell *shell, int pipefd[2])
{
	cmd->pid = fork();
	if (cmd->pid == -1)
		return (false);//exit_bad_fork(cmd), quel comportement ? Fin de la commande et on continue ? Fin de la chaine de cmd ?
	else if (cmd->pid == 0)
	{
		(dup2(cmd->fd_in, STDIN_FILENO), dup2(cmd->fd_out, STDOUT_FILENO));
		close_fds(pipefd[0], pipefd[1]);
		close_fds(cmd->fd_in, -1);
		exec_cmd(cmd, shell->envp, shell);
		return (false);//si j'arrive ici c'est qu'execve a fail
	}
	else
	{
		if (pipefd[1] >= 0 && !is_std_fd(pipefd[1]))
			close(pipefd[1]);
		if (cmd->prev && cmd->prev->fd_out != STDOUT_FILENO)
			close(cmd->prev->fd_out);
		if (cmd->fd_in >= 0 && cmd->fd_in != STDIN_FILENO)
			close (cmd->fd_in);
		if (cmd->next)//si on n'est pas a la derniere commande
			cmd->fd_out = pipefd[0];//on ecrit dans le pipe pour envoyer a la prochaine commande
		return (true);
	}
	return (true);
}

static bool	handle_single_cmd(t_cmd *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	
	if (!cmd)
		return (true);
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	handle_redir_in(cmd, cmd->redirs_in, shell);
	handle_redir_out(cmd, cmd->redirs_out);
	if (!is_builtin(cmd))
	{
		cmd->pid = fork();
		if (cmd->pid == -1)
			return (false);//exit_bad_fork(cmd), quel comportement ? Fin de la commande et on continue ? Fin de la chaine de cmd ?
		else if (cmd->pid == 0)
		{
			(dup2(cmd->fd_in, STDIN_FILENO), dup2(cmd->fd_out, STDOUT_FILENO));
			if (check_cmd(cmd))
				exec_cmd(cmd, shell->envp, shell);
			return (false);//car check_cmd ou execve a fail
		}
		else
		{
			if (cmd->prev && cmd->prev->fd_out != STDOUT_FILENO)
				close(cmd->prev->fd_out);
			if (cmd->fd_in >= 0 && cmd->fd_in != STDIN_FILENO)
				close (cmd->fd_in);
			return (true);
		}
	}
	else//faut stocker les STD(dup), puis dup2, puis executer, puis restaurer(dup2), puis fermer
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		if (cmd->fd_in >= 0)
			dup2(cmd->fd_in, STDIN_FILENO);
		if (cmd->fd_out >= 0)
			dup2(cmd->fd_out, STDOUT_FILENO);
		cmd->exit_status = exec_builtin(cmd, shell);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		if (cmd->fd_in >= 0 && cmd->fd_in != STDIN_FILENO)
			close(cmd->fd_in);
		if (cmd->fd_out >= 0 && cmd->fd_out != STDOUT_FILENO)
			close(cmd->fd_out);
	}
	return (cmd->exit_status == 0);//return 1 = true si ca a fonctionne, 0 = false sinon
}

/**
 * @brief boucle d'execution des commandes successives
 * Chaque boucle se decompose 
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
	{
		handle_single_cmd(cmd, shell);
		cmd = cmd->next;
		return (true);
	}
	while (cmd)
	{
		if (!set_normal_fds(cmd, pipefd))
			return (false) ;// car faut pas faire les autres commandes quand on a fail le pipe ? Mais comment faire pour ne pas faire les premieres commandes dont le pipe n'a pas fail ?
		if (!handle_redir_in(cmd, cmd->redirs_in, shell))//Manage Heredoc ici et impression du message d'erreur si necessaire
		{
			continue ;// ou return (false) ?
		}	
		if (!handle_redir_out(cmd, cmd->redirs_out))
		{
			continue ;// ou return (false) ?
		}	
		//Si juste des redir et pas de cmd -> gerer les redir (HD et creation des outfile) et ne rien faire ensuite
		if (!check_cmd(cmd))
			continue ;
		if (!do_cmd(cmd, shell, pipefd))
			continue ;// ou return (false) ?
		cmd = cmd->next;
	}
	return (true);
}
