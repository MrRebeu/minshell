#include "minishell.h"

int ft_pwd(void)
{
    char buff[PATH_MAX];
    
    if (getcwd(buff, PATH_MAX) != NULL)
    {
        printf("%s\n", buff);
        return (0);
    }
    else
    {
        perror("minishell: pwd");
        return (1);
    }
}
