import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;


public class BaquedanoThread{
    public static char[] laberinto;

    public static void main(String[] args) {
        long tiempoInicio = System.nanoTime();
        // Ruta relativa al archivo de texto
        String ruta = "laberinto.txt";

        Funciones funciones = new Funciones();

        int dimension;
        int[] posicion = new int[2];
        char[][] matriz_laberinto;
        int norte=0;
        int sur=0;
        int este=0;
        int oeste=0;
        char camino_anterior ='L';
        char camino='L';

        List<Thread> hilos = new ArrayList<>();
            
        try (BufferedReader br = new BufferedReader(new FileReader(ruta))) {    //verifica si se abre bien el txt
            String linea;
            String[] partes;
            char[] partesChar;

            linea = br.readLine();
            partes = linea.split("x");
            dimension =Integer.parseInt(partes[0]); //obtiene la dimension del laberinto
            matriz_laberinto = new char[dimension][dimension];

            linea = br.readLine();
            linea = linea.substring(1, linea.length() - 1);
            partes = linea.split(",");
            posicion[0] = Integer.parseInt(partes[0]);
            posicion[1] = Integer.parseInt(partes[1]);


            for(int i=0;i<dimension;i++){   //lee linea a linea de la matriz
                linea = br.readLine();
                linea=linea.replace(" ","");
                partesChar = linea.toCharArray();
                for(int j=0;j<dimension;j++){
                    matriz_laberinto[j][i]=partesChar[j];   //guarda los elementos en la matriz
                }
            }
            matriz_laberinto[posicion[0]][posicion[1]] = '0';
            //Buscar caminos
            while(true){
                if (camino_anterior!='N'){
                    norte = funciones.see(posicion[0], posicion[1], 'N', matriz_laberinto, dimension);
                    if (norte >=10){
                        posicion[1] -= 1;
                    }
                }
                if (camino_anterior!='S'){
                    sur = funciones.see(posicion[0],posicion[1], 'S', matriz_laberinto, dimension);
                    if (sur >=10){
                        posicion[1] += 1;
                    }
                }
                if (camino_anterior!='E'){
                    este = funciones.see(posicion[0],posicion[1], 'E', matriz_laberinto, dimension);
                    if (este >=10){
                        posicion[0] += 1;
                    }
                }
                if (camino_anterior!='O'){
                    oeste = funciones.see(posicion[0], posicion[1], 'O', matriz_laberinto, dimension);
                    if (oeste >=10){
                        posicion[0] -= 1;
                    }
                }

                //----------------------------------------------------------------------------------------------------//
                //Posibles casos
                //Un camino
                if((norte+sur+este+oeste)==1){
                    if(norte==1){
                        camino='N';
                        posicion = funciones.avanzar(posicion, camino);
                        camino_anterior='S';
                    }
                    else if(sur==1){
                        camino='S';
                        posicion = funciones.avanzar(posicion, camino);
                        camino_anterior='N';
                    }
                    else if(este==1){
                        camino='E';
                        posicion = funciones.avanzar(posicion, camino);
                        camino_anterior='O';
                    }
                    else if(oeste==1){
                        camino='O';
                        posicion = funciones.avanzar(posicion, camino);
                        camino_anterior='E';
                    }
                    matriz_laberinto[posicion[0]][posicion[1]] = '0';
                }
                //Se llego a salida
                else if((norte+sur+este+oeste)>=10){
                    System.out.println("Se encontro una salida en la hebra: " + Thread.currentThread().getId() + " Con posicion x: "+ posicion[0] + " y: " + posicion[1]);
                    break;
                }
                //Camino sin salida
                else if((norte+sur+este+oeste)==0){
                    System.out.println("No se encontro una salida en la hebra: " + Thread.currentThread().getId() + " Con posicion x: "+ posicion[0] + " y: " + posicion[1]);
                    break;
                }
                //Bifurcacion
                else if((norte+sur+este+oeste)<=4){
                    if(este==1){
                        camino_anterior = 'O';
                        int[] posicionCopia = posicion.clone();
                        MiRunnable miRunnable = new MiRunnable(matriz_laberinto,funciones.avanzar(posicionCopia, 'E'),dimension,camino_anterior);
                        Thread miHebra = new Thread(miRunnable);
                        hilos.add(miHebra);
                        miHebra.start();
                    }
                    if(oeste==1){
                        camino_anterior = 'E';
                        int[] posicionCopia = posicion.clone();
                        MiRunnable miRunnable = new MiRunnable(matriz_laberinto,funciones.avanzar(posicionCopia, 'O'),dimension,camino_anterior);
                        Thread miHebra = new Thread(miRunnable);
                        hilos.add(miHebra);
                        miHebra.start();
                    }
                    if(norte==1){
                        camino_anterior = 'S';
                        int[] posicionCopia = posicion.clone();
                        MiRunnable miRunnable = new MiRunnable(matriz_laberinto,funciones.avanzar(posicionCopia, 'N'),dimension,camino_anterior);
                        Thread miHebra = new Thread(miRunnable);
                        hilos.add(miHebra);
                        miHebra.start();
                    }
                    if(sur==1){
                        camino_anterior = 'N';
                        int[] posicionCopia = posicion.clone();
                        MiRunnable miRunnable = new MiRunnable(matriz_laberinto,funciones.avanzar(posicionCopia, 'S'),dimension,camino_anterior);
                        Thread miHebra = new Thread(miRunnable);
                        hilos.add(miHebra);
                        miHebra.start();
                    }
                    break;
                }

                //Resetear variables
                norte=0;
                sur=0;
                este=0;
                oeste=0;
            }
            // Esperar a que todos los hilos terminen
            for (Thread hilo : hilos) {
                if (hilo != null) {
                    try {
                        hilo.join();
                    } catch (InterruptedException e) {
                        // Manejar la excepción si es necesario
                        e.printStackTrace();
                    }
                }
            }
        // Tomar el tiempo de finalización
        long tiempoFin = System.nanoTime();

        // Calcular el tiempo transcurrido
        long tiempoEjecucion = tiempoFin - tiempoInicio;

        // Mostrar el tiempo de ejecución en milisegundos
        System.out.println("Tiempo de ejecución: " + tiempoEjecucion / 1_000_000 + " ms");

        } catch (IOException e) {
            System.out.println("Fallo main");
            e.printStackTrace();
        }
    }
}