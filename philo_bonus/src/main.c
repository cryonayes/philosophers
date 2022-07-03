/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeser <aeser@42kocaeli.com.tr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 20:53:54 by aeser             #+#    #+#             */
/*   Updated: 2022/07/03 15:39:06 by aeser            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_env	env;

	init_arguments(argc, argv, &env);
	init_semaphore(&env); // Sistem üzerinde semaphore kaydını yaptır.
	init_philos(&env);
	init_philo_process(&env); // Her bir philosopher bir process olacak şekilde başlatılır.
	pthread_create(&env.checker, NULL, checker_function, &env);
	pthread_detach(env.checker);
	sem_wait(env.sem_done); // sem_done 0'dan büyük olana kadar bekle.
	destroy(&env);
}

/*
	Semaphore'lar içerisinde integer değerler tutarlar ve bu değerler dosya sisteminde
	saklanır, böylece farklı process'ler bu değerlere dosya gibi ulaşabilir ve iletişim
	sağlayabilirler.

	sem_post semaphore içerisinde saklanan integer degerini bir arttırır.
	sem_wait ise bu değeri bir eksiltir.
	Eğer semaphore'un tuttuğu değer 0 ise, sem_wait semaphore'un değeri 0'dan büyük
	olana kadar kodu bloklar ve bekler.

	env.sem_done -> başlangıç değeri 0'dır ve bir filozof ölene kadar da 0 kalır.
	sem_wait bir filozofun ölmesiyle beraber artan sem_done değerini bir eksiltir
	ve kodu kaldığı yerden devam ettirir. 
*/
