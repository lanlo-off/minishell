#include "../../../includes/minishell.h"
/**
 * @brief si que exit exit avec code 0 OK
 * si exit + num, exit avec code = num
 * si exit + str + autres -> exit avec code 2 (moi j'ai 255)
 * si exit + num + autres (num/str) -> pas d'exit OK
 *
 * @param cmd
 * @param shell
 * @return true
 * @return false
 */
static int is_numeric(char *str)
{
    int i;

    i = 0;
    if (str[i] == '+' || str[i] == '-')
        i++;
    if (!str[i])
        return (0);
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

int ft_exit(t_cmd *cmd, t_shell *shell)
{
    int lenArgs;

    lenArgs = check_args(cmd->av, 2);
    if (lenArgs == 1)
    {
        shell->flag_exit = true;
        return (0);
    }
    if (lenArgs == 2 && is_numeric(cmd->av[1]))
    {
        shell->flag_exit = true;
        shell->exit_code = ft_atoi(cmd->av[1]);
        return (ft_atoi(cmd->av[1]));
    }
    if (!is_numeric(cmd->av[1]))
    {
        shell->flag_exit = true;
        shell->exit_code = 255;
        return (255);
    }
    if (is_numeric(cmd->av[1]) && cmd->av[2])
        return (1);
    return (0);
}
