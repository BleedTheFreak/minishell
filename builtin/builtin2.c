/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-ghem <ael-ghem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 00:41:28 by ytaya             #+#    #+#             */
/*   Updated: 2022/03/11 15:25:47 by ael-ghem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_export_check_params(char *param, int fd)
{
	char	**ret;

	(void)fd;
	if (param)
	{
		ret = get_name_value(param);
		if (ret == NULL)
			return (1);
		if (!check_export_input((ret)[0]))
		{
			printf("minishell : export: `%s': not a valid identifier\n", param);
			return (1);
		}
	}
	return (0);
}

static int	pipe_find(char *s)
{
	int	i;

	i = 0;
	if (s)
	{
		while (s[i])
		{
			if (s[i] == '|')
				return (i);
			i++;
		}
	}
	return (0);
}

static char	*get_newparam(char *s)
{
	char	*result;
	int		i;
	int		j;

	j = 0;
	i = 0;
	result = xmalloc(sizeof(char) * ft_strlen(s) + 3);
	while (s[i] && s[i] != '|')
		result[j++] = s[i++];
	result[j++] = '\'';
	result[j++] = s[i++];
	result[j++] = '\'';
	while (s[i])
		result[j++] = s[i++];
	result[j] = '\0';
	return (result);
}

void	ft_export_helper(char *param)
{
	int				i;
	char			**ret;

	g_cmd.env_len++;
	ret = (char **) malloc(sizeof(char *) * (g_cmd.env_len + 2));
	i = -1;
	while (g_cmd.env_p[++i])
		ret[i] = ft_strdup(g_cmd.env_p[i], 0);
	if (pipe_find(param))
		ret[i++] = get_newparam(param);
	else
		ret[i++] = ft_strdup(param, 0);
	ret[i] = NULL;
	ft_free_env(&g_cmd.env_p);
	g_cmd.env_p = ret;
}

int	len_env(char **l)
{
	int	i;

	i = 0;
	while (l && l[i])
		i++;
	return (i);
}
