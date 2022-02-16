/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeatris <fbeatris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:39:40 by misha             #+#    #+#             */
/*   Updated: 2022/02/09 22:27:57 by fbeatris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ph_eat(t_phil *phil)
{
	pthread_mutex_lock(phil->left_fork);
	print_status(phil, "has taken a fork");
	if (phil->args->philo_qty == 1)
	{
		pthread_mutex_unlock(phil->left_fork);
		return (1);
	}
	pthread_mutex_lock(phil->right_fork);
	print_status(phil, "has taken a fork");
	pthread_mutex_lock(&phil->death);
	phil->last_eating = get_time(phil->args);
	print_status(phil, "is eating");
	usleep(phil->args->time_eat * 1000);
	phil->eat_count++;
	if (phil->eat_count == phil->args->cycles)
		phil->args->eat_total++;
	return (0);
}

void	*ph_live(void *argument)
{
	t_phil	*phil;

	phil = (t_phil *)argument;
	while ((phil->eat_count < phil->args->cycles || phil->args->cycles == 0) \
		&& phil->args->finish == 0)
	{
		if (ph_eat(phil) != 0)
			break ;
		pthread_mutex_unlock(&phil->death);
		pthread_mutex_unlock(phil->right_fork);
		pthread_mutex_unlock(phil->left_fork);
		print_status(phil, "is sleeping");
		usleep(phil->args->time_sleep * 1000);
		print_status(phil, "is thinking");
	}
	return (NULL);
}

/*	return 0 if ok, 1 if error */
int	init_threads(t_args *args)
{
	int	i;

	i = 0;
	if (pthread_create(&args->death_thread, NULL, check_death, args))
		return (error_clean("Error while creating death thread", args));
	args->timer_start = get_start_time();
	while (i < args->philo_qty)
	{
		if (pthread_create(&args->phils[i].thread, \
			NULL, ph_live, &args->phils[i]) != 0)
			return (error_clean("Error while creating philo thread", args));
		i += 2;
	}
	i = 1;
	usleep(args->time_eat * 1000);
	while (i < args->philo_qty)
	{
		if (pthread_create(&args->phils[i].thread, \
			NULL, ph_live, &args->phils[i]) != 0)
			return (error_clean("Error while creating philo thread", args));
		i += 2;
	}
	return (0);
}

/*	return 0 if ok, 1 if error */
int	join_threads(t_args *args)
{
	int	i;

	i = 0;
	if (args->join == 0)
	{
		args->join = 1;
		while (i < args->philo_qty)
		{
			if (pthread_join(args->phils[i].thread, NULL) != 0)
				return (error_clean("Error while joining threads", args));
			i++;
		}
		if (pthread_join(args->death_thread, NULL) != 0)
			return (error_clean("Error while joining threads", args));
	}	
	return (0);
}

int	main(int argc, char **argv)
{
	t_args	args;

	if (argc != 5 && argc != 6)
		return (error_return("Wrong number of arguments"));
	else
	{
		if (save_args(&args, argc, argv))
			return (1);
		if (init_forks(&args))
			return (1);
		if (init_philosophers(&args))
			return (1);
		if (init_threads(&args))
			return (1);
		if (join_threads(&args))
			return (1);
		if (destroy_mutexes(&args))
			return (1);
		free(args.phils);
		free(args.forks);
	}
}
