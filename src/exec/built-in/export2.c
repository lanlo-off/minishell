#include "../../../includes/minishell.h"

t_env	*find_env_node(t_shell *shell, char *key)
{
	t_env	*current;
	int		key_len;

	current = shell->env;
	key_len = ft_strlen(key);
	while (current)
	{
		if (ft_strncmp(current->key, key, key_len + 1) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

static int	ft_strcmp_export(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	sort_tab(char **tab)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (tab[i])
	{
		j = i + 1;
		while (tab[j])
		{
			if (ft_strcmp_export(tab[i], tab[j]) > 0)
			{
				tmp = tab[i];
				tab[i] = tab[j];
				tab[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_and_free_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		ft_putstr_fd(tab[i], 1);
		ft_putchar_fd('\n', 1);
		free(tab[i]);
		i++;
	}
	free(tab);
}

int	get_env_size(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		env = env->next;
		i++;
	}
	return (i);
}
