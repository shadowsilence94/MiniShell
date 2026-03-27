/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/15 12:00:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_in(t_redir *redir)
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

int	handle_out(t_redir *redir)
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

static void	read_heredoc(int fd, t_redir *redir, t_exec_params *params)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, redir->filename,
				ft_strlen(redir->filename) + 1) == 0)
		{
			free(line);
			break ;
		}
		if (!redir->quoted)
		{
			expanded = expand_heredoc_line(line, params);
			write(fd, expanded, ft_strlen(expanded));
			free(expanded);
		}
		else
			write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

int	handle_heredoc(t_redir *redir, t_exec_params *params)
{
	int	fd[2];

	if (pipe(fd) == -1)
		return (1);
	read_heredoc(fd[1], redir, params);
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		return (close(fd[0]), 1);
	close(fd[0]);
	return (0);
}
