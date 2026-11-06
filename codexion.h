#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_sim	t_sim;

typedef enum e_scheduler
{
	FIFO,
	EDF,
	ERROR
}	t_scheduler;

typedef struct s_config
{
	int	number_of_coders;
	int	time_to_burnout;
	int	time_to_compile;
	int	time_to_debug;
	int time_to_refactor;
	int	number_of_compiles_required;
	int	dongle_cooldown;
	t_scheduler scheduler;
}	t_config;

typedef struct s_dongle
{
	int				id;
	int				in_use;
	long			available_at;
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	long			last_compile_start;
	int				compiles_done;
	pthread_mutex_t	state_lock;
	t_sim			*program;
}	t_coder;

typedef struct s_sim
{
	t_config		config;
	t_coder			*coders;
	t_dongle		*dongles;
	long			start_time;
	pthread_mutex_t	log_lock;
	int				simulation_over;
	pthread_mutex_t	end_lock;
	pthread_t		monitor_thread;
}	t_sim;

	// Parsing
	t_config	parser(char **argv);
	int			config_checker(t_config config);

	// Parsing Utils
	long		ft_atoi(const char *nptr);

	// Time
	long		get_time_ms(void);

	// Dongles
	int		init_dongles(t_sim *sim);

	// Coders
	int		init_coders(t_sim *program);

	// test
	void	debug_print_sim(t_sim *sim);

	// Routine
	void	*coder_routine(void *arg);

	// Log
	void	print_log(t_coder *coder, char *action);

	// Simulator
	int		start_threads(t_sim *program);

#endif
