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

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
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

char	*ft_strjoin_philo(char *time, char *id, char *action)
{
	char	*result;
	int		i;
	int 	j;
	int		count;

	i = 0;
	while (time[i])
		i++;
	count = i;
	i = 0;
	while (id[i])
		i++;
	count += i;
	i = 0;
	while (action[i])
		i++;
	count += i;

	if (!(result = (char*)malloc(sizeof(char) * count + 3)))
		return (NULL);
	i = 0;
	j = 0;
	while (time[i])
		result[j++] = time[i++];
	result[j++] = ' ';
	i = 0;
	while (id[i])
		result[j++] = id[i++];
	result[j++] = ' ';
	i = 0;
	while (action[i])
		result[j++] = action[i++];
	result[j] = '\0';
	return (result);
}
