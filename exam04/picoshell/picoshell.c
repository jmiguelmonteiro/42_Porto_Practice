#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

int    picoshell(char **cmds[])
{
	int		i = 0;
	int		n_commands = 0;
	int		fd[2];
	int 	prev_fd = -1;
	pid_t	pid;

	while (cmds[n_commands])
		n_commands++;

	if (n_commands == 0)
		return 0;

	while (i < n_commands)
	{
		// If not the last
		if (i < (n_commands - 1))
			if (pipe(fd) == -1)
				return 1;
		pid = fork();
		if (pid == -1)
		// Error on fork
		{
			close(fd[0]);
			close(fd[1]);
			return 1;
		}
		if (pid == 0)
		// Child process
		{
			// If not the first
			if (i > 0)
			{
				if (dup2(prev_fd, STDIN_FILENO) == -1)
					exit(1);
				close(prev_fd);
			}
			// If not the last
			if (i < (n_commands - 1))
			{
				if (dup2(fd[1], STDOUT_FILENO) == -1)
					exit(1);
				close(fd[0]);
				close(fd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		else
		// Parent process
		{
			// If not the first
			if (i > 0)
				close(prev_fd);
			// If not the last
			if (i < (n_commands - 1))
			{
				close(fd[1]);
				prev_fd = fd[0];
			}
		}
		i++;
	}

	for (i = 0; i < n_commands; i++)
		wait(NULL);

	return 0;
}

int main(void)
{
	// First command: echo squalala
	char *cmd1[] = {"echo", "squalala", NULL};

	// Second command: cat
	char *cmd2[] = {"cat", NULL};

	// Third command: sed 's/a/b/g'
	char *cmd3[] = {"sed", "s/a/b/g", NULL};

	// Array of commands (NULL-terminated)
	char **cmds[] = {cmd1, cmd2, cmd3, NULL};

	// Execute the pipeline
	int result = picoshell(cmds);

	printf("\nReturn value: %d\n", result);
}

// static int count_cmds(int argc, char **argv)
// {
//     int count = 1;
//     for (int i = 1; i < argc; i++)
//     {
//         if (strcmp(argv[i], "|") == 0)
//             count++;
//     }
//     return count;
// }

// int main(int argc, char **argv)
// {
//     if (argc < 2)
//         return (fprintf(stderr, "Usage: %s cmd1 [args] | cmd2 [args] ...\n", argv[0]), 1);

//     int cmd_count = count_cmds(argc, argv);
//     char ***cmds = calloc(cmd_count + 1, sizeof(char **));
//     if (!cmds)
//         return (perror("calloc"), 1);

//     // Parsear argumentos y construir array de comandos
//     int i = 1, j = 0;
//     while (i < argc)
//     {
//         int len = 0;
//         while (i + len < argc && strcmp(argv[i + len], "|") != 0)
//             len++;
        
//         cmds[j] = calloc(len + 1, sizeof(char *));
//         if (!cmds[j])
//             return (perror("calloc"), 1);
        
//         for (int k = 0; k < len; k++)
//             cmds[j][k] = argv[i + k];
//         cmds[j][len] = NULL;
        
//         i += len + 1;  // Saltar el "|"
//         j++;
//     }
//     cmds[cmd_count] = NULL;

//     int ret = picoshell(cmds);

//     // Limpiar memoria
//     for (int i = 0; cmds[i]; i++)
//         free(cmds[i]);
//     free(cmds);

//     return ret;
// }
