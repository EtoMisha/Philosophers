/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeatris <fbeatris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 20:45:57 by misha             #+#    #+#             */
/*   Updated: 2021/11/17 21:42:49 by fbeatris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	save_zero_args(t_args *args)
{
	args->eat_total = 0;
	args->join = 0;
	args->exit = 0;
	args->finish = 0;
	args->timer_start = 0;
}

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
	save_zero_args(args);
	return (0);
}

/*	return 0 if ok, 1 if error */
int	init_forks(t_args *args)
{
	int	i;

	i = 0;
	args->forks = malloc(sizeof(*(args->forks)) * args->philo_qty);
	if (!args->forks)
		return (error_return("Malloc error"));
	while (i < args->philo_qty)
	{
		if (pthread_mutex_init(&args->forks[i], NULL) != 0)
		{
			free(args->forks);
			return (error_return("Mutex init error"));
		}
		i++;
	}
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
		args->phils[i].left_fork = &args->forks[i];
		if (i < args->philo_qty - 1)
			args->phils[i].right_fork = &args->forks[i + 1];
		else
			args->phils[i].right_fork = &args->forks[0];
		args->phils[i].args = args;
		args->phils[i].last_eating = args->timer_start;
		args->phils[i].eat_count = 0;
		args->phils[i].finish_eat = 0;
		if (pthread_mutex_init(&args->phils[i].death, NULL) != 0)
			return (error_clean("Mutex init error", args));
		i++;
	}
	return (0);
}
