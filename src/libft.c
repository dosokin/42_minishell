/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dosokin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 11:06:25 by dosokin           #+#    #+#             */
/*   Updated: 2024/03/20 01:25:39 by dosokin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	should_add_quotes(const char *s, char *cut, int *l)
{
	int	i;

	i = 0;
	while (s[i] && cut[i] && s[i] == cut[i])
		i++;
	if (s[i] == '=')
		i++;
	while (s[i])
	{
		if (is_quote_o_pipe(s[i]))
			*l = *l + 1;
		++i;
	}
}

void	ft_strdup_env_child(const char *s, t_dup_data *data, int *j, int *i)
{
	if (is_quote_o_pipe(s[*i]))
	{
		data->result[*j] = '\\';
		*j = *j + 1;
		data->result[*j] = s[*i];
		increment_both(j, i);
	}
	else
		dup_and_get_next((char **)&s, i, &data->result, j);
}

char	*ft_strdup_env(const char *s, char *cut, int i, int j)
{
	t_dup_data	data;

	data.multiple_word = false;
	data.reset = false;
	data.l = ft_strlen((char *)s) - (ft_strlen(cut) + 1);
	should_add_quotes(s, cut, &data.l);
	data.result = malloc((data.l + 1) * sizeof(char));
	if (!data.result)
		return (NULL);
	while (s[i] && cut[i] && s[i] == cut[i])
		i++;
	if (s[i] == '=')
		i++;
	while (s[i])
		ft_strdup_env_child(s, &data, &j, &i);
	data.result[j] = '\0';
	return (data.result);
}
