#include "minishell.h"

char *find_cmd_path(t_tree_node *root, char **env)
{
    int i = 0;
    int j = 0;
    char *full_path;
    char *path;
    char **path_dirs = NULL;
    
    if (strchr(root->cmd->args[0], '/'))
    {
        if (access(root->cmd->args[0], X_OK) == 0)
            return strdup(root->cmd->args[0]);
        return NULL;
    }
    while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
        i++;
    if (env[i])
        path_dirs = ft_split(env[i] + 5, ':');
    else
        return NULL;
    j = 0;
    while (path_dirs[j])
    {
        full_path = ft_strjoin(path_dirs[j], "/");
        path = ft_strjoin(full_path, root->cmd->args[0]);
        free(full_path);
        if (access(path, X_OK) == 0)
        {
            i = 0;
            while (path_dirs[i])
                free(path_dirs[i++]);
            free(path_dirs);
            return path;  // Commande trouvée
        }
        free(path);
        j++;
    }
    i = 0;
    while (path_dirs[i])
        free(path_dirs[i++]);
    free(path_dirs);
    return NULL; 
}
