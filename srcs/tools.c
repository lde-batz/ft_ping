
#include "ft_ping.h"

int		ft_arraystrlen(char **array)
{
	int i = -1;

	if (!array)
		return -1;
	while (array[++i])
		;
	return i;
}

double	*ft_atof_strict(char *str, double *nb)
{
	int		i;
	double	ret;
	int		sign;
	int		point;
	int		error;

	i = 0;
	ret = 0;
	sign = 1;
	error = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == 43 || str[i] == 45)
		sign = (str[i++] == 45) ? 0 : sign;
	if (str[i] >= '0' && str[i] <= '9')
		error = 0;
	while (str[i] >= '0' && str[i] <= '9')
		ret = ret * 10 + (str[i++] - 48);
	if (!str[i])
	{
		ret = (!sign) ? -ret : ret;
		*nb = ret;
		return nb;
	}
	if (str[i] != ',' && str[i] != '.')
		return NULL;
	point = i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		ret += (str[i] - 48) * ft_powf(10, point - i);
		i++;
	}
	if (error || point + 1 == i)
		return NULL;
	ret = (!sign) ? -ret : ret;
	*nb = ret;
	return nb;
}

double	ft_sqrtf(const double m)
{
	int		j;
	double	i = 0;
	double	x1, x2;

	while (i * i <= m)
		i += 0.1f;
	x1 = i;
	for (j = 0; j < 10; j++)
	{
		x2 = m;
		x2 /= x1;
		x2 += x1;
		x2 /= 2;
		x1 = x2;
	}
	return x2;
}
