/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/15 12:00:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_redir_err(t_redir *redir)
{
	if (!redir->filename || !redir->filename[0]
		|| redir->filename[0] == '\1')
	{
		ft_putstr_fd("minishell: ", 2);
		if (redir->filename && redir->filename[0] == '\1')
			ft_putstr_fd("", 2);
		ft_putendl_fd("ambiguous redirect", 2);
		return (1);
	}
	return (0);
}

static int	dispatch_redir(t_redir *redir, t_exec_params *params)
{
	if (redir->type == REDIR_IN)
		return (handle_in(redir));
	else if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		return (handle_out(redir));
	else if (redir->type == REDIR_HEREDOC)
		return (handle_heredoc(redir, params));
	return (0);
}

int	handle_redirections(t_command *cmd, t_exec_params *params)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (check_redir_err(redir))
			return (1);
		if (dispatch_redir(redir, params))
			return (1);
		redir = redir->next;
	}
	return (0);
}
