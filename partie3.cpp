#define NB_CHAISE 5
#define TEMPS_RASE 10

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sys/wait.h>
#include <thread>
#include <mutex>
#include <cstring>

#include "sema.h"

using namespace std;
//g++ -std=c++11 -pthread -o index.exe index.cpp

CSemaphore attente(1);
CSemaphore clients(0);
CSemaphore senbarbier(1);

int place = NB_CHAISE;
int idClient = 1;

void barbier();
void client(int);

int main(int argc, char const *argv[]) //argc => Nb d'argument dans ./partie2.exe args1 args2 = 3 (./partie2.exe; args1; args2)  / argv => contient les valeurs des args aux index de argc
{
    //std :: thread first ( lecteur ); // Cr ́eation du premier thread , qui ex ́ecute ping
    //std :: thread second ( redacteur ); // Cr ́eation du deuxi`eme thread , qui ex ́ecute pong
    srand(time(NULL));
    cout << "Initialisation des Threads" << endl;

    thread tBarbier(barbier); 
    tBarbier.detach();

    while(true) {

        int random = rand()%2;

        if (random == 1) {
            thread tClient(client, idClient);
            tClient.detach(); 
            idClient++;
        }
        sleep(2);
    }

    return EXIT_SUCCESS;
}

void barbier (void) {
    while (true) {

        clients.wait();
        attente.wait();
        
        place++;
        attente.notify();

        cout << "Le Barbier travaille !" << endl;
        sleep(TEMPS_RASE);
        cout << "Le Barbier a finit de travailler !" << endl;
        senbarbier.notify();
    }
}

void client (int id) {
    attente.wait();
    if (place > 0) {

        place--;
        attente.notify();

        cout << "Le Client "<< id << " entre en salle d'attente, il reste " << place << " places !" << endl;

        clients.notify();
        senbarbier.wait();
        cout << "Le Client "<< id << " se fait raser." << endl;
        sleep(TEMPS_RASE);
        cout << "Le Client "<< id << " a finit de se faire raser..." << endl;
    } else {
        cout << "Le Client "<< id << " rentre chez lui pour cause de manque de place ..." << endl;
        attente.notify();
    }
}