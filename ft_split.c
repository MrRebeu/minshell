/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 08:14:45 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/12 06:49:55 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*word_dup(const char *str, int len)
{
	char	*word;
	int		i;

	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static int	count_words(const char *str, char delimiter)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (*str != delimiter && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*str == delimiter)
		{
			in_word = 0;
		}
		str++;
	}
	return (count);
}

static int	add_word(char **split, const char *str, int len, int *i)
{
	split[*i] = word_dup(str, len);
	if (!split[*i])
	{
		while (*i > 0)
			free(split[--(*i)]);
		free(split);
		return (0);
	}
	(*i)++;
	return (1);
}

char	**split_words(const char *str, char delimiter, char **split, int *i)
{
	int			word_len;
	const char	*word_start = NULL;

	word_len = 0;
	while (*str)
	{
		if (*str != delimiter && word_len++ == 0)
			word_start = str;
		else if (*str == delimiter && word_len > 0)
		{
			if (!add_word(split, word_start, word_len, i))
				return (NULL);
			word_len = 0;
		}
		str++;
	}
	if (word_len > 0)
	{
		if (!add_word(split, word_start, word_len, i))
			return (NULL);
	}
	split[*i] = NULL;
	return (split);
}

char	**ft_split(const char *str, char delimiter)
{
	int		i;
	int		num_words;
	char	**split;

	i = 0;
	num_words = count_words(str, delimiter);
	split = (char **)malloc(sizeof(char *) * (num_words + 1));
	if (!split)
		return (NULL);
	if (!split_words(str, delimiter, split, &i))
	{
		free(split);
		return (NULL);
	}
	return (split);
}
