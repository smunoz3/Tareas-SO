public class Funciones {
    //Ve la posicion y la direccion y revisa lo que hay en la matriz
    public int see(int x, int y,char direccion, char[][] laberinto, int dimension){
        int retorno;
        if(direccion == 'O'){
            if((x-1) >= 0){
                if((laberinto[x-1][y])=='S'){
                    return 10;  //llega a salida
                }
                else{
                    retorno =  (int) laberinto[x-1][y];
                    retorno = retorno -48;
                    return (retorno);
                    
                }
            }
            return 0;   // se sale de rango 
        }
        else if(direccion == 'E'){
            if((x+1) < dimension){
                if((laberinto[x+1][y])=='S'){
                    return 10;  //llega a salida
                }
                else{
                    retorno =  (int) laberinto[x+1][y];
                    retorno = retorno -48;
                    return (retorno);
                }
            }
            return 0;   // se sale de rango 
        }
        else if(direccion == 'S'){
            if((y+1) <dimension){
                if((laberinto[x][y+1])=='S'){
                    return 10;  //llega a salida
                }
                else{
                    retorno =  (int) laberinto[x][y+1];
                    retorno = retorno -48;
                    return (retorno);
                }
            }
            return 0;   // se sale de rango 
        }
        else if(direccion == 'N'){
            if((y-1) >= 0){
                if((laberinto[x][y-1])=='S'){
                    return 10;  //llega a salida
                }
                else{
                    retorno =  (int) laberinto[x][y-1];
                    retorno = retorno -48;
                    return (retorno);
                }
            }
            return 0;   // se sale de rango 
        }
        else{
            return 0;
        }
    }
    //Cambia la posicion actual, con respecto a la direccion que se quiere avanzar
    public int[] avanzar(int[] posicion, char direccion){
        if(direccion == 'N'){
            posicion[1] -= 1;
            return posicion;
        }
        else if(direccion == 'S'){
            posicion[1] += 1;
            return posicion; 
        }
        else if(direccion == 'E'){
            posicion[0] += 1;
            return posicion;
        }
        else if(direccion == 'O'){
            posicion[0] -= 1;
            return posicion;
        }
        else{
            return posicion;
        }
    }
}

