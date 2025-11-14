#include "utils.c"

int ft_popen(const char *file, char *const argv[], char type)
{
	pid_t	pid;
	int		fd[2];

	if (!file || !argv || (type != 'r' && type != 'w'))
		return (-1);

	if (pipe(fd) == -1)
		return (-1);
	// fd[0] - read end
	// fd[1] - write end

	pid = fork();
	if (pid == -1)
	{
		// Fork failed
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		// Child process
		if (type == 'r')
		{
			if (dup2(fd[1], STDOUT_FILENO) == -1)
				exit(EXIT_FAILURE);
		}
		else
		{
			if (dup2(fd[0], STDIN_FILENO) == -1)
				exit(EXIT_FAILURE);
		}
		close(fd[0]);
		close(fd[1]);
		execvp(file, argv);
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		// Parent process
		if (type == 'r')
		{
			close(fd[1]); // Close write end
			return (fd[0]); // Return read end
		}
		else
		{
			close(fd[0]); // Close read end
			return (fd[1]); // Return write end
		}
	}
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
