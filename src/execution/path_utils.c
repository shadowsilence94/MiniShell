/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/14 21:10:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

int	is_dir(char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
		return (0);
	return (S_ISDIR(st.st_mode));
}

static char	*try_paths(char **paths, char *cmd)
{
	char	*part_path;
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(full_path, X_OK) == 0 && !is_dir(full_path))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_command_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path_env;
	char	*res;

	path_env = get_env_value(envp, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	res = try_paths(paths, cmd);
	free_split(paths);
	return (res);
}

void	exec_script(char *path, char **args, char **envp)
{
	char	**new_args;
	int		i;
	int		j;

	i = 0;
	while (args[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		exit(1);
	new_args[0] = ft_strdup("/bin/sh");
	new_args[1] = ft_strdup(path);
	j = 0;
	while (args[++j])
		new_args[j + 1] = ft_strdup(args[j]);
	new_args[j + 1] = NULL;
	execve("/bin/sh", new_args, envp);
}
