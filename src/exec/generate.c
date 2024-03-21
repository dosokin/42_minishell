/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelage <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 21:23:01 by tdelage           #+#    #+#             */
/*   Updated: 2024/03/08 10:36:49 by tdelage          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**dup_exec_envp(char **envp)
{
	char	**new;
	int		i;
	int		j;

	i = -1;
	j = 0;
	while (envp[++i])
		if (ft_strchr(envp[i], '='))
			j++;
	new = malloc((j + 1) * sizeof(char *));
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '='))
		{
			new[j] = ft_strdup(envp[i]);
			j++;
		}
		i++;
	}
	new[j] = NULL;
	return (new);
}

struct s_cmd	extract_base_cmd(t_main *data, int id)
{
	struct s_cmd	command;
	t_com			*commandr;

	commandr = data->commands_data[id];
	if (commandr->has_program && !is_builtin(commandr->program)
		&& ft_strlen(commandr->program) > 0)
		command.exec = ft_find_path(data->commands_data[id]->program,
				data->paths);
	else if (commandr->has_program)
		command.exec = ft_strdup(data->commands_data[id]->program);
	else
		command.exec = NULL;
	command.args = dup_char_dt(data->commands_data[id]->arguments);
	command.env = dup_exec_envp(data->envp);
	return (command);
}

struct s_cmd	*generate_cmd(t_main *data, int (*pipes)[2], int id, int last)
{
	struct s_cmd	*command;
	t_com			*self;

	command = malloc(sizeof(struct s_cmd));
	if (!command)
		return (NULL);
	*command = extract_base_cmd(data, id);
	self = data->commands_data[id];
	command->infd = resolve_entry(self, pipes, (struct s_mainloop){id, last},
			data->envp);
	command->outfd = resolve_out(self, pipes, id);
	return (command);
}

t_bool	ft_create_pipes(int (*pipes)[2], int count)
{
	int	i;

	i = -1;
	while (++i < count)
	{
		if (pipe(pipes[i]) < 0)
		{
			while (--i >= 0)
			{
				m_close(pipes[i][0]);
				m_close(pipes[i][1]);
			}
			return (FALSE);
		}
	}
	return (TRUE);
}

void	generate_fork_data(struct s_cmds_piped *self, t_main *data, int last)
{
	int	i;

	i = -1;
	self->valid = TRUE;
	self->count = data->command_c;
	self->cmds = NULL;
	self->pipes = malloc((self->count - 1) * sizeof(int [2]));
	if (self->pipes)
	{
		self->valid = ft_create_pipes(self->pipes, self->count - 1);
		self->cmds = malloc((data->command_c + 1) * sizeof(struct s_cmd *));
		if (self->valid && self->cmds)
		{
			while (++i < self->count)
			{
				self->cmds[i] = generate_cmd(data, self->pipes, i, last);
				if (!self->cmds[i])
					self->valid = FALSE;
			}
		}
		else
			self->valid = FALSE;
	}
	else
		self->valid = FALSE;
}
