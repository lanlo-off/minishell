/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 16:01:15 by llechert          #+#    #+#             */
/*   Updated: 2025/11/24 14:52:24 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "struct.h"
# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

/*=============== EXIT CODES =============== */
# define EXIT_SUCCESS 			0
# define EXIT_FAILURE 			1
# define EXIT_SYNTAX_ERROR		2
# define EXIT_PERMISSION_DENIED 126
# define EXIT_CMD_NOT_FOUND		127
# define EXIT_SIGNAL			128
# define EXIT_CTRL_C 			130
# define EXIT_CTRL_D 			131

/*lexer.c*/



#endif