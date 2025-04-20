/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:43:34 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/12 06:48:33 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_word(char *input, int *i, t_token **head, t_token **current)
{
	int		start;
	char	*cmd;
	t_token	*new_token;

	start = *i;
	while (input[*i] && !is_special(input[*i]) && !is_whitespace(input[*i]))
		(*i)++;
	if (start == *i)
		return (0);
	cmd = ft_substr(input, start, *i - start);
	if (!cmd)
		return (0);
	if (cmd && *cmd)
	{
		new_token = create_token(TOKEN_WORD, cmd);
		if (!new_token)
			return (0);
		add_token(head, current, new_token);
	}
	free(cmd);
	return (1);
}

int	handle_quotes(char *input, int *i, t_token **head, t_token **current)
{
	char	quote_type;
	int		start;
	char	*cmd;
	t_token	*new_token;

	quote_type = input[*i];
	start = ++(*i);
	while (input[*i] && input[*i] != quote_type)
		(*i)++;
	if (!input[*i])
		return (0);
	cmd = ft_substr(input, start, *i - start);
	new_token = create_token(TOKEN_WORD, cmd);
	new_token->quote_type = quote_type;
	add_token(head, current, new_token);
	free(cmd);
	(*i)++;
	return (1);
}

int	handle_less_operator(char *input, int *i, t_token **head, t_token **current)
{
	t_token	*new_token;

	new_token = NULL;
	if (input[*i] == 60)
	{
		if (input[*i + 1] == 60)
		{
			new_token = create_token(TOKEN_HEREDOC, "<<");
			*i = *i + 2;
		}
		else
		{
			new_token = create_token(TOKEN_REDIR_IN, "<");
			(*i)++;
		}
	}
	if (!new_token)
		return (0);
	add_token(head, current, new_token);
	return (1);
}

int	handle_greater_operator(char *input, int *i, t_token **head,
		t_token **current)
{
	t_token	*new_token;

	new_token = NULL;
	if (input[*i] == 62)
	{
		if (input[*i + 1] == 62)
		{
			new_token = create_token(TOKEN_APPEND, ">>");
			*i = *i + 2;
		}
		else
		{
			new_token = create_token(TOKEN_REDIR_OUT, ">");
			(*i)++;
		}
	}
	if (!new_token)
		return (0);
	add_token(head, current, new_token);
	return (1);
}

int	handle_redirection(char *input, int *i, t_token **head, t_token **current)
{
	if (input[*i] == 60)
		return (handle_less_operator(input, i, head, current));
	else if (input[*i] == 62)
		return (handle_greater_operator(input, i, head, current));
	return (0);
}
