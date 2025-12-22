/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 18:26:42 by llechert          #+#    #+#             */
/*   Updated: 2025/12/22 18:26:43 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	ft_pwd(t_cmd *cmd)
{
	char	*cwd;

	(void)cmd;
	cwd = getcwd(NULL, PATH_MAX);
	if (!cwd)
	{
		perror("minishell: pwd");
		return (EXIT_FAILURE);
	}
	else
		ft_putendl_fd(cwd, 1);
	free(cwd);
	return (EXIT_SUCCESS);
}
