import java.util.concurrent.RecursiveTask;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class BaquedanoForks {
    static class HijosTask extends RecursiveTask<Void> {
        public char[][] matriz_laberinto;
        public int[] posicion;
        public int dimension;
        public char camino_anterior;

        //Constructor
        public HijosTask(char[][] matriz_laberinto,int[] posicion, int dimension, char camino_anterior) {
            this.matriz_laberinto = matriz_laberinto;
            this.posicion = posicion.clone();
            this.dimension = dimension;
            this.camino_anterior = camino_anterior; 
        }

        @Override
        protected Void compute() {
            List<HijosTask> tasks = new ArrayList<>();
            try {
                int norte=0;
                int sur =0;
                int este=0;
                int oeste=0;
                char camino ='L';
                Funciones funciones = new Funciones();
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
                        System.out.println("Se encontro una salida en la task: " + Thread.currentThread().getId() + " Con posicion x: "+ posicion[0] + " y: " + posicion[1]);
                        break;
                    }
                    //Camino sin salida
                    else if((norte+sur+este+oeste)==0){
                        System.out.println("No se encontro una salida en la task: " + Thread.currentThread().getId() + " Con posicion x: "+ posicion[0] + " y: " + posicion[1]);
                        break;
                    }
                    //Bifurcacion
                    else if((norte+sur+este+oeste)<=4){
                        if(este==1){
                            camino_anterior = 'O';
                            int[] posicionCopia = posicion.clone();
                            HijosTask p1 = new HijosTask(matriz_laberinto,funciones.avanzar(posicionCopia, 'E'),dimension,camino_anterior);
                            p1.fork();
                            tasks.add(p1);
                        }
                        if(oeste==1){
                            camino_anterior = 'E';
                            int[] posicionCopia = posicion.clone();
                            HijosTask p2 = new HijosTask(matriz_laberinto,funciones.avanzar(posicionCopia, 'O'),dimension,camino_anterior);
                            p2.fork();
                            tasks.add(p2);
                        }
                        if(norte==1){
                            camino_anterior = 'S';
                            int[] posicionCopia = posicion.clone();
                            HijosTask p3 = new HijosTask(matriz_laberinto,funciones.avanzar(posicionCopia, 'N'),dimension,camino_anterior);
                            p3.fork();
                            tasks.add(p3);
                        }
                        if(sur==1){
                            camino_anterior = 'N';
                            int[] posicionCopia = posicion.clone();
                            HijosTask p4 = new HijosTask(matriz_laberinto,funciones.avanzar(posicionCopia, 'S'),dimension,camino_anterior);
                            p4.fork();
                            tasks.add(p4);
                        }
                        break;
                    }
                    //Resetear variables
                    norte=0;
                    sur=0;
                    este=0;
                    oeste=0;
                }
                // Esperar a que todas las tasks terminen
                for (HijosTask task : tasks) {
                    if (task != null) {
                        task.join();
                    }
                }
                return null;
            }
            catch (Exception e) {
                System.out.println("Fallo hebra");
                e.printStackTrace();
                return null;
            }
        }
    }

    public static void main(String[] args) {
        long tiempoInicio = System.nanoTime();

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

        List<HijosTask> tasks = new ArrayList<>();

            
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
                    System.out.println("Se encontro una salida en la task: " + Thread.currentThread().getId() + " Con posicion x: "+ posicion[0] + " y: " + posicion[1]);
                    break;
                }
                //Camino sin salida
                else if((norte+sur+este+oeste)==0){
                    System.out.println("No se encontro una salida en la task: " + Thread.currentThread().getId() + " Con posicion x: "+ posicion[0] + " y: " + posicion[1]);
                    break;
                }
                //Bifurcacion
                else if((norte+sur+este+oeste)<=4){
                    if(este==1){
                        camino_anterior = 'O';
                        int[] posicionCopia = posicion.clone();
                        HijosTask p1 = new HijosTask(matriz_laberinto,funciones.avanzar(posicionCopia, 'E'),dimension,camino_anterior);
                        p1.fork();
                        tasks.add(p1);
                    }
                    if(oeste==1){
                        camino_anterior = 'E';
                        int[] posicionCopia = posicion.clone();
                        HijosTask p2 = new HijosTask(matriz_laberinto,funciones.avanzar(posicionCopia, 'O'),dimension,camino_anterior);
                        p2.fork();
                        tasks.add(p2);
                    }
                    if(norte==1){
                        camino_anterior = 'S';
                        int[] posicionCopia = posicion.clone();
                        HijosTask p3 = new HijosTask(matriz_laberinto,funciones.avanzar(posicionCopia, 'N'),dimension,camino_anterior);
                        p3.fork();
                        tasks.add(p3);
                    }
                    if(sur==1){
                        camino_anterior = 'N';
                        int[] posicionCopia = posicion.clone();
                        HijosTask p4 = new HijosTask(matriz_laberinto,funciones.avanzar(posicionCopia, 'S'),dimension,camino_anterior);
                        p4.fork();
                        tasks.add(p4);
                    }
                    break;
                }

                //Resetear variables
                norte=0;
                sur=0;
                este=0;
                oeste=0;
            }
            // Esperar a que todas las tasks terminen
            for (HijosTask task : tasks) {
                if (task != null) {
                    task.join();
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
