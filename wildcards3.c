/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 06:22:47 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/12 06:31:26 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_wildcard_matches(char *arg)
{
	char	**expanded;
	int		j;
	int		count;

	expanded = expand_wildcard(arg);
	if (!expanded)
		return (0);
	count = 0;
	j = 0;
	while (expanded[j])
	{
		count++;
		j++;
	}
	free_string_array(expanded);
	return (count);
}

int	count_total_args(t_command *cmd)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (cmd->args && cmd->args[i])
	{
		if (contain_wildcard(cmd->args[i]))
			count += count_wildcard_matches(cmd->args[i]);
		else
			count++;
		i++;
	}
	return (count);
}

void	free_string_array(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	add_arg_to_array(char *arg, char **new_args, int new_count)
{
	char	**expanded;
	int		j;

	if (contain_wildcard(arg))
	{
		expanded = expand_wildcard(arg);
		if (expanded)
		{
			j = 0;
			while (expanded[j])
			{
				new_args[new_count++] = expanded[j++];
			}
			free(expanded);
		}
	}
	else
	{
		new_args[new_count++] = ft_strdup(arg);
	}
	return (new_count);
}

char	**create_new_args_array(t_command *cmd, int count)
{
	int		i;
	int		new_count;
	char	**new_args;

	new_args = malloc(sizeof(char *) * (count + 1));
	if (!new_args)
		return (NULL);
	i = 0;
	new_count = 0;
	while (cmd->args && cmd->args[i])
	{
		new_count = add_arg_to_array(cmd->args[i], new_args, new_count);
		i++;
	}
	new_args[new_count] = NULL;
	return (new_args);
}
