#include <unistd.h>

void wolfram_update(cello_state *state, unsigned int x, unsigned int y)
{
    // TODO Change me
    const unsigned char rule = 73;
    
    unsigned char left = cello_get(state, x-1, y);
    unsigned char center = cello_get(state, x, y);
    unsigned char right = cello_get(state, x+1, y);
    
    // Compue a key witch tells us which bit of the rule we should set
    // as output. Refer to Wolfram's automata rule scheme.
    unsigned char in = right + (center << 1) + (left << 2);
    unsigned char out = (rule & (1 << in)) >> in;
    
    cello_set(state, x, y, out);
}

void wolfram_update_2d(cello_state *state, unsigned int x, unsigned int y)
{
    // TODO Change me
    const unsigned int rule = 453;
    
    // Refer to the encoding scheme described on page 173 
    // of A New Kind Of Science by Steven Wolfram
    unsigned int n_neighbors = cello_count_neighbors(state, x ,y);
    unsigned char center_cell = cello_get(state, x, y);
    unsigned char out = 0;
    
    if(n_neighbors == 0) {
        if(center_cell == 0) {
            out = rule & 1;
        } else {
            out = (rule >> 1) & 1;
        }
    } else {
        out = (rule >> (n_neighbors + 1)) & 1;
    }
    
    cello_set(state, x, y, out);
}

// Generalize the rule system
void wolfram(unsigned int width, unsigned int height)
{
    cello_state *cello;
    
    if(height == 1) {
        cello = cello_init(width, height, wolfram_update);
        // Set the center cell
        cello_set(cello, cello->width/2, 0, 1);
    } else if(height > 1) {
        cello = cello_init(width, height, wolfram_update_2d);
        // Set the center cell
        cello_set(cello, cello->width/2, cello->height/2, 1);
    } else {
        exit(1);
    }
    
    cello_swap(cello);
    
    for(int i=0; i<22; ++i) {
        cello_print(cello);
        cello_update(cello);
        usleep(100000); // .1s
    }
    
    cello_destroy(cello);
}
