/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeser <aeser@42kocaeli.com.tr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 21:00:28 by aeser             #+#    #+#             */
/*   Updated: 2022/07/03 16:08:44 by aeser            ###   ########.fr       */
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
		printf("Usage: ./philo philo_count tt_die tt_eat tt_sleep <eat_count>\n");
		exit(FAILURE);
	}
	env->n_philo = atoi(argv[1]);
	env->tt_die = atoi(argv[2]);
	env->tt_eat = atoi(argv[3]);
	env->tt_sleep = atoi(argv[4]);
	env->must_eat = -1;
	env->is_running = true;
	if (argc == 6)
		env->must_eat = atoi(argv[5]);
	check_arguments(env);
}

/*
	Her bir fork mutex'dir, mutex'ler lock ve unlock özelliklerine sahiplerdir.
	Bir mutex lock edildiği zaman başka bir thread o mutex'i locklamak ister ise
	o mutex unlock olana kadar beklemek zorundadır. Bu özellik sayesinde farklı threadler
	arası senkronizasyon işlemi sağlanabilir.
*/
void	init_mutexes(t_env *env)
{
	int	index;

	env->philos = malloc(sizeof(t_philo) * env->n_philo);
	env->forks = malloc(sizeof(pthread_mutex_t) * env->n_philo);
	index = -1;
	while (++index < env->n_philo)
		pthread_mutex_init(&env->forks[index], NULL); // Mutex'i Unlocked olarak init eder.
	// Yazım işlemlerini senkron yapmak için.
	// (Şart değil) Printf hali hazırda thread safe bir fonksiyonç.
	pthread_mutex_init(&env->write, NULL); 
}

/*
	Her bir philosopher sağında ve solunda bulunan çatallara erişebilir, bu yüzden sol çatal
	philo_id, sağ çatal ise philo_id + 1 olur. Bulundukları masa yuvarlak olduğu için ise
	sağ çatalın n_philo ile modu alınır. Bu sayede son philonun sağ çatalı, ilk philonun sol
	çatalı olmuş olur.
*/
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
		env->philos[index].done = false; // Philosopher'ın yemesi bitti mi?
	}
}

/*
	Her bir philo bir thread'dir, threadler aynı process içerisinde çalışır
	ve paylaştıkları hafıza da aynıdır. Bu yüzden process içerisinde kullanılan
	değişkenler gibi ortak verilere erişebilirler. 
*/
void	init_threads(t_env *env)
{
	int			index;
	pthread_t	life_cycle_id;

	index = -1;
	while (++index < env->n_philo)
		pthread_create(&env->philos[index].thread_id, NULL,
			&life_cycle, &env->philos[index]);
	// Tüm philosopher'ları kontrol eden tek bir thread var.
	pthread_create(&life_cycle_id, NULL, &life_cycle_checker, env);
	// life_cycle_checker thread'i bitene kadar blocking olarak bekler. 
	pthread_join(life_cycle_id, NULL);
}
