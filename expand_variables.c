/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:49:56 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/11 23:22:10 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_status(char *result, int exit_status)
{
	char	*status_str;
	char	*temp;

	status_str = ft_itoa(exit_status);
	if (!status_str)
		return (free(result), NULL);
	temp = ft_strjoin(result, status_str);
	free(result);
	free(status_str);
	return (temp);
}

char	*expand_var_name(char *token_value, int *i)
{
	int		start;
	int		end;
	char	*var_name;

	start = *i + 1;
	end = start;
	while (token_value[end] && ((token_value[end] >= 65
				&& token_value[end] <= 90) || (token_value[end] >= 97
				&& token_value[end] <= 122) || (token_value[end] >= 48
				&& token_value[end] <= 57) || token_value[end] == '_'))
		end++;
	var_name = ft_strndup(&token_value[start], end - start);
	*i = end;
	return (var_name);
}

char	*expand_env_var(char *result, char *var_name)
{
	char	*var_value;
	char	*temp;

	var_value = getenv(var_name);
	if (var_value)
		temp = ft_strjoin(result, var_value);
	else
		temp = ft_strjoin(result, "");
	free(result);
	return (temp);
}

char	*append_char(char *result, char c)
{
	char	*temp;

	temp = ft_strjoin_char(result, c);
	free(result);
	return (temp);
}

char	*expand_variables(char *token_value, char **envp, int exit_status)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	(void)envp;
	if (!result)
		return (NULL);
	while (token_value[i])
	{
		if (token_value[i] == '$')
			result = handle_dollar_sign(token_value, &i, result, exit_status);
		else
		{
			result = append_char(result, token_value[i]);
			i++;
		}
		if (!result)
			return (NULL);
	}
	return (result);
}
// char	*expand_variables(char *token_value, char **envp, int exit_status)
// {
// 	int		i;
// 	char	*result;
// 	char	*temp;
// 	char	*var_name;
// 	char	*var_value;
// 	char	*status_str;
// 	int		start;
// 	int		end;

// 	(void)envp;
// 	i = 0;
// 	result = ft_strdup("");
// 	if (!result)
// 		return (NULL);
// 	while (token_value[i])
// 	{
// 		if (token_value[i] == '$')
// 		{
// 			if (token_value[i + 1] && token_value[i + 1] == '?')
// 			{
// 				status_str = ft_itoa(exit_status);
// 				if (!status_str)
// 				{
// 					free(result);
// 					return (NULL);
// 				}
// 				temp = ft_strjoin(result, status_str);
// 				free(result);
// 				free(status_str);
// 				result = temp;
// 				i += 2;
// 			}
// 			else if ((token_value[i + 1] >= 'A' && token_value[i + 1] <= 'Z')
// 				|| (token_value[i + 1] >= 'a' && token_value[i + 1] <= 'z')
// 				|| token_value[i + 1] == '_')
// 			{
// 				start = i + 1;
// 				end = start;
// 				while (token_value[end] && ((token_value[end] >= 'A'
// 							&& token_value[end] <= 'Z')
// 						|| (token_value[end] >= 'a' && token_value[end] <= 'z')
// 						|| (token_value[end] >= '0' && token_value[end] <= '9')
// 						|| token_value[end] == '_'))
// 					end++;
// 				var_name = malloc(end - start + 1);
// 				if (!var_name)
// 				{
// 					free(result);
// 					return (NULL);
// 				}
// 				ft_strncpy(var_name, &token_value[start], end - start);
// 				var_name[end - start] = '\0';
// 				var_value = getenv(var_name);
// 				if (var_value)
// 					temp = ft_strjoin(result, var_value);
// 				else
// 					temp = ft_strdup(result);
// 				free(var_name);
// 				free(result);
// 				result = temp;
// 				i = end;
// 			}
// 			else
// 			{
// 				temp = ft_strjoin_char(result, token_value[i]);
// 				free(result);
// 				result = temp;
// 				i++;
// 			}
// 		}
// 		else
// 		{
// 			temp = ft_strjoin_char(result, token_value[i]);
// 			free(result);
// 			result = temp;
// 			i++;
// 		}
// 	}
// 	return (result);
// }