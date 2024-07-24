#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void copiarImagen(char *origen, char *destino) {
    FILE *file_origen;
    FILE *file_destino;
    char imagen;

    file_origen = fopen(origen, "rb");
    if (file_origen == NULL) {
        perror("Error al abrir el archivo de origen");
        exit(EXIT_FAILURE);
    }

    file_destino = fopen(destino, "wb");
    if (file_destino == NULL) {
        perror("Error al abrir el archivo de destino");
        fclose(file_origen);
        exit(EXIT_FAILURE);
    }

    // Copia las imagenes
    while ((imagen = fgetc(file_origen)) != EOF) {
        fputc(imagen, file_destino);
    }

    fclose(file_origen);
    fclose(file_destino);
}

void delete_directory(const char *dir_path) {
    DIR *dir;
    struct dirent *entry;

    // Abre el directorio
    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("Error al abrir el directorio");
        exit(EXIT_FAILURE);
    }

    // Itera sobre los elementos del directorio
    while ((entry = readdir(dir)) != NULL) {
        // Ignora "." y ".."
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

            struct stat statbuf;
            if (stat(path, &statbuf) == -1) {
                perror("Error al obtener la información del archivo");
                exit(EXIT_FAILURE);
            }

            // Si es un directorio, llámate a ti mismo recursivamente
            if (S_ISDIR(statbuf.st_mode)) {
                delete_directory(path);
            } else {
                // Si es un archivo, elimínalo
                if (remove(path) != 0) {
                    perror("Error al eliminar el archivo");
                    exit(EXIT_FAILURE);
                } else {
                    printf("Archivo eliminado: %s\n", path);
                }
            }
        }
    }

    // Cierra el directorio
    closedir(dir);

    // Elimina el directorio
    if (rmdir(dir_path) != 0) {
        perror("Error al eliminar el directorio");
        exit(EXIT_FAILURE);
    } else {
        printf("Directorio eliminado: %s\n", dir_path);
    }
}

int main() {
    /*
    char *ruta_origen = "./Sprites/abra_63.png";
    char *ruta_destino = "./Temp/abra_63.png";

    copiarImagen(ruta_origen, ruta_destino);
    */

    const char *directory_to_delete = "./main";
    delete_directory(directory_to_delete);
    return 0;


    return 0;
}

