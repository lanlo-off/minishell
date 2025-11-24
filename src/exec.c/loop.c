/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:05:29 by llechert          #+#    #+#             */
/*   Updated: 2025/11/24 15:06:23 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	infinite_loop(t_shell *shell)
{
	while (1)
	{
		//check le signal a intervalles frequents ?
		shell->av = readline("Dis toujours> ");
		if (shell->av && *shell->av)
			add_history(shell->av);
		if(!lexer(shell, shell->av, shell->lexer))//si pb, on imprime erreur dans lexer et on set le token a NULL pour envoyer la boucle suivante
			continue ;//et passe a la boucle suivante
		// if (!parser(cmd_line))//on imprime l'erreur si besoin dans la fonction
		// 	continue ;
		// if (!exec_cmd(cmd_line, env))
		// 	continue ;
		printf("%s\n", shell->av);
	}
	return (shell->exit_code);
}
