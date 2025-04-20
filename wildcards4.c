/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 06:27:44 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/12 06:55:03 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_args_with_wildcards(t_command *cmd)
{
	int		count;
	char	**new_args;

	count = count_total_args(cmd);
	new_args = create_new_args_array(cmd, count);
	if (!new_args)
		return ;
	free_string_array(cmd->args);
	cmd->args = new_args;
}

int	contain_wildcard(char *str)
{
	return (ft_strchr(str, '*') != NULL);
}
