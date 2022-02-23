/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytaya <ytaya@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 09:05:52 by ytaya             #+#    #+#             */
/*   Updated: 2022/02/23 22:24:47 by ytaya            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./memwatch-2.71/memwatch.h"

char *ft_expand(char *str)
{
	int i;
	int j;

	char *result[3];
	char *from;
	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	if (i >= 1)
	{
		result[0] = MALLOC(sizeof(char) * (i + 1));
		ft_strlcpy(result[0],str,i + 1);
	}
	else
		result[0] = ft_strdup("", 1);
	if (str[i++] == '$')
	{
		j = i;
		while (str[i] && str[i] != 32 && str[i] != '\"' && str[i] != 39)
			i++;
		from = ft_substr(&str[j],0,i - j);
		// result[1] = ft_strdup(getenv(from));
		result[1] = ft_getenv(g_cmd.env_p,from);
		if (!result[1])
			return (ft_strdup("", 1));
		else if (!result[1])
			return (ft_strdup("", 1));
		result[2] = ft_strdup(&str[i], 1);
	}
	if (result[0] && result[1] && result[2])
	{
		from = ft_strjoin(ft_strjoin(result[0],result[1]),result[2]);
		return (from);
	}
	return (NULL);
}

int get_nbdollar(char *str)
{
	int c;

	c = 0;
	str = ft_strchrq(str,'$');
	while (str)
	{
		str++;
		if (*str == 32 || *str == '$')
			return (-1);
		else if (*str == 34)
		{
			str = ft_strchrq(str,'$');
			continue;
		}
		c++;
		str = ft_strchrq(str,'$');
	}
	return (c);
}

char *ft_expandall(char *str)
{
	int nb;
	char *result;

	nb = get_nbdollar(str);
	result = str;
	if (nb == -1)
		return(NULL);
	while (nb--)
		result = ft_expand(result);
	return (result);
}

int ft_check_syntax(char *str)
{
	unsigned int i;

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
		if(str[i] == 39)
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

static char **add_env(char **envp)
{
	unsigned int i;
	char	**ret;

	i = 0;
	while(envp[i])
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

char *ft_getenv(char **envp,char *var)
{
	int	i;
	char *ret;

	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], var, ft_strlen(var)))
		{
			ret = ft_strchr(envp[i],'=');
			if (!ret)
				return(ft_strdup("", 0));
			else
				return (ret + 1);
		}
		i++;
	}
	return (ft_strdup("", 0));
}

void ft_free_env(char ***env_p)
{
	int i; 

	i = 0;
	while (*env_p[i])
		free(*env_p[i++]);
	free(*env_p);
}

void ft_export(char *parm)
{
	unsigned int i;
	char	**ret;

	ret = NULL;
	g_cmd.env_len++;
	ret = (char **) malloc(sizeof(char *) * (g_cmd.env_len + 2));
	i = 0;
	while (g_cmd.env_p[i])
	{
		ret[i] = ft_strdup(g_cmd.env_p[i], 0);
		i++;
	}
	ret[i] = ft_strdup(parm, 0);
	ret[++i] = NULL;
	free(g_cmd.env_p);
	g_cmd.env_p = ret;
}

void ft_printenv()
{
	int i;

	i = 0;
	while (g_cmd.env_p[i])
		printf("%s\n",g_cmd.env_p[i++]);
}

void ft_exit()
{
	free(g_cmd.env_p);
	leakcheck();
	exit(0);
}

t_token *init_token(int type, char *value)
{
	t_token *token;

	token = MALLOC(sizeof(t_token));
	if (!token)
		return (NULL);
	token->e_type = type;
	token->value = value;
	return (token);
}

t_list *ft_inittokens(char *str)
{
	t_lexer *lexer;
	t_token *token;
	t_list *head;

	token = (t_token *) MALLOC(sizeof(t_token));
	str = ft_expandall(str);
	lexer = init_lexer(str);
	token = lexer_next_token(lexer);
	if (token)
		head = ft_lstnew(token);
	while (token)
	{
		// printf("TOKEN(%d,%s)\n",token->e_type,token->value);
		token = lexer_next_token(lexer);
		if (token)
			ft_lstadd_back(&head,ft_lstnew(token));
	}
	return (head);
}

t_files *init_file(int type,char *value)
{
	t_files *file;

	file = malloc(sizeof(t_files));
	file->e_ftype = type;
	file->value = value;
	return (file);
}

t_command *init_command(void *files, void *args)
{
	t_command *command;
	
	command = malloc(sizeof(t_command));
	command->file = files;
	command->args = args;
	return (command);
}

t_list *init_commands(t_list *tokens)
{
	t_list *command;
	t_list *args;
	t_list *files;
	int e_type;
	int i;
	int j;
	
	i = 0;
	j = 0;
	args = NULL;
	files = NULL;
	
	while (tokens)
	{
		if (((t_token *)(tokens->content))->e_type == 5)
		{
			if (!i)
			{
				args = ft_lstnew(((t_token *)(tokens->content))->value);
				i = 1;
			}
			else if (args)
				ft_lstadd_back(&args,ft_lstnew(((t_token *)(tokens->content))->value));
		}
		else if (((t_token *)(tokens->content))->e_type >= 1  && ((t_token *)(tokens->content))->e_type <= 4)
		{
			if (!j)
			{
				e_type = ((t_token *)(tokens->content))->e_type;
				tokens = tokens->next;
				files = ft_lstnew(init_file(e_type,((t_token *)(tokens->content))->value));
				j = 1;
			}
			else if (files)
			{
				e_type = ((t_token *)(tokens->content))->e_type;
				tokens = tokens->next;
				ft_lstadd_back(&files,ft_lstnew(init_file(e_type,((t_token *)(tokens->content))->value)));	
			}
		}
		else if (((t_token *)(tokens->content))->e_type == 0)
			break;
		tokens = tokens->next;
	}
	g_cmd.tokens = tokens;
	command = ft_lstnew(init_command(files,args));
	return (command);
}

int main(int argc, char const *argv[],char **envp)
{
	(void)  argc;
	(void)  argv;
	(void) envp;
	char *str;
	t_list *tokens;
	t_list *commands; 
	t_list *args;
	t_list *files;
	// t_files *file;
	
	g_cmd.env_p = add_env(envp);
	while (1)
	{
		signal(2,ft_exit);
		str = readline("minishell : ");
		// ft_export("F=test");
		if (str && !ft_check_syntax(str))
		{
			tokens = ft_inittokens(str);
			g_cmd.tokens = tokens;
			g_cmd.commands = init_commands(g_cmd.tokens);
			commands = g_cmd.commands;
			while (commands)
			{
				args = ((t_command *)commands->content)->args;
				while (args)
				{
					printf("Words = %s\n",(char *)args->content);
					args = args->next;
				}
				files = ((t_command *)commands->content)->file;
				while (files)
				{
					printf("type = %d\n",((t_files *)files->content)->e_ftype);
					printf("filename = %s\n",((t_files *)files->content)->value);
					files = files->next;
				}
				commands = commands->next;
			}
			
		}
		else
			printf("Error\n");
		xflush();
	}
	return (0);
}