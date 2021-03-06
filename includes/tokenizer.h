/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytaya <ytaya@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 09:06:32 by ytaya             #+#    #+#             */
/*   Updated: 2022/03/02 08:33:50 by ytaya            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

typedef struct s_token
{
	enum
	{
		TOKEN_PIPE,
		TOKEN_GTEHN,
		TOKEN_LTEHN,
		TOKEN_LHEREDOC,
		TOKEN_RHEREDOC,
		TOKEN_WORD,
		TOKEN_UNKNOWN
	}e_type;
	char	*value;
}t_token;

#endif
