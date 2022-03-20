#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define MAXNAME 256
#define LENGTH 256

char cwd[MAXNAME];
char command[LENGTH] = {""};
char **args;

char **split(char *cmd, char **buf) {
	char *tmp;
	int cnt = 1;
	int i = 0;
	while(cmd[i] != '\0' && cmd[i] != '\n') {
		if (cmd[i] == ' ')
			cnt++;
		i++;
	}
	free(buf);
	buf = (char**)malloc((cnt + 1) * sizeof(char*));
	int j = 0;

	tmp = strtok(cmd, " ");
	while(tmp != NULL) {
		buf[j] = tmp;
		j++;
		tmp = strtok(NULL, " ");
	}
	buf[j] = NULL;

	return buf;
}

int main() {
	printf("Hello, it's a terminal emulator\n");

	while (1) {
		if(getcwd(cwd, MAXNAME) == NULL)
			perror("getcwd() error\n");
		printf("%s$ ", cwd);
		fgets(command, LENGTH, stdin);
		if (command[strlen(command) - 1] == '\n')
			command[strlen(command) - 1] = '\0';
		if(strcmp(command, "exit") == 0)
			break;
		args = (char**)malloc(sizeof(char*));
		args = split(command, args);

		int p = fork();
		if(p == 0) {
			execvp(args[0], args);
			fprintf(stderr, "can't run program \"%s\"\n", args[0]);
			exit(0);
		}
		free(args);
		wait(NULL);
	}

	exit(0);
}
