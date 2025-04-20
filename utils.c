#include "minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	if (!s1 || !s2)
		return (s1 == s2 ? 0 : (s1 ? 1 : -1));
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

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
            free((*envp)[i]);
            (*envp)[i] = new;
            return (0);
        }
        i++;
    }
    free(new);
    return (1);
}
