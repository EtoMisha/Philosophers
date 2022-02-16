/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeatris <fbeatris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:39:40 by misha             #+#    #+#             */
/*   Updated: 2021/11/22 22:58:37 by fbeatris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*check_death(void *argument)
{
	t_phil	*phil;

	phil = (t_phil *)argument;
	while (1)
	{
		if (get_time(phil->args) > \
			phil->last_eating + phil->args->time_die)
		{
			sem_wait(phil->args->death);
			print_status(phil, "died");
			exit(0);
		}
	}
	return (NULL);
}

/*	return 0 if ok, 1 if error */
int	phil_live(t_phil *phil)
{
	if (pthread_create(&phil->death_thread, NULL, check_death, phil) != 0)
		return (error_clean("Thread creating error", phil->args));
	while (phil->eat_count < phil->args->cycles || phil->args->cycles == 0)
	{
		sem_wait(phil->args->forks);
		print_status(phil, "has taken a fork");
		sem_wait(phil->args->forks);
		print_status(phil, "has taken a fork");
		phil->last_eating = get_time(phil->args);
		print_status(phil, "is eating");
		usleep(phil->args->time_eat * 1000);
		phil->eat_count++;
		sem_post(phil->args->forks);
		sem_post(phil->args->forks);
		if (phil->eat_count == phil->args->cycles)
			sem_post(phil->args->eat);
		print_status(phil, "is sleeping");
		usleep(phil->args->time_sleep * 1000);
		print_status(phil, "is thinking");
	}
	exit (0);
}

/*	return 0 if ok, 1 if error */
int	run_processes(t_args *args)
{
	int	i;

	i = 0;
	args->timer_start = get_start_time();
	while (i < args->philo_qty)
	{
		args->phils[i].pid = fork();
		if (args->phils[i].pid == 0)
		{
			if (phil_live(&args->phils[i]) != 0)
				return (1);
			if (pthread_join(args->phils[i].death_thread, NULL) != 0)
				return (error_clean("Thread join error", args));
		}
		i++;
	}
	waitpid(-1, NULL, 0);
	i = 0;
	while (i < args->philo_qty)
	{
		kill(args->phils[i].pid, SIGTERM);
		i++;
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
		if (init_sem(&args))
			return (1);
		if (run_processes(&args))
			return (1);
		free(args.phils);
	}
}
