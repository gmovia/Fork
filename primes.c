#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#ifndef N
#define N 10000
#endif

int main(int argc, char *argv[]){
	
	int n = N;

	int file_descriptor_1[2];

	if (pipe(file_descriptor_1) == -1) {
		perror("pipe");
		_exit(-1) ;

	}

	for (int i = 2; i <= n; i++) {
		write(file_descriptor_1[1], &i, sizeof(i));
	}

	if (close(file_descriptor_1[1]) != 0) {
		perror("close");
		_exit(-1) ;

	}

	int primo;
	int file_descriptor_2[2];
	int termine_de_leer;

	while (termine_de_leer != 0) {
	
		int i = fork();

		if (i == 0) {

			if (pipe(file_descriptor_2) != 0) {
				perror("pipe");
				_exit(-1) ;

			}

			termine_de_leer = read(file_descriptor_1[0], &primo, sizeof(primo));
			
			if (termine_de_leer > 0) {
				printf("primo %d \n", primo);
			}

			int j;

			while (read(file_descriptor_1[0], &j, sizeof(j)) > 0) {
				if (j % primo != 0) {
					write(file_descriptor_2[1], &j, sizeof(j));
				}
			}

			if (close(file_descriptor_2[1]) != 0) {
				perror("close");
				_exit(-1) ;

			}

			if (close(file_descriptor_1[0]) != 0) {
				perror("close");
				_exit(-1) ;

			}


			file_descriptor_1[0] = file_descriptor_2[0];
		}


		if (i > 0) {

			wait(NULL);

			if (close(file_descriptor_1[0]) != 0) {
				perror("close");
				_exit(-1) ;

			}
			
			_exit(0) ;
		}


		if (i < 0) {

			perror("fork") ;
			_exit(-1);
		}

	}

	return 0;
}
