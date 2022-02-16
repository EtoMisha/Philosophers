/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeatris <fbeatris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 20:45:57 by misha             #+#    #+#             */
/*   Updated: 2021/11/22 22:39:50 by fbeatris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <errno.h>

/*	return 0 if ok, 1 if error */
int	save_args(t_args *args, int argc, char **argv)
{
	args->philo_qty = ft_atoi(argv[1]);
	if (args->philo_qty < 1)
		return (error_return("Wrong 1 argument"));
	args->time_die = ft_atoi(argv[2]);
	if (args->time_die <= 0)
		return (error_return("Wrong 2 argument"));
	args->time_eat = ft_atoi(argv[3]);
	if (args->time_eat <= 0)
		return (error_return("Wrong 3 argument"));
	args->time_sleep = ft_atoi(argv[4]);
	if (args->time_sleep <= 0)
		return (error_return("Wrong 4 argument"));
	if (argc == 6)
	{
		args->cycles = ft_atoi(argv[5]);
		if (args->cycles == 0)
			return (error_return("Wrong 5 argument"));
	}
	else
		args->cycles = 0;
	args->timer_start = 0;
	return (0);
}

/*	return 0 if ok, 1 if error */
int	init_forks(t_args *args)
{
	sem_unlink("forks_sem");
	args->forks = sem_open("forks_sem", O_CREAT, S_IRWXU, args->philo_qty);
	if (args->forks == SEM_FAILED)
		return (error_clean("Sem open error", args));
	return (0);
}

/*	return 0 if ok, 1 if error */
int	init_sem(t_args *args)
{
	sem_unlink("death_sem");
	args->death = sem_open("death_sem", O_CREAT, S_IRWXU, 1);
	if (args->death == SEM_FAILED)
		return (error_clean("Sem open error", args));
	sem_unlink("print_sem");
	args->print = sem_open("print_sem", O_CREAT, S_IRWXU, 1);
	if (args->print == SEM_FAILED)
		return (error_clean("Sem open error", args));
	sem_unlink("eat_sem");
	args->eat = sem_open("eat_sem", O_CREAT, S_IRWXU, 0);
	if (args->eat == SEM_FAILED)
		return (error_clean("Sem open error", args));
	return (0);
}

/*	return 0 if ok, 1 if error */
int	init_philosophers(t_args *args)
{
	int	i;

	i = 0;
	args->phils = malloc(sizeof(t_phil) * args->philo_qty);
	if (!args->phils)
		return (error_clean("Malloc error", args));
	while (i < args->philo_qty)
	{
		args->phils[i].number = i + 1;
		args->phils[i].args = args;
		args->phils[i].last_eating = args->timer_start;
		args->phils[i].eat_count = 0;
		i++;
	}
	return (0);
}
