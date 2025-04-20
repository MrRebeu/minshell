#include "minishell.h"

int ft_unset(t_tree_node *root, char **envp)
{
	int i;
	int j;
	int len;
	int argu;

	argu = 1;
	if (!root->cmd->args[1])
		return 0; // pas sur de la valeur de return
	while (root->cmd->args[argu])
	{
		len = ft_strlen(root->cmd->args[1]);
		i = 0;
		while (envp[i])
		{
			if (ft_strncmp(envp[i], root->cmd->args[argu], len) == 0 && envp[i][len] == '=')
			{
				free(envp[i]);
				j = i;
				while (envp[j])
				{
					envp[j] = envp[j + 1];
					j++;
				}
				break;
			}
			i++;
		}
		argu++;
	}
	return (0);
}
