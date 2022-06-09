/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeser <aeser@42kocaeli.com.tr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 15:41:11 by aeser             #+#    #+#             */
/*   Updated: 2022/06/07 01:18:07 by aeser            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_fork(t_philo *philo)
{
	sem_wait(philo->env->sem_forks);
	print_state(philo, TOOK_FORK, get_time_ms());
	sem_wait(philo->env->sem_forks);
	print_state(philo, TOOK_FORK, get_time_ms());
}

void	leave_fork(t_philo *philo)
{
	sem_post(philo->env->sem_forks);
	sem_post(philo->env->sem_forks);
}

void	philo_eat(t_philo *philo, uint64_t timestamp)
{
	print_state(philo, EATING, timestamp);
	philo->eat_count++;
	philo->last_eat = timestamp;
	usleep(philo->env->tt_eat * 1000);
}

void	philo_sleep(t_philo *philo)
{
	print_state(philo, SLEEPING, get_time_ms());
	usleep(philo->env->tt_sleep * 1000);
}

void	philo_think(t_philo *philo)
{
	print_state(philo, THINKING, get_time_ms());
}
