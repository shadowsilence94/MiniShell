/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/15 12:00:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#ifdef __APPLE__

void	rl_replace_line(const char *text, int clear_undo)
{
	(void)text;
	(void)clear_undo;
}

void	rl_clear_history(void)
{
	clear_history();
}

#endif
