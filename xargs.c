#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#ifndef NARGS
#define NARGS 4
#endif


void ejecucion(char *args[], int *p, char *arg){

	args[*p + 1] = NULL;

	*p = 0;

	int i = fork();

	if (i < 0) {
		perror("fork");
		_exit(-1);
	}

	if (i > 0) {

		wait(NULL);

		for (int j = 1; j < NARGS + 2; j++) {
			free(args[j]);
			args[j] = NULL;
		}
	}

	if (i == 0) {
		execvp(arg, args);
		perror("execvp");
		_exit(-1);
	}
}


int main(int argc, char *argv[]){

	char *args[NARGS + 2] = { argv[1], NULL };

	char *linea = NULL;

	int contador = 0;

	int *p = &contador;

	size_t car;


	while (getline(&args[contador + 1], &car, stdin) > 0) {

		int len = strlen(args[contador + 1]);

		if (args[contador + 1][len - 1] == '\n') {

			args[contador + 1][len - 1] = '\0';
		}

		contador++;

		if ((contador == NARGS)) {

			ejecucion(args, p, argv[1]);

		}

	}

	if (contador > 0) {

		ejecucion(args, p, argv[1]);

	}

	return 0;
}
