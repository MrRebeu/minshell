/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:56:08 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/11 23:26:59 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *s)
{
	if (strcmp(s, "echo") == 0 || strcmp(s, "cd") == 0 || strcmp(s, "pwd") == 0
		|| strcmp(s, "export") == 0 || strcmp(s, "unset") == 0 || strcmp(s,
			"env") == 0 || strcmp(s, "exit") == 0)
		return (1);
	return (0);
}

int	is_whitespace(char c)
{
	return (c == 32 || (c >= 9 && c <= 13));
}

int	is_special(char c)
{
	return (c == 124 || c == 60 || c == 62 || c == 39 || c == 34);
}

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	new_token->type = type;
	new_token->value = ft_strdup(value);
	new_token->next = NULL;
	return (new_token);
}

void	add_token(t_token **head, t_token **current, t_token *new_token)
{
	if (!*head)
	{
		*head = new_token;
		*current = new_token;
	}
	else
	{
		(*current)->next = new_token;
		*current = new_token;
	}
}
