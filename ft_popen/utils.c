#include "gnl/get_next_line.h"
#include <unistd.h>

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (fd != -1)
		write(fd, s, ft_strlen(s));
}
