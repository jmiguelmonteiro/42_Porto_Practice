#include "utils.c"

int ft_popen(const char *file, char *const argv[], char type)
{

	(void)file;
	(void)argv;
	(void)type;

	return (-1);
}

int main()
{
	int  fd;
	char *line;

	fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
	while ((line = get_next_line(fd)))
		ft_putstr_fd(line, STDOUT_FILENO);
	return (0);
}

// int	main() {
// 	int	fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
// 	dup2(fd, 0);
// 	fd = ft_popen("grep", (char *const []){"grep", "c", NULL}, 'r');
// 	char	*line;
// 	while ((line = get_next_line(fd)))
// 		printf("%s", line);
// }
