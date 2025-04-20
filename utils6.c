/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:38:34 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/12 06:45:38 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*init_cmd(void)
{
	t_command	*new_cmd;

	new_cmd = malloc(sizeof(t_command));
	if (!new_cmd)
		return (NULL);
	new_cmd->args = NULL;
	new_cmd->next = NULL;
	new_cmd->input_file = NULL;
	new_cmd->output_file = NULL;
	new_cmd->heredoc_limiter = NULL;
	new_cmd->append = 0;
	return (new_cmd);
}

void	add_cmd_to_list(t_command **list, t_command *new_cmd)
{
	t_command	*tmp;

	if (!*list)
	{
		*list = new_cmd;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_cmd;
}

void	add_to_args(t_command *cmd, char *arg)
{
	int		i;
	char	**new;

	i = 0;
	while (cmd->args && cmd->args[i])
		i++;
	new = malloc(sizeof(char *) * (i + 2));
	if (!new)
		return ;
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		new[i] = cmd->args[i];
		i++;
	}
	new[i++] = ft_strdup(arg);
	new[i] = NULL;
	free(cmd->args);
	cmd->args = new;
}

char	*handle_dollar_sign(char *token_value, int *i, char *result,
		int exit_status)
{
	char	*var_name;

	if (token_value[*i + 1] == '?')
	{
		result = expand_status(result, exit_status);
		*i = *i + 2;
	}
	else if ((token_value[*i + 1] >= 65 && token_value[*i + 1] <= 90)
		|| (token_value[*i + 1] >= 97 && token_value[*i + 1] <= 122)
		|| token_value[*i + 1] == '_')
	{
		var_name = expand_var_name(token_value, i);
		if (!var_name)
			return (free(result), NULL);
		result = expand_env_var(result, var_name);
		free(var_name);
	}
	else
	{
		result = append_char(result, token_value[*i]);
		(*i)++;
	}
	return (result);
}
