/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytaya <ytaya@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 14:10:07 by ytaya             #+#    #+#             */
/*   Updated: 2022/03/07 07:55:21 by ytaya            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_expand(char *str)
{
	int		i;
	int		j;
	char	*result[3];

	i = get_dollar_index(str);
	result[0] = ft_exapnd_if1(i, str);
	if (str[i++] == '$')
	{
		if (str[i] == '?' && i++)
			result[1] = ft_itoa(g_cmd.exit_code);
		else if (str[i] == '\0')
			result[1] = ft_strdup("$", 1);
		else if (ft_isdigit(str[i]))
		{
			j = i + 1;
			result[1] = ft_expand_else1(&i, str, j);
		}
		else
		{
			j = i;
			result[1] = ft_expand_else2(&i, str, j);
		}
		result[2] = ft_strdup(&str[i], 1);
	}
	if (result[0] && result[1] && result[2])
		return (ft_expand_else3(result[0], result[1], result[2]));
	return (NULL);
}

t_list *ft_expand_tokens(t_list *tokens)
{
	t_list *head;

	head = tokens;
	// tokens = g_cmd.tokens;

	while (tokens)
	{
		if (((t_token *)tokens->content)->e_type == 5)
			printf("%d\n",get_nbdollar(((t_token *)tokens->content)->value));
		tokens = tokens->next;
	}
	return (head);
}