/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 17:00:00 by hko-ko           #+#    #+#             */
/*   Updated: 2026/03/14 17:00:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	handle_infiles(t_command *cmd)
{
	t_infile	*in;
	int			fd;
	in = cmd->infiles;
	while (in)
	{
		if (in->is_heredoc)
		{
		}

		else
		{
			fd = open(in->filename, O_RDONLY);
			if (fd == -1)
			{
				perror(in->filename);
				return (1);
			}

			if (dup2(fd, STDIN_FILENO) == -1)
				return (1);
			close(fd);
		}

		in = in->next;
	}

	return (0);
}

static int	handle_outfiles(t_command *cmd)
{
	t_outfile	*out;
	int			fd;
	out = cmd->outfiles;
	while (out)
	{
		if (out->is_append)
			fd = open(out->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(out->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror(out->filename);
			return (1);
		}

		if (dup2(fd, STDOUT_FILENO) == -1)
			return (1);
		close(fd);
		out = out->next;
	}

	return (0);
}

int	handle_redirections(t_command *cmd)
{
	if (handle_infiles(cmd))
		return (1);
	if (handle_outfiles(cmd))
		return (1);
	return (0);
}

