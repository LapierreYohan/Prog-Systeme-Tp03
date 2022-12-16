#define TEMPS_ECRITURE 0.5

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

int nbLecteur = 0;

CSemaphore nbActive(1); //mutex pour lecteurActive
CSemaphore partage(1); //mutex pour la zone partage

void lecteur ();
void redacteur ();

void demanderLecture() {
    nbActive.wait();
    if (nbLecteur == 0)
        partage.wait();
    nbLecteur++;
    nbActive.notify();
}

void signalerFinLecture() {
    nbActive.wait();
    nbLecteur--;
    if (nbLecteur == 0)
        partage.notify();
    nbActive.notify();
}

void demanderEcriture() {
    partage.wait();
}

void signalerFinEcriture() {
    partage.notify();
}

ofstream s("match.txt", ios::out);

char chaine[6][10] = {"Bonjour","Hola","Que tal","Hello","Hi","Halo"};

int main(int argc, char const *argv[]) //argc => Nb d'argument dans ./partie2.exe args1 args2 = 3 (./partie2.exe; args1; args2)  / argv => contient les valeurs des args aux index de argc
{
    //std :: thread first ( lecteur ); // Cr ́eation du premier thread , qui ex ́ecute ping
    //std :: thread second ( redacteur ); // Cr ́eation du deuxi`eme thread , qui ex ́ecute pong

    std :: cout << "Initialisation des Threads" << endl;

    int tot = 5;

    if (argc == 2){
        tot = atoi(argv[1]);
    }

    srand(time(NULL));

    thread t;
    int random = 0;
    for (int i = 0; i < tot; i++)
    {
        random = rand() % 5;
        if (random == 4 || random == 3){
            t = thread(redacteur);
        } else {
            t = thread(lecteur);
        }
        t.detach();
        sleep(1);
    }

    return EXIT_SUCCESS;
}

void lecteur ()
{
    demanderLecture();
    cout << "Début de lecture : ";
    ifstream entrer("match.txt", ios::in);
    char temp[100];

    while(!entrer.eof()){
        entrer >> temp;
        cout << temp <<endl; 
    }
    cout << endl;
    entrer.close(); 
    signalerFinLecture();
}
void redacteur ()
{
    demanderEcriture();
    cout << "Début d'ecriture : ";
    srand(time(NULL));
    int rd = rand() % 6;
    char* txt = chaine[rd];
    cout << "J'ai choisi le mot : " << txt << endl;
    ofstream sortie("match.txt", ios::app);
    for(int i = 0; i < strlen(chaine[rd]); i++){
        sortie << txt[i];
        sortie.flush();
        sleep(TEMPS_ECRITURE);
    }
    sortie << "\n";
    sortie.close();

    signalerFinEcriture();
}