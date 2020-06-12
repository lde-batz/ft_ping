
#include "ft_ping.h"

char	*ft_strslice(char *str, int slice)
{
	char	*new;
	int		i;
	int		len;

	len = ft_strlen(str);
	i = 0;
	if (!(new = (char*)malloc(sizeof(char) * (len - slice))))
		return (NULL);
	while (++slice < len)
		new[i++] = str[slice];
	new[i] = '\0';
	return (new);
}

int		ft_atof_strict1(char *str, int *i, int *sign, double *ret)
{
	int error;

	error = 1;
	while ((str[*i] >= 9 && str[*i] <= 13) || str[*i] == 32)
		(*i)++;
	if (str[*i] == 43 || str[*i] == 45)
		sign = (str[*i++] == 45) ? 0 : sign;
	if (str[*i] >= '0' && str[*i] <= '9')
		error = 0;
	while (str[*i] >= '0' && str[*i] <= '9')
		*ret = *ret * 10 + (str[*i++] - 48);
	return (error);
}

void	ft_atof_strict2(char *str, int *i, int *point, double *ret)
{
	*point = *i++;
	while (str[*i] >= '0' && str[*i] <= '9')
	{
		*ret += (str[*i] - 48) * ft_powf(10, *point - *i);
		(*i)++;
	}
}

double	*ft_atof_strict(char *str, double *nb)
{
	int		i;
	int		sign;
	int		point;
	int		error;
	double	ret;

	i = 0;
	ret = 0;
	sign = 1;
	error = ft_atof_strict1(str, &i, &sign, &ret);
	if (!str[i])
	{
		ret = (!sign) ? -ret : ret;
		*nb = ret;
		return (nb);
	}
	if (str[i] != ',' && str[i] != '.')
		return (NULL);
	ft_atof_strict2(str, &i, &point, &ret);
	if (error || point + 1 == i)
		return (NULL);
	ret = (!sign) ? -ret : ret;
	*nb = ret;
	return (nb);
}

double	ft_sqrtf(const double m)
{
	int		j;
	double	i;
	double	x1;
	double	x2;

	i = 0;
	while (i * i <= m)
		i += 0.1f;
	x1 = i;
	j = 0;
	while (j++ < 10)
	{
		x2 = m;
		x2 /= x1;
		x2 += x1;
		x2 /= 2;
		x1 = x2;
	}
	return (x2);
}
