/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeser <aeser@42kocaeli.com.tr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 20:53:49 by aeser             #+#    #+#             */
/*   Updated: 2022/06/08 19:32:04 by aeser            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>
# include <inttypes.h>
# include <sys/time.h>

# define FAILURE -1

# define STR_THINKING	"is thinking"
# define STR_EATING		"is eating"
# define STR_SLEEPING	"is sleeping"
# define STR_TOOK_FORK	"has taken a fork"
# define STR_DEAD		"died"

typedef enum e_state
{
	THINKING,
	EATING,
	SLEEPING,
	TOOK_FORK,
	DEAD
}	t_state;

typedef struct s_env	t_env;

typedef struct s_philo
{
	int				id;
	int				l_fork;
	int				r_fork;
	int				eat_count;
	bool			done;
	uint64_t		last_eat;
	pthread_t		thread_id;
	t_env			*env;
}	t_philo;

typedef struct s_env
{
	int				n_philo;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				must_eat;
	int				count_done;
	bool			is_running;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write;
}	t_env;

// init.c
void		init_arguments(int argc, char **argv, t_env *env);
void		init_mutexes(t_env *env);
void		init_philos(t_env *env);
void		init_threads(t_env *env);

// cleanup.c
void		destroy(t_env *env);

// philo_thread.c
void		*life_cycle(void *arg);
void		*life_cycle_checker(void *arg);

// utils.c
void		print_state(t_philo *philo, t_state state, uint64_t timestamp);
uint64_t	get_time_ms(void);

// actions.c
void		take_forks(t_philo *philo, uint64_t timestamp);
void		leave_forks(t_philo *philo);
void		philo_eat(t_philo *philo, uint64_t timestamp);
void		philo_sleep(t_philo *philo);
void		philo_think(t_philo *philo);

#endif
