#include "philo.h"

void	ft_putstr_fd(char *s, int fd)
{
	int i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

int check_for_num(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '+' && i == 0)
		{
			i++;
			continue ;
		}
		if (str[i] < 48 || str[i] > 57)
		{
			return (1);
		}
		i++;
	}
	return (0);
}