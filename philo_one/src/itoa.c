#include "philo.h"

static int		num_len(int n)
{
	int count;

	count = 0;
	if (n == 0)
		return (1);
	while (n)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

static char		*resulter(char *result, int n, int len)
{
	int i;
	int digit;

	i = 0;
	if (len > num_len(n))
	{
		result[i] = '-';
		i++;
	}
	result[len] = '\0';
	if (n == 0)
		result[i] = '0';
	len--;
	while (n > 0)
	{
		digit = n % 10;
		result[len] = digit + '0';
		n = n / 10;
		len--;
	}
	return (result);
}

static char		*min_int_allocation(int n)
{
	int		l;
	int		digit;
	char	*result;

	l = num_len(n);
	result = (char*)malloc(sizeof(char) * (l + 2));
	result[0] = '-';
	result[l + 1] = '\0';
	while (n)
	{
		digit = n % 10;
		result[l] = (digit * -1) + '0';
		n = n / 10;
		l--;
	}
	return (result);
}

char			*ft_itoa(int n)
{
	int		len;
	char	*result;

	if (n == -2147483648)
		return (min_int_allocation(n));
	len = num_len(n);
	if (n < 0)
	{
		n = n * (-1);
		len++;
	}
	result = (char*)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	result = resulter(result, n, len);
	return (result);
}