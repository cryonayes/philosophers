/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeser <aeser@42kocaeli.com.tr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 21:06:35 by aeser             #+#    #+#             */
/*   Updated: 2022/07/03 15:52:01 by aeser            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	Her philosopher bir process'dir dolayısıyla waitpid kullanılark bu process'lerin
	bitip bitmediğini kontrol edebiliriz.

	Burda ise waitpid ile herhangi bir child process'in bitmesini bekliyoruz.
	Bunu n_philo kadar yaparak tüm philosopher'ların yemesi gereken kadar yemek yediğini
	kontrol ediyoruz.

	Tüm philosopher'lar yemeyi bitirdiği zaman sem_post değeri arttırılıyor ve main
	fonksiyonunda destroy fonksiyonuna geçiliyor. 
*/
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
		// Philosopher yemesi gerektiği kadar yediyse break yapılır
		if (philo->eat_count == env->must_eat)
			break ;
		// Ölümü kontrol eder, eğer herhangi bir philosopher ölürse sem_done değeri bir arttırılır,
		//ve main fonksiyonunda destroy fonksiyonuna geçilir. 
		timestamp = get_time_ms();
		if ((int)(timestamp - philo->last_eat) > env->tt_die)
		{
			print_state(philo, DEAD, get_time_ms());
			sem_post(env->sem_done);
			break ;
		}
		// CPU kullanımını dengelemek için.
		usleep(1000);
	}
	return (NULL);
}

/*
	Her bir child process (philosopher) bu fonksiyonu çalıştırır.
*/
void	start_lifecycle(t_philo *philo)
{
	pthread_t	checker_thread;

	// Philosopher'in id'si çift sayı ise ufak bir süre bekler.
	// Bunun nedeni aynı anda tüm philosopher'ların yarısına yetecek kadar çatal olmasıdır.
	if (philo->id % 2 == 0)
	{
		philo_think(philo);
		usleep(1000);
	}
	// Ölüm kontrolü için kullanılacak olan son yeme timestamp'ı
	philo->last_eat = get_time_ms();
	// Her bir philosopherın kendi ölüm kontrolünü yapacak olan thread başlatılır. 
	pthread_create(&checker_thread, NULL, &life_cycle_checker, philo);
	// Thread detach edilerek process bittiği anda hafızadan silinmesi gerektiği söylenir.
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
