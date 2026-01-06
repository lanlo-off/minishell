/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 18:25:59 by llechert          #+#    #+#             */
/*   Updated: 2026/01/06 11:16:31 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_args(char **av, int nbArgs)
{
	int	count;

	count = 0;
	while (av[count])
		count++;
	if (count <= nbArgs)
		return (count);
	return (-1);
}

int	rl_sigint_hook(void)
{
	if (g_signal_received == 67)
	{
		rl_replace_line("", 0);
		rl_done = 1;
	}
	return (0);
}

void	init_value(int *pipe_0, int *pipe_1)
{
	*pipe_0 = -1;
	*pipe_1 = -1;
}
