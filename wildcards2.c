/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 06:21:24 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/12 11:15:31 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_matching_entries(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	count = 0;
	dir = opendir(".");
	if (!dir)
		return (0);
	entry = readdir(dir);
	while (entry)
	{
		if (match_wildcard_pattern(pattern, entry->d_name))
			count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

int	handle_match_failure(char **matches, int i, DIR *dir)
{
	while (i > 0)
		free(matches[--i]);
	closedir(dir);
	return (0);
}

int	fill_matches_array(char *pattern, char **matches)
{
	DIR				*dir;
	struct dirent	*entry;
	int				i;

	i = 0;
	dir = opendir(".");
	if (!dir)
		return (0);
	entry = readdir(dir);
	while (entry)
	{
		if (match_wildcard_pattern(pattern, entry->d_name))
		{
			matches[i] = ft_strdup(entry->d_name);
			if (!(matches[i]))
				return (handle_match_failure(matches, i, dir));
			i++;
		}
		entry = readdir(dir);
	}
	matches[i] = NULL;
	closedir(dir);
	return (1);
}

char	**allocate_matches(int count)
{
	char	**matches;

	matches = malloc(sizeof(char *) * (count + 1));
	return (matches);
}

char	**expand_wildcard(char *pattern)
{
	int		count;
	char	**matches;

	count = count_matching_entries(pattern);
	matches = allocate_matches(count);
	if (!matches)
		return (NULL);
	if (!fill_matches_array(pattern, matches))
	{
		free(matches);
		return (NULL);
	}
	return (matches);
}
