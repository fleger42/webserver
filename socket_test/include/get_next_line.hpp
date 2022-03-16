/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleger <fleger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 19:15:22 by fleger            #+#    #+#             */
/*   Updated: 2019/12/02 18:56:09 by fleger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_HPP
# define GET_NEXT_LINE_HPP
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# define BUFFER_SIZE 256

size_t	ft_strlen(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strdup(const char *s1);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		get_next_line(int fd, char **line);
int		verif_chaine(char *str);
char	*readline(char *str, int fd);
#endif
