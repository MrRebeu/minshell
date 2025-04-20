#include "minishell.h"

void find_cmd_path(t_tree_node *cmd, char **env)
{
    int i;
	int j;
	char	*full_path;
	char	*path;
	i = 0;
    j = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
        i++;
    if (env[i])
        cmd->path_dir = ft_split(env[i] + 5, ':');
    else
        cmd->path_dir = NULL;
    cmd->cmd_paths = malloc(sizeof(char *) * cmd->cmd_count);
    i = 0;
    while (i < cmd->cmd_count)
    {
        cmd->cmd_paths[i] = NULL;
        j = 0;
        while (cmd->path_dir && cmd->path_dir[j])
        {
            full_path = ft_strjoin(cmd->path_dir[j], "/");
            path = ft_strjoin(full_path, cmd->cmd_args[i][0]);
            free(full_path);
            if (access(path, X_OK) == 0)
            {
                cmd->cmd_paths[i] = path;
                break;
            }
            free(path);
            j++;
        }
        i++;
    }
}
