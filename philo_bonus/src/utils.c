/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeser <aeser@42kocaeli.com.tr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 21:15:52 by aeser             #+#    #+#             */
/*   Updated: 2022/06/07 01:31:04 by aeser            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t	get_time_ms(void)
{
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

void	print_state(t_philo *philo, t_state state, uint64_t timestamp)
{
	const char	*states[6] = {STR_THINKING, STR_EATING, STR_SLEEPING,
		STR_TOOK_FORK, STR_DEAD, NULL};

	sem_wait(philo->env->sem_write);
	printf("%lu %d %s\n", timestamp, philo->id, states[state]);
	sem_post(philo->env->sem_write);
}
