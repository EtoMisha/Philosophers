/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeatris <fbeatris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/13 20:13:38 by fbeatris          #+#    #+#             */
/*   Updated: 2021/11/15 22:08:25 by fbeatris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	finish_norm(t_args *args)
{
	args->finish = 1;
	usleep(500);
	printf("Everyone has eaten\n");
}

static void	finish_death(t_phil *phil)
{
	pthread_mutex_lock(&phil->death);
	phil->args->cycles = 1;
	phil->args->exit = 1;
	printf("%ld %d died\n", get_time(phil->args), phil->number);
	pthread_mutex_unlock(&phil->death);
}

static void	update_i(int *i, int qty)
{
	if (*i == qty - 1)
		*i = 0;
	else
		(*i)++;
}

void	*check_death(void *argument)
{
	t_args	*args;
	int		i;
	int		eaten_count;

	i = 0;
	eaten_count = 0;
	args = (t_args *)argument;
	while (i < args->philo_qty)
	{
		if (args->eat_total == args->philo_qty)
		{
			finish_norm(args);
			break ;
		}
		else if (get_time(args) - 8 > \
			args->phils[i].last_eating + args->time_die)
		{
			finish_death(&args->phils[i]);
			break ;
		}
		update_i(&i, args->philo_qty);
	}
	return (NULL);
}

/*	return 0 if ok, 1 if error */
int	destroy_mutexes(t_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_qty)
	{
		if (pthread_mutex_destroy(&args->forks[i]) != 0)
			return (error_clean("Error while destroy fork mutex", args));
		i++;
	}
	i = 0;
	while (i < args->philo_qty)
	{
		if (pthread_mutex_destroy(&args->phils[i].death) != 0)
			return (error_clean("Error while destroy death mutex", args));
		i++;
	}
	return (0);
}
