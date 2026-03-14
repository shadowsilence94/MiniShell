/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antigrav <antigrav@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 16:35:00 by antigrav       #+#    #+#             */
/*   Updated: 2026/03/14 16:35:00 by antigrav      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;
	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

void	append_token(t_token **head, t_token *new_t)
{
	t_token	*tmp;
	if (!*head)
	{
		*head = new_t;
		return ;
	}

	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_t;
}

