#define TEMPS_ECRITURE 1

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sys/wait.h>
#include <thread>
#include <mutex>

#include "sema.h"

using namespace std;
//g++ -std=c++11 -pthread -o index.exe index.cpp

//CSemaphore sp(0);
//CSemaphore sf(1);

void lecteur ();
void redacteur ();

ofstream sortie("match.txt", ios::out);

char chaine[6][10] = {"Bonjour","Hola","Que tal","Hello","Hi","Halo"};

int main(int argc, char const *argv[]) //argc => Nb d'argument dans ./partie2.exe args1 args2 = 3 (./partie2.exe; args1; args2)  / argv => contient les valeurs des args aux index de argc
{
    //std :: thread first ( lecteur ); // Cr ́eation du premier thread , qui ex ́ecute ping
    //std :: thread second ( redacteur ); // Cr ́eation du deuxi`eme thread , qui ex ́ecute pong

    std :: cout << " Initialisation des Threads" ;

    int tot;

    if (argc == 2){
        tot = atoi(argv[1]);
    }

    srand(time(NULL));

    for (size_t i = 0; i < tot; i++)
    {
        thread t;
        int random = rand() % 5;
        if (random == 3 || random == 4){
            t = thread(redacteur);
            t.detach();
        } else {
            t = thread(lecteur);
            t.detach();
        }
        sleep(1);
    }

    return EXIT_SUCCESS;
}

void lecteur ()
{
    ifstream entrer("match.txt", ios::in);
    cout << "Début de lecture : " << endl;
    while(entrer.eof()){
        char* temp;
        entrer >> temp;
        cout << temp << endl; 
    }
    entrer.close();
    cout << "Fin de lecture : " << endl;
}
void redacteur ()
{
    cout << "Début d'ecriture : " << endl;
    srand(time(NULL));
    int rd = rand() % 6;
    char* txt = chaine[rd];
    cout << "J'ai choisi le mot : " << txt << endl;
    for(int i = 0; i < sizeof(txt); i++){
        sortie << txt[i];
        sleep(TEMPS_ECRITURE);
    }
    sortie << "\n";
    sortie.flush();
    cout << "Fin de l'ecriture: " << endl;
}