#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
	char line[1024 * 4] = {0};
	char *argv[] = {"ls", NULL};
	char *env[] = {NULL};
	int pid = -1;

	while (1) {
		printf("?> ");
		fgets(line, 1024 * 4, stdin);
		printf("%s\n", line);
		pid = fork();
		printf("pid: %d\n", pid);
		if (pid == 0) {
			execve("/bin/ls", argv, env);
			exit(-1);
		}
		waitpid(pid, NULL, 0);
	}
	return 0;

	
}
