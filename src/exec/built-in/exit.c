#include "../../../includes/minishell.h"
/**
 * @brief si que exit exit avec code 0
 * si exit + num, exit avec code = num
 * si exit + str + autres -> exit avec code 2
 * si exit + num + autres (num/str) -> pas d'exit
 * 
 * @param cmd 
 * @param shell 
 * @return true 
 * @return false 
 */
int ft_exit(t_cmd *cmd, t_shell *shell)
{
    int lenArgs;

    lenArgs = check_args(cmd->av, 1);
    if (lenArgs == 1)
        shell->flag_exit=true;
    else
        return (false);
    return (true);
}
