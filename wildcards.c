/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 07:31:00 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/12 06:54:15 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	match_middle_wildcard(char *pattern, char *filename)
{
	char	*middle;
	int		result;

	middle = ft_substr(pattern, 1, ft_strlen(pattern) - 2);
	if (!middle)
		return (0);
	result = (ft_strstr(filename, middle) != NULL);
	free(middle);
	return (result);
}

int	match_prefix_wildcard(char *pattern, char *filename)
{
	int		pattern_len;
	int		filename_len;
	char	*suffix;
	int		suffix_len;

	pattern_len = ft_strlen(pattern);
	filename_len = ft_strlen(filename);
	if (pattern_len > 1)
	{
		suffix = pattern + 1;
		suffix_len = pattern_len - 1;
		if (filename_len < suffix_len)
			return (0);
		return (ft_strcmp(filename + (filename_len - suffix_len), suffix) == 0);
	}
	return (1);
}

int	match_suffix_wildcard(char *pattern, char *filename)
{
	int	prefix_len;

	prefix_len = ft_strlen(pattern) - 1;
	return (ft_strncmp(pattern, filename, prefix_len) == 0);
}

int	match_middle_asterisk(char *pattern, char *filename)
{
	char	*asterisk;
	int		prefix_len;
	char	*suffix;
	int		suffix_len;
	int		filename_len;

	asterisk = ft_strchr(pattern, '*');
	if (!asterisk)
		return (0);
	prefix_len = asterisk - pattern;
	if (ft_strncmp(pattern, filename, prefix_len) != 0)
		return (0);
	suffix = asterisk + 1;
	suffix_len = ft_strlen(suffix);
	filename_len = ft_strlen(filename);
	if (filename_len < prefix_len + suffix_len)
		return (0);
	return (ft_strcmp(filename + (filename_len - suffix_len), suffix) == 0);
}

int	match_wildcard_pattern(char *pattern, char *filename)
{
	if (ft_strcmp(pattern, "*") == 0)
		return (1);
	if (pattern[0] == '*' && pattern[ft_strlen(pattern) - 1] == '*')
		return (match_middle_wildcard(pattern, filename));
	if (pattern[0] == '*')
		return (match_prefix_wildcard(pattern, filename));
	if (pattern[ft_strlen(pattern) - 1] == '*')
		return (match_suffix_wildcard(pattern, filename));
	if (ft_strchr(pattern, '*'))
		return (match_middle_asterisk(pattern, filename));
	return (ft_strcmp(pattern, filename) == 0);
}
