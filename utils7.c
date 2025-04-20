#include "minishell.h"

int set_env_var(char ***envp, const char *name, const char *value)
{
    int i = 0;
    size_t len_name = ft_strlen(name);
    char *new = malloc(sizeof(char) * (len_name + ft_strlen(value) + 2));
    if (!new)
        return (1);

    ft_strcpy(new, name);
    ft_strcat(new, "=");
    ft_strcat(new, value);
    while ((*envp)[i])
    {
        if (ft_strncmp((*envp)[i], name, len_name) == 0 && (*envp)[i][len_name] == '=')
        {
            //free((*envp)[i]); provoque un crash , mais pourquoi , double free ?
            (*envp)[i] = new;
            return (0);
        }
        i++;
    }
    free(new);
    return (1);
}

int	ft_atoi(char *str)
{
	int	i;
	int	res;
	int	sign;

	res = 0;
	i = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || (str[i] == ' '))
		i++;
	while (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

char *my_getenv(char **envp, const char *name)
{
    int i = 0;
    size_t len = strlen(name);
    
    while (envp[i])
    {
        if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
            return &envp[i][len + 1];
        i++;
    }
    return NULL;
}
