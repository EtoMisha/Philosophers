/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeatris <fbeatris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:39:46 by misha             #+#    #+#             */
/*   Updated: 2021/11/22 22:37:39 by fbeatris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <semaphore.h>
# include <signal.h>

typedef struct s_phil
{
	int				number;
	pthread_t		death_thread;
	pid_t			pid;
	struct s_args	*args;
	long			last_eating;
	int				eat_count;
}	t_phil;

typedef struct s_args
{
	t_phil			*phils;
	sem_t			*forks;
	sem_t			*death;
	sem_t			*eat;
	sem_t			*print;
	pthread_t		eat_thread;
	int				philo_qty;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				cycles;
	long			timer_start;
}	t_args;

int		save_args(t_args *args, int argc, char **argv);
int		init_forks(t_args *args);
int		init_philosophers(t_args *args);
int		init_sem(t_args *args);
int		destroy_mutexes(t_args *args);
void	*check_death(void *argument);

int		ft_atoi(const char *str);
void	print_status(t_phil *phil, char *msg);
int		error_return(char *msg);
int		error_clean(char *msg, t_args *args);
long	get_start_time(void);
long	get_time(t_args *args);

#endif