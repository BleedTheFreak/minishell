/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-ghem <ael-ghem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 02:50:27 by ael-ghem          #+#    #+#             */
/*   Updated: 2022/03/01 02:33:04 by ael-ghem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*getPath (char **cmd, char **envp)
{
	char	*path;
	char	*dir;
	char	*bin;
	int		i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	path = envp[i] + 5;
	while (path && ft_strnchr(path, ':') > -1)
	{
		dir = ft_strndup(path, ft_strnchr(path, ':'));
		bin = join_path(dir, cmd[0]);
		free(dir);
		if (access(bin, F_OK) == 0)
			return (bin);
		free(bin);
		path += ft_strnchr(path, ':') + 1;
	}
	return (cmd[0]);
}

int	openfile (char *filename, int mode)
{
	if (mode == TYPE_LTEHN)
	{
		if (access(filename, F_OK))
		{
			write(STDERR, "Minishell: ", 7);
			write(STDERR, filename, ft_strnchr(filename, 0));
			write(STDERR, ": No such file or directory\n", 28);
			return (STDIN);
		}
		return (open(filename, O_RDONLY));
	}
	else if (mode == TYPE_GTEHN)
		return (open(filename, O_CREAT | O_WRONLY | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH));
	else if (mode == TYPE_APPEND)
		return (open(filename, O_CREAT | O_WRONLY | O_APPEND,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH));
    else
        return (here_doc("/tmp/heredoc420", filename));
}

void	exec (char **cmd, char **envp)
{
	char	*path;

	if (ft_strnchr(cmd[0], '/') > -1)
		path = cmd[0];
	else
		path = getPath(cmd, envp);
	execve(path, cmd, envp);
	write(STDERR, "minishell: ", 7);
	write(STDERR, "command not found: ", 19);
	write(STDERR, path, ft_strnchr(path, '\0'));
	write(STDERR, "\n", 1);
	exit(127);
}

void    redir (char **cmd, char **envp, int *pipes, int index,int flag, int fd, int type)
{
	pid_t	pid;

	pid = fork();
	if (pid)
	{
        if (index == 0 && flag == 0)
        {
            close(pipes[1]);
        }
        else if (index  != 0 && flag == 1)
        {
            close(pipes[(index * 2) - 2]);
        }
        else if (index == 0 && flag == 1)
            ;
        else
        {
            close(pipes[(index * 2 ) + 1]);
            close(pipes[(index * 2 ) - 2]);
        }
	}
	else if (pid == 0)
	{
        if (index == 0 && flag == 1)
        {
            if (fd >= 0 && (type == TYPE_GTEHN || type == TYPE_APPEND))
                dup2(fd, 1);
            else if (fd >= 0 && (type == TYPE_LTEHN || type == TYPE_HEREDOC))
                dup2(fd, 0);
        }
        else if (index == 0 && flag == 0)
        {
            dup2(pipes[1], 1);
        }
        else if (index  != 0 && flag == 1)
        {
            dup2(pipes[(index*2 ) -2], 0);
            close(pipes[(index*2 ) -1]);
        }
        else
        {
            dup2(pipes[(index * 2) + 1], 1);
            dup2(pipes[(index * 2 ) - 2], 0);
        }
        if (type >= 1 && type <= 4)
		    exec(cmd, envp);
	}
    else
    {
        perror("minishell");
        exit(0);
    }
}

int     here_doc(char *path, char *esc)
{
	int		infd;
	char	*buf;
	int		r;

	r = 42;
	buf = malloc(1025);
	infd = openfile(path, OUTFILE);
	while (r)
	{
		write(1, "> ", 3);
		r = read(0, buf, 1024);
		buf[r] = '\0';
		if ((ft_strncmp(buf, esc, ft_strnchr(esc, '\0')) == 0)
			&& (ft_strnchr(esc, '\0') == ft_strnchr(buf, '\0') - 1))
			break ;
		if (ft_strnchr(buf, '\n') != -1)
			write(infd, buf, ft_strnchr(buf, '\n') + 1);
	}
	close(infd);
	free(buf);
	return (openfile(path, TYPE_LTEHN));
}

int	ft_strnchr (char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	if (str[i] == c)
		return (i);
	return (-1);
}

char	*ft_strndup (char *s, unsigned int n)
{
	char				*ret;
	unsigned int		i;

	i = 0;
	ret = malloc(sizeof(char) * (n + 1));
	while (i < n)
		ret[i++] = *s++;
	ret[n] = 0;
	return (ret);
}

char	*join_path (char *path, char *bin)
{
	char	*ret;
	int		i;
	int		j;

	ret = malloc(sizeof(char)
			*(ft_strnchr(path, 0) + ft_strnchr(bin, 0) + 2));
	if (!ret)
		return (NULL);
	i = 0;
	j = 0;
	while (path[j])
		ret[i++] = path[j++];
	ret[i++] = '/';
	j = 0;
	while (bin[j])
		ret[i++] = bin[j++];
	ret[i] = 0;
	return (ret);
}

int	ft_strncmp (char *s1, char *s2, int n)
{
	while (--n > 0 && *s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s2 - *s1);
}

char	**ft_split (char *s, char sep)
{
	char	**ret;
	int		words;
	int		i;
	int		j;

	words = 0;
	j = 0;
	while (s[j])
		if (s[j++] == sep)
			words++;
	ret = malloc(sizeof(char *) * (words + 2));
	ret[words + 1] = NULL;
	i = 0;
	while (i < words + 1)
	{
		j = 0;
		while (s[j] && s[j] != sep)
			j++;
		ret[i++] = ft_strndup(s, j);
		s = s + j + 1;
	}
	return (ret);
}
