/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antigrav <antigrav@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 16:35:00 by antigrav       #+#    #+#             */
/*   Updated: 2026/03/14 16:35:00 by antigrav      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_command	*new_command(void)
{
	t_command	*cmd;
	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(t_command));
	cmd->fd_in = -1;
	cmd->fd_out = -1;
	return (cmd);
}

void	add_argument(t_command *cmd, char *arg)
{
	char	**new_args;
	int		len;
	int		i;
	len = 0;
	if (cmd->args)
		while (cmd->args[len])
			len++;
	new_args = (char **)malloc(sizeof(char *) * (len + 2));
	if (!new_args)
		return ;
	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			new_args[i] = cmd->args[i];
			i++;
		}

		free(cmd->args);
	}

	new_args[i] = ft_strdup(arg);
	new_args[i + 1] = NULL;
	cmd->args = (char **)new_args;
}

