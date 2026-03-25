/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/14 21:10:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_in(t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->filename);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	handle_out(t_redir *redir)
{
	int	fd;
	int	flags;

	if (redir->type == REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(redir->filename, flags, 0644);
	if (fd == -1)
	{
		perror(redir->filename);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static void	read_heredoc(int fd, char *delim)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delim,
				ft_strlen(delim) + 1) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

static int	handle_heredoc(t_redir *redir)
{
	int	fd[2];

	if (pipe(fd) == -1)
		return (1);
	read_heredoc(fd[1], redir->filename);
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		return (close(fd[0]), 1);
	close(fd[0]);
	return (0);
}

int	handle_redirections(t_command *cmd)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (!redir->filename || !redir->filename[0])
			return (ft_putendl_fd("minishell: ambiguous redirect", 2), 1);
		if (redir->type == REDIR_IN && handle_in(redir))
			return (1);
		else if ((redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
			&& handle_out(redir))
			return (1);
		else if (redir->type == REDIR_HEREDOC && handle_heredoc(redir))
			return (1);
		redir = redir->next;
	}
	return (0);
}
