#include "minishell.h"


int ft_cd(t_tree_node *root, char **envp)
{
	struct stat buf;
	char old_pwd[PATH_MAX];
	char new_pwd[PATH_MAX];
	char *home;
	char *oldpwd_ptr;

    if (getcwd(old_pwd, PATH_MAX) == NULL)
    {
        perror("minishell: getcwd");
        return (1);
    }

    if (root->cmd->args[1] == NULL)
    {
        home = getenv("HOME");
        if (home == NULL)
        {
            write(2, "minishell: cd: HOME not set\n", 28);
            return 1;
        }
        else if (chdir(home) != 0)
        {
            perror("minishell: cd");
            return (1);
        }
    }
    else if (root->cmd->args[1][0] == '-' && root->cmd->args[1][1] == '\0')
    {
        char *oldpwd_ptr = getenv("OLDPWD");
        if (oldpwd_ptr == NULL)
        {
            write(2, "minishell: cd: OLDPWD not set\n", 30);
            return 1;
        }
        if (chdir(oldpwd_ptr) != 0)
        {
            perror("minishell: cd");
            return (1);
        }
        printf("%s\n", oldpwd_ptr);
    }
    else
    {
        if (chdir(root->cmd->args[1]) != 0)
        {
            perror("minishell: cd");
            return (1);
        }
    }
    if (getcwd(new_pwd, PATH_MAX) != NULL)
    {
        set_env_var(&envp, "PWD", new_pwd);
        set_env_var(&envp, "OLDPWD", old_pwd);
    }

    return (0);
}
