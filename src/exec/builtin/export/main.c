/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelage <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 18:52:08 by tdelage           #+#    #+#             */
/*   Updated: 2024/03/15 12:07:09 by tdelage          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../minishell.h"

int	bisalpha(int c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int	bisalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}

int	check_export(char *args)
{
	int	j;
	int	ret;

	ret = 0;
	if (!bisalpha(args[0]) && args[0] != '_')
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(args, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	j = 0;
	while (bisalnum(args[++j]) || args[j] == '_')
		;
	if (args[j] != '=' && !(args[j] == '+' && args[j + 1] == '=')
		&& args[j] != 0)
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(args, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		ret = 1;
	}
	return (ret);
}

int	b_export(int argc, char **argv, char **envp)
{
	int	ret;
	int	i;

	(void)envp;
	i = 0;
	ret = 0;
	if (argc == 1)
		print_export(envp);
	else
	{
		while (++i < argc)
		{
			if (ret == 0)
				ret = check_export(argv[i]);
			else
				check_export(argv[i]);
		}
	}
	return (ret);
}
