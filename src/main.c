/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 15:57:14 by llechert          #+#    #+#             */
/*   Updated: 2025/11/20 17:36:49 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	infinite_loop(char **env)
{
	char	*cmd_line;
	
	while (1)
	{
		//check le signal
		cmd_line = readline("minishell> ");
		if (cmd_line && *cmd_line)
			add_history(cmd_line);
		// if(!lexer_cmd(cmd_line))//on imprime l'erreur si besoin dans la fonction
		// 	continue ;//et passe a la ligne
		// if (!parser(cmd_line))//on imprime l'erreur si besoin dans la fonction
		// 	continue ;
		// if (!exec_cmd(cmd_line))
		// 	continue ;
		printf("%s\n", cmd_line);
	}
	free(cmd_line);
	return ;
}

int	main(int ac, char **av, char **env)
{
	if (ac != 1)
	{
		printf("./minishell to launch the program");
		return (0);
	}
	infinite_loop(env);
	return(0);
}
