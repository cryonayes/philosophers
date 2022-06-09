/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeser <aeser@42kocaeli.com.tr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 21:01:39 by aeser             #+#    #+#             */
/*   Updated: 2022/06/08 22:37:11 by aeser            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_semaphores(void)
{
	sem_unlink(SEM_FORK);
	sem_unlink(SEM_WRITE);
	sem_unlink(SEM_DIED);
}

static void	destroy_zombies(t_env *env)
{
	int	index;

	index = -1;
	while (++index < env->n_philo)
		kill(env->pids[index], SIGKILL);
}

void	destroy(t_env *env)
{
	destroy_zombies(env);
	destroy_semaphores();
	free(env->pids);
	free(env->philos);
}
