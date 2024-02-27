/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:40:41 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/27 12:13:07 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 *	Returns the index of the next quote into the given str, starting from i
 *	If no quote is found, returns the index of the end of str
 */
static size_t	pipex_split_get_end_quotes_idx(char *str, size_t i, char quote)
{
	size_t	j;

	j = i;
	while (str[j] != '\0' && str[j] != quote)
		j++;
	return (j);
}

/*
 *	Extracts the chunks from the given string and stores them into res NULL
 *	terminated array
 *	Quotes are taken into account
 *	Returns NULL on error
 */
static char	**pipex_split_extract(
	char *s, char **res, size_t	nb_words, char quote
)
{
	size_t	i;
	size_t	j;
	size_t	index_res;

	i = 0;
	index_res = 0;
	while (index_res < nb_words)
	{
		while (s[i] != '\0' && ft_isspace(s[i]))
			i++;
		if (s[i] == quote)
			j = pipex_split_get_end_quotes_idx(s, ++i, quote);
		else
		{
			j = i;
			while (s[j] != '\0' && !ft_isspace(s[j]))
				j++;
		}
		res[index_res] = ft_substr(s, i, (j - i));
		if (res[index_res] == NULL)
			return (NULL);
		i = j + 1;
		index_res++;
	}
	return (res);
}

/*
 *	Sets given str to the character right after the next quote
 *	If no quote is found, str pointer reaches the end of the given string
 */
static void	pipex_split_reach_next_quote(char **str, char quote)
{
	char	*s;

	if (str == NULL)
		return ;
	s = *str;
	while (*s != '\0' && *s != quote)
		s++;
	if (*s == quote)
		s++;
	*str = s;
}

/*
 *	Counts the number of elements into the final splitted array
 *	Takes quote into account
 */
static size_t	pipex_split_count(char *str, char quote)
{
	size_t	count;
	bool	in_word;

	count = 0;
	in_word = false;
	while (*str != '\0')
	{
		if (*str == quote)
		{
			count++;
			str++;
			pipex_split_reach_next_quote(&str, quote);
			in_word = false;
			continue ;
		}
		if (*str != '\0' && !in_word && !ft_isspace(*str))
		{
			count++;
			in_word = true;
		}
		else if (*str && in_word && ft_isspace(*str))
			in_word = false;
		str++;
	}
	return (count);
}

/*
 *	Splits a string into multiple chunks stored in a NULL terminated array
 *	If a quote is encountered, the split takes it into account
 *	Returns NULL on error
 */
char	**pipex_split(char *str, char quote)
{
	char	**res;
	size_t	nb_words;

	nb_words = pipex_split_count(str, quote);
	res = (char **)ft_calloc(nb_words + 1, sizeof(char *));
	if (res == NULL)
		return (NULL);
	if (pipex_split_extract(str, res, nb_words, quote) == NULL)
	{
		ft_free_str_array(&res);
		return (NULL);
	}
	return (res);
}
