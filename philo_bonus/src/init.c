/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeser <aeser@42kocaeli.com.tr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 21:00:28 by aeser             #+#    #+#             */
/*   Updated: 2022/06/08 19:21:35 by aeser            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	check_arguments(t_env *env)
{
	if (env->tt_die < 60 || env->tt_eat < 60 || env->tt_sleep < 60)
	{
		printf("Error: Arguments cannot be smaller than 60ms\n");
		exit(FAILURE);
	}
	if (env->n_philo > 200)
	{
		printf("Error: Philosopher count cannot be higher than 200\n");
		exit(FAILURE);
	}
}

void	init_arguments(int argc, char **argv, t_env *env)
{
	if (argc < 5)
	{
		printf("Usage: ./philo tt_die tt_eat tt_sleep <eat_count>\n");
		exit(FAILURE);
	}
	env->n_philo = atoi(argv[1]);
	env->tt_die = atoi(argv[2]);
	env->tt_eat = atoi(argv[3]);
	env->tt_sleep = atoi(argv[4]);
	env->must_eat = -1;
	if (argc == 6)
		env->must_eat = atoi(argv[5]);
	check_arguments(env);
}

void	init_semaphore(t_env *env)
{
	destroy_semaphores();
	env->sem_forks = sem_open(SEM_FORK, O_CREAT, 0644, env->n_philo);
	env->sem_write = sem_open(SEM_WRITE, O_CREAT, 0644, 1);
	env->sem_done = sem_open(SEM_DIED, O_CREAT, 0644, 0);
	if (env->sem_forks == SEM_FAILED || env->sem_write == SEM_FAILED)
		exit(1);
	if (env->sem_done == SEM_FAILED)
		exit(1);
}

void	init_philos(t_env *env)
{
	int	index;

	env->philos = malloc(sizeof(t_philo) * env->n_philo);
	index = -1;
	while (++index < env->n_philo)
	{
		env->philos[index].id = index + 1;
		env->philos[index].eat_count = 0;
		env->philos[index].last_eat = 0;
		env->philos[index].env = env;
	}
}

void	init_philo_process(t_env *env)
{
	int			index;

	env->pids = malloc(sizeof(int) * env->n_philo);
	index = -1;
	while (++index < env->n_philo)
	{
		env->pids[index] = fork();
		if (env->pids[index] == 0)
		{
			start_lifecycle(&env->philos[index]);
			return ;
		}
	}
}
