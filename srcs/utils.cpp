#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
void	free_double_tab(char **tmp)
{
	for(int i = 0; tmp[i]; i++)
		free(tmp[i]);
	free(tmp);
}
/*
Ajoute une ligne à un char **
*/
char ** add_line_doubletab(char **dtab, const char * newline)
{
	char ** ret;
	int count = 0;
	if(!dtab)
	{
		ret = (char**)malloc(sizeof(char*) * 2);
		ret[0] = strdup(newline);
		ret[1] = NULL;
		return ret;
	}
	while(dtab[count])
		count++;
	ret = (char**)malloc(sizeof(char*) * (count + 2));
	count = 0;
	while(dtab[count])
	{
		ret[count] = strdup(dtab[count]);
		count++;
	}
	ret[count] = strdup(newline);
	ret[count + 1] = NULL;
	for(int i = 0; dtab[i]; i++)
		free(dtab[i]);
	free(dtab);
	return (ret);
}
/*
Isole un bloc du fichier de configuration en mettant un \0 après le } de fermeture
*/
void cutblock(std::string & str)
{
	int i = 0;
	int is_open = 0;
	while(str[i])
	{
		if(str[i] == '{' && is_open == 1)
			is_open = 0;
		else if(str[i] == '{' && is_open == 0)
			is_open = 1;
		if(str[i] == '}' && is_open == 1)
			str.resize(i + 1);
		else if(str[i] == '}' && is_open == 0)
			is_open = 1;
		i++;
	}
}
/*
Isole un bloc du fichier de configuration en mettant un \0 après le } de fermeture
*/
int walk_end_block(std::string const & str)
{
	int i = 0;
	int is_open = 0;
	while(str[i])
	{
		if(str[i] == '{' && is_open == 1)
			is_open = 0;
		else if(str[i] == '{' && is_open == 0)
			is_open = 1;
		if(str[i] == '}' && is_open == 1)
			return (i + 1);
		else if(str[i] == '}' && is_open == 0)
			is_open = 1;
		i++;
	}
	return (0);
}
/*
Compte le nombre de fois qu'apparait le mot str dans file
*/
int count_appearance(std::string file, std::string str)
{
	int count = 0;
	size_t found = 0;
	while(42)
	{
		found = file.find(str, found);
		if(found == std::string::npos)
		{
			return (count);
		}
		file = &file[walk_end_block(&file[found + 1])];
		count++;
	}
	return (count);
}

std::vector<std::string> string_to_double_tab(std::string str)
{
	int i = 0;
	int nbr = 0;
	int line_lenght = 0;
	std::vector<std::string> double_tab;
	while(str[i])
	{
		if(str[i] == '\n' && (str[i + 1] != '\0' || (str[i + 1] != '\n')))
		{
			double_tab.push_back(str.substr(nbr, line_lenght));
			nbr = i + 1;
			line_lenght = -1;
		}

		i++;
		line_lenght++;
	}
	return (double_tab);
}


static unsigned int	ft_get_nb_strs(char const *s, const char *delimiters)
{
	unsigned int	i;
	unsigned int	nb_strs;

	if (!s[0])
		return (0);
	i = 0;
	nb_strs = 0;
	while (s[i] && strchr(delimiters, s[i]))
		i++;
	while (s[i])
	{
		if (strchr(delimiters, s[i]))
		{
			nb_strs++;
			while (s[i] && strchr(delimiters, s[i]))
				i++;
			continue ;
		}
		i++;
	}
	if (!strchr(delimiters, s[i - 1]))
		nb_strs++;
	return (nb_strs);
}

static void			ft_get_next_str(char **next_str, unsigned int *next_str_len,
					const char *delimiters)
{
	unsigned int i;

	*next_str += *next_str_len;
	*next_str_len = 0;
	i = 0;
	while (**next_str && strchr(delimiters, **next_str))
		(*next_str)++;
	while ((*next_str)[i])
	{
		if (strchr(delimiters, (*next_str)[i]))
			return ;
		(*next_str_len)++;
		i++;
	}
}

size_t				ft_strlcpy(char *dst, char const *src, size_t size)
{
	size_t		i;
	size_t		src_len;

	i = 0;
	src_len = strlen(src);
	if (size == 0)
		return (src_len);
	while (src[i] && i < (size - 1))
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (src_len);
}

char				**ft_clear_splitted(char **tab)
{
	unsigned int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

char				**ft_split(char const *s, const char *delimiters)
{
	char			**tab;
	char			*next_str;
	unsigned int	next_str_len;
	unsigned int	nb_strs;
	unsigned int	i;

	if (s == NULL || delimiters == NULL)
		return (NULL);
	nb_strs = ft_get_nb_strs(s, delimiters);
	if (!(tab = (char**)malloc(sizeof(char *) * (nb_strs + 1))))
		return (NULL);
	i = 0;
	next_str = (char *)s;
	next_str_len = 0;
	while (i < nb_strs)
	{
		ft_get_next_str(&next_str, &next_str_len, delimiters);
		if (!(tab[i] = (char*)malloc(sizeof(char) * (next_str_len + 1))))
			return (ft_clear_splitted(tab));
		ft_strlcpy(tab[i], next_str, next_str_len + 1);
		i++;
	}
	tab[i] = NULL;
	return (tab);
}

int ft_strlen(char *str)
{
	int i;
	i = 0;
	while (str[i])
		i++;
	return i;
}

int ft_strlen(const char *str)
{
	int i;
	i = 0;
	while (str[i])
		i++;
	return i;
}

int		ft_nbr_c(int n)
{
	int nbr_c;

	nbr_c = 1;
	if (n == -2147483648)
		return (11);
	if (n < 0)
	{
		nbr_c++;
		n = -n;
	}
	while (n >= 10)
	{
		nbr_c++;
		n = n / 10;
	}
	return (nbr_c);
}

char	*ft_itoa(int n)
{
	char		*str;
	int			length;
	long int	n2;

	n2 = (long int)n;
	length = ft_nbr_c(n);
	if (!(str = (char*)malloc(sizeof(char) * (length + 1))))
		return (NULL);
	if (n2 < 0)
	{
		n2 = -n2;
		str[0] = '-';
	}
	str[length] = '\0';
	while (n2 >= 10)
	{
		str[length-- - 1] = (n2 % 10) + 48;
		n2 = n2 / 10;
	}
	str[length - 1] = n2 + 48;
	return (str);
}