/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeser <aeser@42kocaeli.com.tr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 20:53:54 by aeser             #+#    #+#             */
/*   Updated: 2022/06/06 21:02:17 by aeser            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_env	env;

	init_arguments(argc, argv, &env);
	init_mutexes(&env);
	init_philos(&env);
	init_threads(&env);
	destroy_threads(&env);
	destroy_mutexes(&env);
}
