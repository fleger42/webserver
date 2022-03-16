/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleger <fleger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 17:56:16 by fleger            #+#    #+#             */
/*   Updated: 2019/12/19 06:20:36 by fleger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/get_next_line.hpp"

int		verif_chaine(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*readline(char *str, int fd)
{
	int		ret;
	char	*temp;
	char	buff[BUFFER_SIZE + 1];

	while ((verif_chaine(str) == 0) &&
	((ret = read(fd, buff, BUFFER_SIZE)) > 0))
	{
		buff[ret] = '\0';
		if (!(temp = ft_strjoin(str, buff)))
			return (NULL);
		free(str);
		str = temp;
	}
	if (str == NULL)
		return (ft_strdup(""));
	return (str);
}

char	*ft_realloc(char *str, int k)
{
	char	*temp;

	temp = ft_strdup(&str[k + 1]);
	free(str);
	str = temp;
	return (str);
}

int		get_next_line(int fd, char **line)
{
	static char	*str = NULL;
	int			k;
	char		buff[BUFFER_SIZE];

	k = 0;
	if (fd < 0 || BUFFER_SIZE <= 0 || !line || read(fd, buff, 0) < 0)
		return (-1);
	if (!(str = readline(str, fd)))
		return (-1);
	while (str[k] != '\n' && str[k] != '\0')
		k++;
	if (!(*line = ft_substr(str, 0, k)))
		return (-1);
	if (str[k] == '\0')
	{
		free(str);
		str = NULL;
		return (0);
	}
	else if (str[k] == '\n')
	{
		str = ft_realloc(str, k);
		return (1);
	}
	return (-1);
}
