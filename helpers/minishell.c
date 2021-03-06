/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytaya <ytaya@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 09:05:52 by ytaya             #+#    #+#             */
/*   Updated: 2022/03/10 13:54:23 by ytaya            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_nbdollar(char *str)
{
	int	c;

	c = 0;
	str = ft_strchrq(str, '$');
	while (str)
	{
		str++;
		if (!g_cmd.q.d_q && (*str == 34 || *str == 39))
			continue ;
		if (((*str == 32 || *str == '$') && g_cmd.q.d_q) || !ft_isalnum(*str))
			return (-1);
		c++;
		str = ft_strchrq(str, '$');
	}
	return (c);
}

char	*ft_expandall(char *str)
{
	int		nb;
	char	*result;

	nb = get_nbdollar(str);
	result = str;
	if (nb == -1)
		return (str);
	while (nb--)
		result = ft_expand(result);
	return (result);
}

int	ft_check_syntax(char *str)
{
	unsigned int	i;

	i = 0;
	while (i < ft_strlen(str))
	{
		if (str[i] == 34)
		{
			i++;
			while (str[i] && str[i] != 34)
				i++;
			if (str[i] != 34)
				return (-1);
		}
		if (str[i] == 39)
		{
			i++;
			while (str[i] && str[i] != 39)
				i++;
			if (str[i] != 39)
				return (-1);
		}
		i++;
	}
	return (0);
}

char	**add_env(char **envp)
{
	unsigned int	i;
	char			**ret;

	i = 0;
	while (envp[i])
		i++;
	g_cmd.env_len = i;
	ret = (char **) malloc(sizeof(char *) * (g_cmd.env_len + 1));
	i = 0;
	while (envp[i])
	{
		ret[i] = ft_strdup(envp[i], 0);
		i++;
	}
	ret[i] = NULL;
	return (ret);
}
