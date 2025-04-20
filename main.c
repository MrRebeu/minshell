#include "minishell.c"

int execute_external_command(t_tree_node *root, char **env)
{
    pid_t pid;
    int status;
    const char *path;

    fd = 0;
    path = find_cmd_path(root->cmd, env);
    if (!path)
    {
        write(2, "minishell: command not found: ", 30);
        write(2, root->cmd->args[0], strlen(root->cmd->args[0]));
        write(2, "\n", 1);
        return 127; // code pour "commande non trouvée"
    }
    pid = fork();
    if (pid == -1)
    {
        perror("minishell: fork");
        return 1;
    }
    else if (pid == 0)
    {
        if (root->cmd->input_file || root->cmd->output_file || 
            root->cmd->heredoc_limiter)
        {
            setup_redirections(root->cmd);
        }
        if (execve(path, root->cmd->args, env) == -1)
        {
            perror("minishell");
            exit(1);
        }
    }
    else
    {
        waitpid(pid, &status, 0);
        return status;
    }
    return 0;
}

int execute_command(t_tree_node *root, char **env)
{
    if (!cmd || !root->cmd->args || !cmd->args[0])
        return 1;
    if (strcmp(root->cmd->args[0], "echo") == 0)
        return ft_echo(root);
    else if (strcmp(root->cmd->args[0], "cd") == 0)
        return ft_cd(root);
    else if (strcmp(root->cmd->args[0], "pwd") == 0)
        return ft_pwd(root);
    else if (strcmp(root->cmd->args[0], "export") == 0)
        return ft_export(root, env);
    else if (strcmp(root->cmd->args[0], "unset") == 0)
        return ft_unset(root, env);
    else if (strcmp(root->cmd->args[0], "env") == 0)
        return ft_env(env);
    else if (strcmp(cmd->args[0], "exit") == 0)
        return ft_exit(root);
    else
        return execute_external_command(cmd, env);
}

int execute_tree(t_tree_node *root, char **env)
{
    int status;

    if (!root)
        return 0;
    if (root->type == NODE_COMMAND)
    {
        status = execute_command(root->cmd, env);
    }
    else if (root->type == NODE_PIPE)
    {
        status = execute_pipe(root->left, root->right, env);
    }
    else if (root->type == NODE_AND)
    {
        status = execute_tree(root->left, env);
        if (status == 0)
            status = execute_tree(root->right, env);
    }
    else if (root->type == NODE_OR)
    {
        status = execute_tree(root->left, env);
        if (status != 0)
            status = execute_tree(root->right, env);
    }
    return status;
}
int main(int argc, char **argv, char **envp)
{
    char *line;
    t_tree_node *root;
    int status;

    status = 0;
    setup_signals();
    while (1)
    {
        handle_signals();
        line = readline("minishell> ");
        if (!line) // si readline == null alors -> ctrl+D
        {
            printf("\nexit\n");
            break;
        }
        if (*line)
            add_history(line);
        root = parsing(line); // fonction de tobia
        if (root)
            status = execute_tree(root, envp);
        free(line);
        if (root)
            free_tree(root);
    }
    rl_clear_history();
    return status;
}
