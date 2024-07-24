#include <stdio.h>
#include <dirent.h>
#include <string.h>
//#include <unistd.h>
//#include <sys/types.h>

int main()
{
    DIR *folder;
    struct dirent *entry;
    int files = 0;

    char primer_letra;
    folder = opendir("./Sprites");
    if(folder == NULL)
    {
        perror("Unable to read directory");
        return(1);
    }

    while( (entry=readdir(folder)) )
    {
        files++;
        printf("File %3d: %s\n",
            files,
            entry->d_name
        );

        char nombre_archivo[260];
        strcpy(nombre_archivo, entry->d_name);
        printf("El nombre del archivo es: %s \n", nombre_archivo);

        primer_letra = entry->d_name[0];
        printf("La primer letra es: %c \n",primer_letra);

        if (primer_letra == '.'){
            printf("Malo\n");
        } else {
            int flag = 1;
            char letra;
            int i = 1;
            while(flag == 1){
                letra = entry->d_name[i];
                if(letra == '_'){
                    flag = 0;
                }
                i++;
            }

            // i es el indice donde empieza el numero
            char numero_string[4] = {0};
            flag = 1;
            int posicion = 0;
            while(flag == 1){
                numero_string[posicion] = entry->d_name[i];
                i++;
                posicion++;
                if(entry->d_name[i] == '.'){
                    flag = 0;
                }
            }
            printf("El numero es %s \n \n", numero_string);

        }

        
    }

    closedir(folder);

    return(0);
}