/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:07:30 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/12 06:52:06 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*tokenize(char *input)
{
	t_token	*head;
	t_token	*current;
	int		i;
	int		status;
	t_token	*eof_token;

	head = NULL;
	current = NULL;
	i = 0;
	status = 1;
	if (!input)
		return (NULL);
	while (input[i] && status)
		status = handle_token(input, &i, &head, &current);
	if (status)
	{
		eof_token = create_token(TOKEN_EOF, "EOF");
		if (eof_token)
			add_token(&head, &current, eof_token);
	}
	else
		free_tokens(head);
	return (head);
}

int	handle_pipe_or(char *input, int *i, t_token **head, t_token **current)
{
	t_token	*new_token;

	if (input[*i + 1] == '|')
	{
		new_token = create_token(TOKEN_OR, "||");
		*i += 2;
	}
	else
	{
		new_token = create_token(TOKEN_PIPE, "|");
		(*i)++;
	}
	if (!new_token)
		return (0);
	add_token(head, current, new_token);
	return (1);
}

int	handle_and(char *input, int *i, t_token **head, t_token **current)
{
	t_token	*new_token;

	(void)input;
	new_token = create_token(TOKEN_AND, "&&");
	if (!new_token)
		return (0);
	*i = *i + 2;
	add_token(head, current, new_token);
	return (1);
}

int	handle_word_token(char *input, int *i, t_token **head, t_token **current)
{
	if (!handle_word(input, i, head, current))
	{
		if (i != 0 && !is_whitespace(input[*i - 1]) && !is_special(input[*i
					- 1]))
			return (0);
	}
	return (1);
}

int	handle_token(char *input, int *i, t_token **head, t_token **current)
{
	if (is_whitespace(input[*i]))
		(*i)++;
	else if (input[*i] == 124)
		return (handle_pipe_or(input, i, head, current));
	else if (input[*i] == 38 && input[*i + 1] == 38)
		return (handle_and(input, i, head, current));
	else if (input[*i] == 60 || input[*i] == 62)
		return (handle_redirection(input, i, head, current));
	else if (input[*i] == 34 || input[*i] == 39)
		return (handle_quotes(input, i, head, current));
	else
		return (handle_word_token(input, i, head, current));
	return (1);
}
