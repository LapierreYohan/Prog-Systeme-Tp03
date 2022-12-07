#define TOT 200

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sys/wait.h>
#include <thread>
#include <mutex>

#include "sema.h"

using namespace std;
//g++ -std=c++11 -pthread -o index.exe index.cpp

CSemaphore sp(0);
CSemaphore sf(1);

ofstream sortie("match.txt", ios::out);

void ping ();
void pong ();

int main(int argc, char const *argv[])
{
    std :: thread first ( ping ); // Cr ́eation du premier thread , qui ex ́ecute ping
    std :: thread second ( pong ); // Cr ́eation du deuxi`eme thread , qui ex ́ecute pong

    std :: cout << " main , foo et bar s ’ executent en parallelle ...\n " ;

    // synchronize threads :
    first.join(); // main attend que le premier thread ( ping ) se termine
    second.join(); // main attend que le second thread ( pong ) se termine

    std :: cout << " ping et pong sont termines .\n " ;

    //sleep(3);
    sortie.close();

    return EXIT_SUCCESS;
}

void ping ()
{
    int i = 0;
    while(i<TOT)
    {    
        sf.wait();
        sortie << "Ping-";
        i++;
        sortie.flush();
        sp.notify();

    }
    std :: cout << " fin ping\n " ;
}
void pong ()
{
    int i = 0;
    while(i<TOT)
    {
        sp.wait();
        sortie << "Pong\n";
        sortie.flush();
        i++;
        sf.notify();
    }
    std :: cout << " fin pong\n " ;
}