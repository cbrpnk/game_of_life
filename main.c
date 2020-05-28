#include <unistd.h>

#include "gol.h"


int main(int argc, char **argv)
{
    if(argc == 3) {
        gol(atoi(argv[1]), atoi(argv[2]));
    } else {
        gol(20, 20);
    }
    
    // TODO Wolfram automata
    
    return 0;
}
