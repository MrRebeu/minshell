/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:57:59 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/16 17:29:01 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(const char *s, size_t len)
{
	char	*dup;
	size_t	i;

	i = 0;
	dup = (char *)malloc(len + 1);
	if (!dup)
		return (NULL);
	while (i < len && s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

t_tree_node	*create_cmd_node(t_command *cmd)
{
	t_tree_node	*new_node;

	new_node = malloc(sizeof(t_tree_node));
	if (!new_node)
		return (NULL);
	new_node->cmd = cmd;
	new_node->type = NODE_COMMAND;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

t_tree_node	*create_operator_node(t_node_type type, t_tree_node *left,
		t_tree_node *right)
{
	t_tree_node	*new_node;

	new_node = malloc(sizeof(t_tree_node));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->left = left;
	new_node->right = right;
	new_node->cmd = NULL;
	return (new_node);
}
