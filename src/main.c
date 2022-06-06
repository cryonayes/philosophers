/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeser <aeser@42kocaeli.com.tr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 20:53:54 by aeser             #+#    #+#             */
/*   Updated: 2022/06/06 19:46:43 by aeser            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_arguments(int argc, char **argv, t_env *env)
{
	env->n_philo = atoi(argv[1]);
	env->tt_die = atoi(argv[2]);
	env->tt_eat = atoi(argv[3]);
	env->tt_sleep = atoi(argv[4]);
	env->must_eat = -1;
	env->is_running = true;
	if (argc == 6)
		env->must_eat = atoi(argv[5]);
}

void	init_mutexes(t_env *env)
{
	int	index;

	env->philos = malloc(sizeof(t_philo) * env->n_philo);
	env->forks = malloc(sizeof(pthread_mutex_t) * env->n_philo);
	index = -1;
	while (++index < env->n_philo)
		pthread_mutex_init(&env->forks[index], NULL);
	pthread_mutex_init(&env->write, NULL);
	pthread_mutex_init(&env->read, NULL);
}

void	init_philos(t_env *env)
{
	int	index;

	index = -1;
	while (++index < env->n_philo)
	{
		env->philos[index].id = index + 1;
		env->philos[index].l_fork = index;
		env->philos[index].r_fork = (index + 1) % env->n_philo;
		env->philos[index].env = env;
		env->philos[index].thread_id = 0;
		env->philos[index].last_eat = 0;
		env->philos[index].eat_count = 0;
		env->philos[index].done = false;
		pthread_mutex_init(&env->philos[index].read, NULL);
	}
}

void	init_threads(t_env *env)
{
	int			index;
	pthread_t	life_cycle_id;

	index = -1;
	while (++index < env->n_philo)
		pthread_create(&env->philos[index].thread_id, NULL,
			&life_cycle, &env->philos[index]);
	pthread_create(&life_cycle_id, NULL, &life_cycle_checker, env);
	pthread_join(life_cycle_id, NULL);
}

int	main(int argc, char **argv)
{
	t_env	env;

	init_arguments(argc, argv, &env);
	init_mutexes(&env);
	init_philos(&env);
	init_threads(&env);
}
