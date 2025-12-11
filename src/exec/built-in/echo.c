#include "../../../includes/minishell.h"

bool flag_nl(char *str)
{
	int	i;

	i = 1;
	if (str[0] != '-' || str[1] != 'n')
		return (false);
	while (str[i] == 'n') // verif si besoin de casser
		i++;
	if (str[i] != '\0')
		return (false);
	return (true);
}

bool ft_echo(char **args)
{
  int i;
  bool print_newline;

  i = 1;
  print_newline = true;
  while (args[i] && flag_nl(args[i])) {
    print_newline = false;
    i++;
  }
  while (args[i]) {
    ft_putstr_fd(args[i], 1);
    if (args[i + 1])
      ft_putstr_fd(" ", 1);
    i++;
  }
  if (print_newline)
    ft_putstr_fd("\n", 1);
  return (true);
}
