#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(){
	int fd_1[2];
	int fd_2[2];

	int mensaje = random();
	int mensaje_recibido = 0;


	if (pipe(fd_1) == -1) {
		perror("pipe");
		_exit(-1) ;
	}

	if (pipe(fd_2) == -1) {
		perror("pipe");
		_exit(-1) ;
	}

	printf("Hola , soy PID : %d \n", getpid());
	printf(" - primer pipe me devuelve: [%d,%d]   \n", fd_1[0], fd_1[1]);
	printf(" - segundo pipe me devuelve: [%d, %d] \n", fd_2[0], fd_2[1]);


	int i = fork();

	if (i > 0) {
	
		if (write(fd_1[1], &mensaje, sizeof(mensaje)) == -1) {
			perror("write");
			_exit(-1) ;
		}
	
		printf("Donde fork me devuelve  %d \n", i);
		printf(" - getpid me devuelve:  %d \n", getpid());
		printf(" - getppid me devuelve  %d \n", getppid());
		printf(" - random me devuelve:  %d \n", mensaje);
		printf(" - envío valor %d a través de fd = %d \n",mensaje, fd_1[1]);

		if (close(fd_1[1]) != 0) {
			perror("close");
			_exit(-1) ;

		}


		if (read(fd_2[0], &mensaje, sizeof(mensaje)) == -1) {
			perror("read");
			_exit(-1) ;

		}
		
		wait(NULL) ;

		printf("Hola, de nuevo PID %d: \n", getpid());
		printf(" - recibí valor %d vía fd = %d \n", mensaje, fd_2[0]);

		if (close(fd_2[0]) != 0) {
			perror("close");
			_exit(-1) ;

		}
	}

	if (i == 0) {
	
		if (read(fd_1[0], &mensaje_recibido, sizeof(mensaje_recibido)) == -1) {
			perror("read");
			_exit(-1) ;

		}
	
		printf("Donde fork me devuelve 0 \n");
		printf(" - getpid me devuelve: %d  \n", getpid());
		printf(" - getppid me devuelve: %d \n", getppid());
		printf(" - recibo valor %d via fd = %d \n", mensaje, fd_1[0]);

		if (close(fd_1[0]) != 0) {
			perror("close");
			_exit(-1) ;

		}


		if (write(fd_2[1], &mensaje_recibido, sizeof(mensaje_recibido)) ==  -1) {
			perror("write");
			_exit(-1) ;

		}
	
		printf(" - reenvío valor en fd = %d y termino \n", fd_2[1]);


		if (close(fd_2[1]) != 0) {
			perror("close");
			_exit(-1) ;

		}


	}

	return 0;
}
