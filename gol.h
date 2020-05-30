#include <unistd.h>
#include "cello.h"

void update_cell(cello_state *state, unsigned int x, unsigned int y)
{
    int n_neighbors = cello_count_neighbors(state, x, y);
    if(cello_get(state, x, y)) {
        // Alive
        if(n_neighbors < 2 || n_neighbors > 3) {
            cello_set(state, x, y, 0);
        } else {
            cello_set(state, x, y, 1);
        }
    } else if(n_neighbors == 3) {
        cello_set(state, x, y, 1);
    } else {
        cello_set(state, x, y, 0);
    }
}

void gol(unsigned int width, unsigned int height) {
    cello_state *cello = cello_init(width, height, update_cell);
    cello_randomize(cello);
    
    while(1) {
        // Clear screen
        printf("\e[1;1H\e[2J");
        cello_print(cello);
        cello_update(cello);
        usleep(100000); // .1 second
    }
    
    cello_destroy(cello);
}
