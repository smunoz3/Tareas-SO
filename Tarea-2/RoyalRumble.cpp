#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <random>

using namespace std;


class Jugador {
public:
    Jugador()
        : vida(100), ataque(rand() % 11 + 30), defensa(rand() % 16 + 10) {
            evasion = 60 - defensa;
        }


    int getVida() const { return vida; }
    int getAtaque() const { return ataque; }
    int getDefensa() const { return defensa; }
    int getEvasion() const { return evasion; }


    void setVida(int nueva_vida) { vida = nueva_vida; }

private:
    int vida;
    int ataque;
    int defensa;
    int evasion;
};

Jugador jugadores[4];

/*****
* void mostarDatos
******
* Se muestran por consola los datos de cada jugador
******/
void mostarDatos(){
    cout<<"Jugador 1\n Vida: "<<jugadores[0].getVida()<<" Ataque: "<<jugadores[0].getAtaque()<<" Defensa: "<<jugadores[0].getDefensa()<<" Evasion: "<<jugadores[0].getEvasion()<<endl;
    cout<<"Jugador 2\n Vida: "<<jugadores[1].getVida()<<" Ataque: "<<jugadores[1].getAtaque()<<" Defensa: "<<jugadores[1].getDefensa()<<" Evasion: "<<jugadores[1].getEvasion()<<endl;
    cout<<"Jugador 3\n Vida: "<<jugadores[2].getVida()<<" Ataque: "<<jugadores[2].getAtaque()<<" Defensa: "<<jugadores[2].getDefensa()<<" Evasion: "<<jugadores[2].getEvasion()<<endl;
    cout<<"Jugador 4\n Vida: "<<jugadores[3].getVida()<<" Ataque: "<<jugadores[3].getAtaque()<<" Defensa: "<<jugadores[3].getDefensa()<<" Evasion: "<<jugadores[3].getEvasion()<<endl;
    cout << "\n";
 
}

/*****
* void bots
******
* Aca se hacen las acciones de cada jugador
* donde numeroPlayer = 0, es el jugador por consola.
******
* Input:
*   int numeroPlayer: numero del jugador, para saber cual proceso es cual jugador
*   int pipe_PH[2]: Es el pipe de padre a hijo
*   int pipe_HP[2]: Es el pipe de hijo a padre
******/
void bots(int numeroPlayer, int pipe_PH[2], int pipe_HP[2]){
    close(pipe_PH[1]);//cerrar pipe escritura padre a hijo
    close(pipe_HP[0]);//cerrar pipe escritura hijo a padre
    srand(time(NULL) ^ getpid());
    int vidaPlayer;
    int vidaBot1, vidaBot2, vidaBot3;
    int danioRecibido, evasion, vida;
    int jugadorAtacado;
    bool vivo;
    vivo = true;
    int arrayVida[4];
    bool seleccionValida;
    int vecesAtacado;
    int ganador;
    while(vivo){
        read(pipe_PH[0], &vidaPlayer, sizeof(vidaPlayer));
        arrayVida[0]=vidaPlayer;
        //leer vida de los bots
        read(pipe_PH[0], &vidaBot1, sizeof(vidaBot1));
        arrayVida[1]=vidaBot1;
        read(pipe_PH[0], &vidaBot2, sizeof(vidaBot2));
        arrayVida[2]=vidaBot2;
        read(pipe_PH[0], &vidaBot3, sizeof(vidaBot3));
        arrayVida[3]=vidaBot3;

        
        if (numeroPlayer==0){
            seleccionValida = false;
            while (!seleccionValida){
                cout << "Elije a quien atacar"<< endl;
                if(vidaBot1){
                    cout<<"Jugador 2 Vida: " << vidaBot1 << endl;
                }
                if(vidaBot2){
                    cout<<"Jugador 3 Vida: " << vidaBot2 << endl;
                }
                if(vidaBot3){
                    cout<<"Jugador 4 Vida: " << vidaBot3 << endl;
                }
                
                cin >> jugadorAtacado;
                jugadorAtacado -= 1;
                cout << endl;
                if (jugadorAtacado >= 0 && jugadorAtacado <= 3) {
                    if (jugadorAtacado == 0){
                        cout << "No puedes atacarte a ti mismo!" << endl;
                    } else if (jugadorAtacado == 1 && !vidaBot1) {
                        cout << "El jugador 2 está muerto, elige otro jugador." << endl;
                    } else if (jugadorAtacado == 2 && !vidaBot2) {
                        cout << "El jugador 3 está muerto, elige otro jugador." << endl;
                    } else if (jugadorAtacado == 3 && !vidaBot3) {
                        cout << "El jugador 4 está muerto, elige otro jugador." << endl;
                    } else {
                        seleccionValida = true;
                    }
                } else {
                    cout << "Número de jugador inválido. Inténtelo de nuevo." << endl;
                }
            }
        } else{
            jugadorAtacado = rand()%4;
            while((arrayVida[jugadorAtacado]==0)||(jugadorAtacado==numeroPlayer)){
                jugadorAtacado= rand()%4;
            }
        }
        cout<<"Jugador: "<<numeroPlayer + 1<< " Ataca a: " << jugadorAtacado + 1 <<endl;
        write(pipe_HP[1], &jugadorAtacado, sizeof(jugadorAtacado));
        
        //recibir cuantas veces se le ataca
        read(pipe_PH[0], &vecesAtacado, sizeof(vecesAtacado));
        
        for (int i = 0; i< vecesAtacado; i++){
            read(pipe_PH[0], &danioRecibido, sizeof(danioRecibido));
            read(pipe_PH[0], &vida, sizeof(vida));

            evasion = jugadores[numeroPlayer].getEvasion();
            int probabilidadGolpe = (rand() % 100) + 1;
            if (probabilidadGolpe > evasion){
                cout << "No pudo evadir el jugador " << numeroPlayer + 1 << endl;
                cout << "El jugador " << numeroPlayer + 1 << " recibio un daño de: " << danioRecibido << endl;
                cout << endl;
                if ((vida - danioRecibido) <= 0){
                    vida = 0;
                } else {
                    vida -= danioRecibido;
                }
            } else {
                cout << "Esquivo el jugador " << numeroPlayer + 1 <<endl;
                cout << endl;
            }
            //mandar la vida actualizada
            write(pipe_HP[1], &vida, sizeof(vida));
        }
        read(pipe_PH[0], &vivo, sizeof(vivo));
    }
    if (numeroPlayer == 0){
        cout << "Te has muerto :c" << endl;
    } else {
        cout<<"El bot: " << numeroPlayer + 1 << " ha muerto :c" << endl;
    }

    read(pipe_PH[0], &ganador, sizeof(ganador));
    
    //cerrar los pipes
    close(pipe_PH[0]);
    close(pipe_HP[1]);
    
}

/*****
* int main
******
* Aca se hace todo  el desarrollo del juego
* Es el proceso padre
* Se solicitan los datos necesarios a cada hijo a traves de los pipes
******/
int main(){
    int pipe_PH[4][2], pipe_HP[4][2];
    for(int i = 0; i<4; i++){
        pipe(pipe_PH[i]);
        pipe(pipe_HP[i]);
    }
    pid_t pid;

    for (int i = 0; i<4; i++){
        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Proceso hijo
                bots(i, pipe_PH[i], pipe_HP[i]);
                exit(EXIT_SUCCESS);
        }
    }

    int ronda=1;
    int vidaPlayer;
    int danio;
    int jugadorAtacado[4];
    
    bool verdad=true;
    bool falso=false;
    bool bandera[] ={true,true,true,true}; 
    int vida1, vida2, vida3;
    int vida, ataque;
    int player;
    int numMuertos;
    int ganador;

    cout << "Estas seran las estadisticas iniciales" << endl;
    mostarDatos();
    cout << endl;
    cout << endl;

    //Cerrar pipes que no se ocupan
    for (int i = 0; i<4; i++){
        close(pipe_PH[i][0]);
        close(pipe_HP[i][1]);
    }

    if (pid!=0) {
        while(1){
            //Resetear jugadores atacados
            for (int i = 0; i<4; i++){
                jugadorAtacado[i] = -1;
            }
            //Ver cuantos estan muertos
            numMuertos = 0;
            for (int i = 0; i < 4; i++) {
                if (jugadores[i].getVida() == 0) {
                    numMuertos++;
                }
            }

            if (numMuertos >= 3){
                //Condicion todos muertos
                if (numMuertos == 4){
                    ganador = rand() % 3;
                    cout << "El player " << ganador + 1 << "En realidad no estaba muerto :O" << endl;
                } else {
                    for (int i = 0; i < 4; i++) {
                        if (jugadores[i].getVida() > 0) {
                            //Tirar al ganador que este muerto??
                            ganador = i;
                            break;
                        }
                    }
                    break;
                }
            }

            //Inicio ronda
            cout<<"Ronda: "<<ronda<<"\n";
            mostarDatos();
            vidaPlayer = jugadores[0].getVida();
            for (int i=0; i<4; i++){
                if(bandera[i]){//No entra si el jugador esta muerto
                    //Mandar la vida de los players
                    write(pipe_PH[i][1], &vidaPlayer, sizeof(vidaPlayer));//sincronisador
                    vida1 = jugadores[1].getVida();
                    vida2 = jugadores[2].getVida();
                    vida3 = jugadores[3].getVida();

                    write(pipe_PH[i][1], &vida1, sizeof(vida1));
                    write(pipe_PH[i][1], &vida2, sizeof(vida2));
                    write(pipe_PH[i][1], &vida3, sizeof(vida3));
                
                    //Recibir el jugador atacado
                    
                    read(pipe_HP[i][0], &player, sizeof(player));
                    jugadorAtacado[i] = player;
                }
            }

            //Tenemos guardados los jugadores atacados por cada player en jugadorAtacado[4]
            int vecesAtacadoPlayer[]={0,0,0,0};
            for (int i = 0; i<4; i++){
                if (jugadorAtacado[i] == 0){
                    vecesAtacadoPlayer[0] +=1;
                } else if (jugadorAtacado[i] == 1){
                    vecesAtacadoPlayer[1] +=1;
                } else if (jugadorAtacado[i] == 2){
                    vecesAtacadoPlayer[2] +=1;
                } else if (jugadorAtacado[i] == 3){
                    vecesAtacadoPlayer[3] +=1;
                }
            }
            
            //Mandar cuantas veces se ataco a cada player
            for(int i=0;i<4;i++){
                if(jugadores[i].getVida()>0){
                    write(pipe_PH[i][1], &vecesAtacadoPlayer[i], sizeof(vecesAtacadoPlayer[i]));
                }
            }

            //Realizar los ataques
            for (int i = 0; i<4; i++){
                if(bandera[i]){//No entra si el jugador esta muerto
                    //Daño = ataque - defensa
                    ataque = jugadores[i].getAtaque();
                    //mandamos daño recibido y vida
                    if (jugadorAtacado[i] == 0){//Ataque al player 1
                        danio = ataque - jugadores[0].getDefensa();
                        vida = jugadores[0].getVida();
                        write(pipe_PH[0][1], &danio, sizeof(danio));
                        write(pipe_PH[0][1], &vida, sizeof(vida));
                        //recibimos nueva vida del player
                        read(pipe_HP[0][0], &vida, sizeof(vida));
                        jugadores[0].setVida(vida);
                        
                    } else if (jugadorAtacado[i] == 1){//Ataque al player 2
                        danio = ataque - jugadores[1].getDefensa();
                        vida = jugadores[1].getVida();
                        write(pipe_PH[1][1], &danio, sizeof(danio));
                        write(pipe_PH[1][1], &vida, sizeof(vida));
                        //recibimos nueva vida del player
                        read(pipe_HP[1][0], &vida, sizeof(vida));
                        jugadores[1].setVida(vida);
                        
                    } else if (jugadorAtacado[i] == 2){//Ataque al player 3
                        danio = ataque - jugadores[2].getDefensa();
                        vida = jugadores[2].getVida();
                        write(pipe_PH[2][1], &danio, sizeof(danio));
                        write(pipe_PH[2][1], &vida, sizeof(vida));
                        //recibimos nueva vida del player
                        read(pipe_HP[2][0], &vida, sizeof(vida));
                        jugadores[2].setVida(vida);
                        
                    } else if (jugadorAtacado[i] == 3){//Ataque al player 4
                        danio = ataque - jugadores[3].getDefensa();
                        vida = jugadores[3].getVida();
                        write(pipe_PH[3][1], &danio, sizeof(danio));
                        write(pipe_PH[3][1], &vida, sizeof(vida));
                        //recibimos nueva vida del player
                        read(pipe_HP[3][0], &vida, sizeof(vida));
                        jugadores[3].setVida(vida);
                        
                    }
                }
            }

            for(int i =0;i<4;i++){
                if((jugadores[i].getVida()>0)&& bandera[i]){
                    write(pipe_PH[i][1],&verdad,sizeof(verdad));
                }
                else{
                    if (bandera[i] == true){
                        write(pipe_PH[i][1],&falso,sizeof(falso)); 
                        bandera[i]=false;
                    }
                }
            }
            ronda++;
        }
    }
    for(int i = 0; i<4; i++){
        write(pipe_PH[i][1], &ganador, sizeof(ganador));
    }

    //Cerrar pipes finales
    for (int i = 0; i<4; i++){
        close(pipe_PH[i][1]);
        close(pipe_HP[i][0]);
    }
    
    cout << "ANUNCIADOR: EL GANADOR DEL ROYALRUMBLE 2024 ES: " << ganador + 1 << " WOOOOOOOOOOOOOOOOOOOOO"<<endl;
    return 0;
}