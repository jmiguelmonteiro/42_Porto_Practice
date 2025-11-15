#include <unistd.h>
#include <stdlib.h>

int ft_popen(const char *file, char *const argv[], char type)
{
	pid_t	pid;
	int		fd[2];

	if (!file || !argv || (type != 'r' && type != 'w'))
		return (-1);

	if (pipe(fd) == -1)
		return (-1);

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
