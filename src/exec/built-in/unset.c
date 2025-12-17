/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 16:40:14 by llechert          #+#    #+#             */
/*   Updated: 2025/12/17 16:42:29 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	check_unset(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

static void	remove_env_node(t_shell *shell, t_env *node)
{
	if (node->prev)
		node->prev->next = node->next;
	else
		shell->env = node->next;
	if (node->next)
		node->next->prev = node->prev;
	free(node->key);
	free(node->value);
	free(node);
}

static void	test_unset(t_shell *shell, int key_len, t_env *current, int i)
{
	while (current)
	{
		if (ft_strncmp(current->key, shell->cmds->av[i], key_len + 1) == 0)
		{
			remove_env_node(shell, current);
			break ;
		}
		current = current->next;
	}
}

int	ft_unset(t_cmd *cmd, t_shell *shell)
{
	int		i;
	t_env	*current;
	int		key_len;

	if (!cmd->av[1])
		return (EXIT_SUCCESS);
	i = 1;
	while (cmd->av[i])
	{
		current = shell->env;
		key_len = ft_strlen(shell->cmds->av[i]);
		if (!check_unset(cmd->av[i]))
			test_unset(shell, key_len, current, i);
		else
			ft_putstr_fd("unset: not a valid identifier\n", 2);
		i++;
	}
	return (EXIT_SUCCESS);
}
