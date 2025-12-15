#include "../../includes/minishell.h"

int check_args(char **av, int nbArgs)
{
	int count;

	count = 0;
	while (av[count])
		count++;
	if (count <= nbArgs)
		return (count);
	return (-1);
}
