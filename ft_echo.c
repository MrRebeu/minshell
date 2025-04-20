#include "minishell.h"

static int is_valid_n_option(char *arg)
{
    int i;

    if (!arg || arg[0] != '-')
        return 0;
    i = 1;
    while (arg[i])
    {
        if (arg[i] != 'n')
            return 0;
        i++;
    }
    return (i > 1);
}

static int check_n_options(char **args)
{
    int i;

    i = 1;
    if (!args || !args[1])
        return 0;
    while (args[i] && is_valid_n_option(args[i]))
        i++;
    return (i - 1);
}
int save_std_fds(int *stdin_copy, int *stdout_copy)
{
    *stdin_copy = dup(STDIN_FILENO);
    *stdout_copy = dup(STDOUT_FILENO);

    if (*stdin_copy == -1 || *stdout_copy == -1)
    {
        perror("minishell: dup");
        if (*stdin_copy != -1)
            close(*stdin_copy);
        if (*stdout_copy != -1)
            close(*stdout_copy);
        return 1;
    }
    return 0;
}

void restore_std_fds(int stdin_copy, int stdout_copy)
{
    if (stdin_copy != -1)
    {
        dup2(stdin_copy, STDIN_FILENO);
        close(stdin_copy);
    }
    
    if (stdout_copy != -1)
    {
        dup2(stdout_copy, STDOUT_FILENO);
        close(stdout_copy);
    }
}
int ft_echo(t_tree_node *root)
{
    int i;
    int n_option;
    int stdin_copy = -1;
    int stdout_copy = -1;

    if (!root->cmd || !root->cmd->args)
        return 1;
    if (root->cmd->input_file || root->cmd->output_file || root->cmd->heredoc_limiter)
    {
        if (save_std_fds(&stdin_copy, &stdout_copy) != 0)
            return 1; // error dans save_std_fds
        setup_redirections(root->cmd);
    }
    n_option = check_n_options(root->cmd->args);
    i = 1 + n_option;
    while (root->cmd->args[i])
    {
        printf("%s", root->cmd->args[i]);
        if (root->cmd->args[i + 1])
            printf(" ");
        i++;
    }
    if (n_option == 0)
        printf("\n");
    restore_std_fds(stdin_copy, stdout_copy);
    return 0;
}
