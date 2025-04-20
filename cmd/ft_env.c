#include "minishell.h"

int ft_env(char **envp)
{
	int i;

	i = 0;
	if (!envp)
		return 1;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}
