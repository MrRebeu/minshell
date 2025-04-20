#include "minishell.h"

int ft_export(t_tree_node *root, char **envp)
{
    int i;
    char *equals_pos;
    int name_len;
    char *var_name;
    char *var_value;

    i = 1;
    if (!root->cmd->args[1])
		return (ft_env(envp));
	while (root->cmd->args[i])
	{
		equals_pos = ft_strchr(root->cmd->args[i], '=');
		if (equals_pos)
		{
			name_len = equals_pos - root->cmd->args[i];
			var_name = malloc(sizeof(char) * (name_len + 1));
			if (!var_name)
				return (1);
			ft_strncpy(var_name, root->cmd->args[i], name_len);
			var_name[name_len] = '\0';
			// La valeur commence juste après le signe égal
			var_value = equals_pos + 1;
			// Mettre à jour ou ajouter la variable dans l'environnement
			set_env_var(&envp, var_name, var_value);
			free(var_name);
		}
		i++;
	}
	return (0);
}
