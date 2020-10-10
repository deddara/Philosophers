#include <unistd.h>
#include <pthread.h>


void  *philo(void *val)
{
	t_table *table = (t_table *)val;
	pthread_mutex_lock(&table->l_f);
	write(1, "s\n", 2);
	pthread_mutex_unlock(&table->l_f);
	return NULL;
}

static int init(t_table *table)
{

}

int main(int argc, char **argv)
{
	t_table		table;
	pthread_t	thread;
	pthread_t	thread1;

	if (init(&table, char **argv))
		return(1);

	pthread_mutex_init(&table.l_f, NULL);
	pthread_create(&thread, NULL, philo, &table);
	pthread_create(&thread1, NULL, philo, &table);

	pthread_join(thread, NULL);


	return (0);
}