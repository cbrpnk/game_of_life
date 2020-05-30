#include <unistd.h>

#include "gol.h"
#include "wolfram.h"

int main(int argc, char **argv)
{
    /*
    if(argc == 3) {
        gol(atoi(argv[1]), atoi(argv[2]));
    } else {
       gol(20, 20);
    }
    */
    
    if(argc == 3) {
        wolfram(atoi(argv[1]), atoi(argv[2]));
    } else {
       wolfram(40, 40);
    }
    
    
    return 0;
}
