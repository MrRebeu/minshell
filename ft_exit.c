#include "minishell.h"

int ft_exit(t_tree_node *root)
{
    int exit_code = 0;
    int i = 0;

    if (root->cmd->args[1])
    {
        i = 0;
        while (root->cmd->args[1][i])
        {
            if (root->cmd->args[1][i] < '0' || root->cmd->args[1][i] > '9')
            {
                printf("minishell: exit: %s: numeric argument required\n", root->cmd->args[1]);
                exit_code = 2;
                break;
            }
            i++;
        }
        if (exit_code != 2)
            exit_code = ft_atoi(root->cmd->args[1]);
        if (root->cmd->args[2] && exit_code != 2)
        {
            printf("minishell: exit: too many arguments\n");
            return 1;
        }
    }
    printf("exit\n");
    rl_clear_history();
    exit(exit_code);
    return (exit_code);
}
