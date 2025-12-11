#include "../../../includes/minishell.h"

bool ft_exit(t_shell *shell)
{
    int lenArgs;

    lenArgs = check_args(shell->cmds->av, 1);
    if (lenArgs == 1)
        shell->flag_exit=true;
    else
        return (false);
    return (true);
}
