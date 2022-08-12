#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void find( DIR *flujo , char *nombre_directorio ,char *consulta , char *(*funcion)(const char *, const char *) ) ;

void find( DIR *flujo, char *nombre_directorio,char *consulta,char *(*funcion)(const char *, const char *) ) {

	struct dirent *directorio;

	if (flujo != NULL) {
	
		while ((directorio = readdir(flujo)) != NULL) {
	
			char path[1000] = { 0 };

			strcat(path, nombre_directorio);

			strcat(path, "/");

			strcat(path, directorio->d_name);

			if (funcion(directorio->d_name, consulta) != NULL) {
				printf("%s \n", path);
			}

			if (directorio->d_type == 4 && strcmp(directorio->d_name, ".") != 0 && strcmp(directorio->d_name, "..") != 0) {
				
				DIR *nuevo_flujo;

				int fd = openat(dirfd(flujo) , directorio->d_name, O_DIRECTORY) ;

				nuevo_flujo = fdopendir(fd);

				find(nuevo_flujo, path, consulta, funcion);
			}
		}
	}

	closedir(flujo);
}

int main(int argc, char *argv[]) {

	char *consulta;

	DIR *flujo;

	flujo = opendir(".");

	if (argc > 2) {

		consulta = argv[2];

		find(flujo, "", consulta, strcasestr);

	} else {

		consulta = argv[1];

		find(flujo, "", consulta, strstr);
	}

	return 0;
}
