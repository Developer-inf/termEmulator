#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define MAXNAME 256	//max filename length
#define LENGTH 256	//max length of comand

char cwd[MAXNAME];				//current directory
char command[LENGTH] = {""};	//command buffer
char **args;					//array with command and arguments

//split command by spaces
char **split(char *cmd, char **buf) {
	char *tmp;
	int cnt = 1;
	int i = 0;
	//counting words
	while(cmd[i] != '\0' && cmd[i] != '\n') {
		if (cmd[i] == ' ')
			cnt++;
		i++;
	}
	//reallocate memory for args
	free(buf);
	buf = (char**)malloc((cnt + 1) * sizeof(char*));
	int j = 0;
	
	//using strtok for spliting command
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

		//reading command
		fgets(command, LENGTH, stdin);
		if (command[strlen(command) - 1] == '\n')
			command[strlen(command) - 1] = '\0';
		if(strcmp(command, "exit") == 0)
			break;

		args = (char**)malloc(sizeof(char*));
		args = split(command, args);

		//realizing 'cd' command
		if(strcmp(args[0], "cd") == 0 && args[1] != NULL) {
			int err = chdir(args[1]);
			free(args);
			if(err < 0)
				fprintf(stderr, "can't change directory\n");
			continue;
		}

		//fork main process for execvp
		int p = fork();
		if(p == 0) {
			execvp(args[0], args);
			fprintf(stderr, "can't run program \"%s\"\n", args[0]);
			exit(0);
		}
		free(args);
		//waiting for child process end otherwise it will corrupt output
		wait(NULL);
	}

	exit(0);
}
