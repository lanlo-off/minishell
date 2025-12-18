/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 15:57:14 by llechert          #+#    #+#             */
/*   Updated: 2025/12/15 10:49:39 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal_received = 0;

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	int		exit_code;

	(void)av;
	if (ac != 1)
	{
		printf("./minishell to launch the program");
		return (0);
	}
	init_structs(&shell, envp);
	exit_code = infinite_loop(&shell);
	clear_history();
	clean_shell(&shell);
	return (exit_code);
}
