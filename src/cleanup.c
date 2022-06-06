/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeser <aeser@42kocaeli.com.tr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 21:01:39 by aeser             #+#    #+#             */
/*   Updated: 2022/06/06 21:02:14 by aeser            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_threads(t_env *env)
{
	int	index;

	index = -1;
	while (++index < env->n_philo)
		pthread_detach(env->philos[index].thread_id);
}

void	destroy_mutexes(t_env *env)
{
	int	index;

	index = -1;
	while (++index < env->n_philo)
		pthread_mutex_destroy(&env->forks[index]);
	pthread_mutex_destroy(&env->write);
}
