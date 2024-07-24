#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct stat st = {0};
/*****
* void crear_folder_Alfabeto
******
* La función crea una carpeta con el nombre
* de la letra pasada por parametro
* dentro de la carpeta Alfabeto
******
* Input:
*   char letra: letra para nombre de carpeta
******/
void crear_folder_Alfabeto(char letra){
    char direccion[] = "./Alfabeto/A";
    direccion[11] = letra;
    if (stat(direccion, &st) == -1) {
        mkdir(direccion, 0777);
    }
}

/*****
* void main_folders
******
* La función crea diferentes carpetas
* de acuerdo a lo pedido por la tarea
******/
void main_folders(){
    mkdir("./Alfabeto", 0777);
    mkdir("./Generacion", 0777);
    mkdir("./Generacion/I", 0777);
    mkdir("./Generacion/II", 0777);
    mkdir("./Generacion/III", 0777);
    mkdir("./Generacion/IV", 0777);
}

/*****
* void copiarImagen
******
* La función mueve las imagenes desde su
* directorio de origen al directorio
* solicitado.
******
* Input:
*   char *origen: directorio de origen de la imagen.
*   char *destino: directorio de destino de la imagen.
******/
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

/*****
* void escribir_txt
******
* La función escribe en un .txt la cantidad
* de pokemon por generacion y por letra de alfabeto.
******
* Input:
*   int *numero_generacion: Arreglo dinamico de enteros con
*   la cantidad de pokemon por generacion.
*   int *numero_alfabeto: Arreglo dinamico de enteros con
*   la cantidad de pokemon segun su primer letra del alfabeto.
******/
void escribir_txt(int *numero_generacion, int *numero_alfabeto){
    char *filename = "RegistroPokemon.txt";
    FILE *fp = fopen(filename, "w");

    fprintf(fp, "Generacion\n\n");

    fprintf(fp, "I - %d\n",numero_generacion[0]);
    fprintf(fp, "II - %d\n", numero_generacion[1]);
    fprintf(fp, "III - %d\n", numero_generacion[2]);
    fprintf(fp, "IV - %d\n\n", numero_generacion[3]);

    fprintf(fp, "Alfabetico\n\n");
    //65 es la 'A' en ASCII
    for(int i = 0; i<26; i++){
        fprintf(fp, "%c - %d\n", i + 65,numero_alfabeto[i]);
    }
    
    free(numero_generacion);
    free(numero_alfabeto);
    fclose(fp);
}

/*****
* void ordenar_pokemon
******
* La función lee la carpeta de sprites para poder ordenar los pokemon,
* el objetivo es leer la primera letra del pokemon y su numero
* segun esto hace llamadas a las funciones para copiar las imagenes 
* a sus directorios de destino correspondiente.
******/
void ordenar_pokemon(){
    DIR *folder;
    struct dirent *entry;
    int files = 0;
    char primer_letra;
    folder = opendir("./Sprites");
    char *ruta_origen;
    char *ruta_destino;
    char nombre_archivo[100];

    //Arreglos para conteo del txt
    int *numero_generacion = (int*)malloc(4*sizeof(int));
    int *numero_alfabeto = (int*)malloc(27*sizeof(int));

    //Inicializar los array en 0
    for (int i = 0; i < 4; i++){
        numero_generacion[i] = 0;
    }
    for (int i = 0; i < 27; i++){
        numero_alfabeto[i] = 0;
    }

    if(folder == NULL)
    {
        perror("Incapaz de leer el directorio");
        exit(EXIT_FAILURE);
    }

    while( (entry=readdir(folder)) )
    {
        files++;
        strcpy(nombre_archivo, entry->d_name);

        primer_letra = entry->d_name[0];

        //Aca se llena la carpeta del alfabeto
        if (primer_letra != '.'){

            //Logica para el arreglo de conteo del alfabeto
            numero_alfabeto[primer_letra-'a'] = numero_alfabeto[primer_letra-'a'] + 1;

            crear_folder_Alfabeto(toupper(primer_letra));

            char a[100]="./Sprites/";
            strcat(a, nombre_archivo);
            ruta_origen = a;
            
            char b[100]="./Alfabeto/";
            primer_letra= toupper(primer_letra);

            char letra[2] = {primer_letra, '\0'};
            strncat(b, letra, sizeof(letra) - 1);
            
            strcat(b, "/");
            strcat(b, nombre_archivo);
            ruta_destino=b;

            copiarImagen(ruta_origen,ruta_destino); 
        
            //Aca se llena la carpeta de la generacion
            int flag = 1;
            char caracter;
            int i = 1;
            while(flag == 1){
                caracter = entry->d_name[i];
                if(caracter == '_'){
                    flag = 0;
                }
                i++;
            }

            //i es el indice donde empieza el numero del pokemon
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
            int numero_int = atoi(numero_string);

            if (numero_int <=151){ //Gen1
                numero_generacion[0] = numero_generacion[0] + 1;
                char a[100]="./Sprites/";
                strcat(a, nombre_archivo);
                ruta_origen = a;

                char b[100]="./Generacion/I/";
                strcat(b, nombre_archivo);
                ruta_destino = b;

                copiarImagen(ruta_origen,ruta_destino); 
            }
            else if (numero_int <=251){//Gen2
                numero_generacion[1] = numero_generacion[1] + 1;
                char a[100]="./Sprites/";
                strcat(a, nombre_archivo);
                ruta_origen = a;

                char b[100]="./Generacion/II/";
                strcat(b, nombre_archivo);
                ruta_destino = b;

                copiarImagen(ruta_origen,ruta_destino); 
            }
            else if (numero_int <=386){ //Gen3
                numero_generacion[2] = numero_generacion[2] + 1;
                char a[100]="./Sprites/";
                strcat(a, nombre_archivo);
                ruta_origen = a;

                char b[100]="./Generacion/III/";
                strcat(b, nombre_archivo);
                ruta_destino = b;

                copiarImagen(ruta_origen,ruta_destino); 
            }
            else if (numero_int <=493){ //Gen4
                numero_generacion[3] = numero_generacion[3] + 1;
                char a[100]="./Sprites/";
                strcat(a, nombre_archivo);
                ruta_origen = a;

                char b[100]="./Generacion/IV/";
                strcat(b, nombre_archivo);
                ruta_destino = b;

                copiarImagen(ruta_origen,ruta_destino); 
            }
            //Aca se elimina de la carpeta de sprites
            remove(ruta_origen);
        }
    }

    closedir(folder);
    escribir_txt(numero_generacion, numero_alfabeto);
}



int main(){
    main_folders();
    ordenar_pokemon();
    return 0 ;
}