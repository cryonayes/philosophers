/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeser <aeser@42kocaeli.com.tr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 21:06:35 by aeser             #+#    #+#             */
/*   Updated: 2022/06/08 22:35:04 by aeser            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*checker_function(void *arg)
{
	int		index;
	t_env	*env;

	env = (t_env *)arg;
	index = -1;
	while (++index < env->n_philo)
		waitpid(-1, NULL, 0);
	sem_post(env->sem_done);
	return (NULL);
}

void	*life_cycle_checker(void *arg)
{
	uint64_t	timestamp;
	t_philo		*philo;
	t_env		*env;

	philo = arg;
	env = philo->env;
	while (true)
	{
		if (philo->eat_count == env->must_eat)
			break ;
		timestamp = get_time_ms();
		if ((int)(timestamp - philo->last_eat) > env->tt_die)
		{
			print_state(philo, DEAD, get_time_ms());
			sem_post(env->sem_done);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}

void	start_lifecycle(t_philo *philo)
{
	pthread_t	checker_thread;

	if (philo->id % 2 == 0)
	{
		philo_think(philo);
		usleep(1000);
	}
	philo->last_eat = get_time_ms();
	pthread_create(&checker_thread, NULL, &life_cycle_checker, philo);
	pthread_detach(checker_thread);
	while (true)
	{
		take_fork(philo);
		philo_eat(philo, get_time_ms());
		leave_fork(philo);
		philo_think(philo);
		if (philo->eat_count == philo->env->must_eat)
			exit(EXIT_SUCCESS);
		philo_sleep(philo);
	}
	exit(EXIT_SUCCESS);
}
