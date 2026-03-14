/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antigravity <antigravity@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by antigravity       #+#    #+#             */
/*   Updated: 2025/12/26 12:00:00 by antigravity      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Allocate new command node
 */
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

/*
 * Add argument to command
 */
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
	cmd->args = new_args;
}

/*
 * Add redirection to command
 */
void	add_redirection(t_command *cmd, t_token *token, t_token *file_token)
{
	t_infile	*in;
	t_outfile	*out;
	t_infile	*last_in;
	t_outfile	*last_out;

	if (token->type == TOKEN_REDIRECT_IN || token->type == TOKEN_HEREDOC)
	{
		in = (t_infile *)malloc(sizeof(t_infile));
		in->filename = ft_strdup(file_token->value);
		in->is_heredoc = (token->type == TOKEN_HEREDOC);
		in->next = NULL;
		if (!cmd->infiles)
			cmd->infiles = in;
		else
		{
			last_in = cmd->infiles;
			while (last_in->next)
				last_in = last_in->next;
			last_in->next = in;
		}
	}
	else
	{
		out = (t_outfile *)malloc(sizeof(t_outfile));
		out->filename = ft_strdup(file_token->value);
		out->is_append = (token->type == TOKEN_APPEND);
		out->next = NULL;
		if (!cmd->outfiles)
			cmd->outfiles = out;
		else
		{
			last_out = cmd->outfiles;
			while (last_out->next)
				last_out = last_out->next;
			last_out->next = out;
		}
	}
}

/*
 * Main parser logic
 */
t_command	*parse_input(char *line, char **envp, int *last_status)
{
	t_token		*tokens;
	t_token		*tmp;
	t_command	*head;
	t_command	*curr;

	(void)envp;
	tokens = tokenize(line, *last_status);
	if (!tokens)
		return (NULL);
	head = new_command();
	curr = head;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_PIPE)
		{
			curr->next = new_command();
			curr->next->prev = curr;
			curr = curr->next;
		}
		else if (tmp->type == TOKEN_WORD)
		{
			add_argument(curr, tmp->value);
		}
		else if (tmp->type >= TOKEN_REDIRECT_IN && tmp->type <= TOKEN_APPEND)
		{
			if (tmp->next && tmp->next->type == TOKEN_WORD)
			{
				add_redirection(curr, tmp, tmp->next);
				tmp = tmp->next;
			}
			else
			{
				printf("minishell: syntax error near unexpected token\n");
				// TODO: Handle error properly
				return (NULL);
			}
		}
		tmp = tmp->next;
	}
	return (head);
}
