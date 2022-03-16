/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleger <fleger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 19:07:03 by fleger            #+#    #+#             */
/*   Updated: 2019/12/18 00:19:27 by fleger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/get_next_line.hpp"

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*tab;

	i = 0;
	while (s1[i])
		i++;
	if (!(tab = (char*)malloc(sizeof(char) * (i + 1))))
		return (NULL);
	i = 0;
	while (s1[i])
	{
		tab[i] = s1[i];
		i++;
	}
	tab[i] = '\0';
	return (tab);
}

size_t	ft_strlen(const char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*tab;
	int		i;
	int		j;

	j = -1;
	i = -1;
	if (!s1)
		return (ft_strdup(s2));
	if (!(tab = (char*)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1))))
		return (NULL);
	while (s1[++i])
		tab[i] = s1[i];
	while (s2[++j])
		tab[j + i] = s2[j];
	tab[i + j] = '\0';
	return (tab);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*tab;
	size_t	i;

	i = 0;
	if (ft_strlen(s) < start)
		return (NULL);
	if (!(tab = (char*)malloc(len + 1)))
		return (NULL);
	while (s[start] && i < len)
		tab[i++] = s[start++];
	tab[i] = '\0';
	return (tab);
}
