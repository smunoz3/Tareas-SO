import java.util.ArrayList;
import java.util.List;

class MiRunnable implements Runnable {
    public char[][] matriz_laberinto;
    public int[] posicion;
    public int dimension;
    public char camino_anterior;

    //Constructor
    public MiRunnable(char[][] matriz_laberinto,int[] posicion, int dimension, char camino_anterior){
        this.matriz_laberinto = matriz_laberinto;
        this.posicion = posicion.clone();
        this.dimension = dimension;
        this.camino_anterior = camino_anterior; 
    } 

    public void run()
    {
        try {
            int norte=0;
            int sur =0;
            int este=0;
            int oeste=0;
            char camino ='L';
            Funciones funciones = new Funciones();
            List<Thread> hilos = new ArrayList<>();
            matriz_laberinto[posicion[0]][posicion[1]] = '0';

            while(true){
                //Buscar caminos
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
            //Esperar a que todos los hilos terminen
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
        }
        catch (Exception e) {
            System.out.println("Fallo hebra");
            e.printStackTrace();
        }
    }
}