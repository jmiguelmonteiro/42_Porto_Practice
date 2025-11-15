#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

// 1	- nice
// 0	- bad
// -1	- error

void test_function(void)
{
	sleep(10);
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t pid;
	int status;
	// int ret;

	pid = fork();
	if (pid == -1)
		return -1;
	if (pid == 0)
	{
		// Child
		alarm(timeout);
		f();
		exit(0);
	}
	else
	{
		// Parent
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 0)
			{
				if (verbose)
					printf("Nice function!\n");
				return 1;
			}
			if (verbose)
				printf("Bad function: exited with code %d\n", WEXITSTATUS(status));
			return 0;
		}
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGALRM)
			{
				if (verbose)
					printf("Bad function: timed out after %d seconds\n", timeout);
				return 0;
			}
			if (verbose)
				printf("Bad function: %s\n", strsignal(WTERMSIG(status)));
			return 0;
		}
	}
	return 0;
}

int main(void)
{
	int ret;

	ret = sandbox(&test_function, 5, true);
	printf("Return value: %d\n", ret);

	return 0;
}
