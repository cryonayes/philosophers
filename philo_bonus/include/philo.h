/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeser <aeser@42kocaeli.com.tr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 20:53:49 by aeser             #+#    #+#             */
/*   Updated: 2022/06/08 22:30:43 by aeser            ###   ########.fr       */
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
# include <semaphore.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>

# define FAILURE -1

# define STR_THINKING	"is thinking"
# define STR_EATING		"is eating"
# define STR_SLEEPING	"is sleeping"
# define STR_TOOK_FORK	"has taken a fork"
# define STR_DEAD		"died"

# define SEM_FORK		"/sem_forks"
# define SEM_WRITE		"/sem_write"
# define SEM_DIED		"/sem_died"
# define SEM_FINISH		"/sem_finish"

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
	volatile int	eat_count;
	uint64_t		last_eat;
	t_env			*env;
}	t_philo;

typedef struct s_env
{
	int				n_philo;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				must_eat;
	int				*pids;
	t_philo			*philos;
	pthread_t		checker;

	sem_t			*sem_forks;
	sem_t			*sem_write;
	sem_t			*sem_done;
}	t_env;

// init.c
void		init_arguments(int argc, char **argv, t_env *env);
void		init_semaphore(t_env *env);
void		init_philos(t_env *env);
void		init_philo_process(t_env *env);

// cleanup.c
void		destroy_semaphores(void);
void		destroy(t_env *env);

// philo_thread.c
void		start_lifecycle(t_philo *philo);
void		*life_cycle_checker(void *arg);
void		*checker_function(void *arg);

// utils.c
void		print_state(t_philo *philo, t_state state, uint64_t timestamp);
uint64_t	get_time_ms(void);

// actions.c
void		take_fork(t_philo *philo);
void		leave_fork(t_philo *philo);
void		philo_eat(t_philo *philo, uint64_t timestamp);
void		philo_sleep(t_philo *philo);
void		philo_think(t_philo *philo);

#endif
