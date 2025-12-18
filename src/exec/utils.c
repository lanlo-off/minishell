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