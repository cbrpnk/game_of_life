/*
 * Cello
 * A cellular automata library in one header file
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct cello_state cello_state;

struct cello_state {
    unsigned int width;
    unsigned int height;
    // Pointer to array of width*height bytes where each byte represents a 
    // cell. This a flat contiguous array of (height) rows.
    unsigned char *board;
    // Since the state of a cell can be encoded in 1 bit, we use the 2 LSBs
    // of each byte of the board to encode the current board state and
    // the next board state. The masks are exchanged each turn so that the
    // next state becomes the current and vis versa.
    //unsigned char board_current_mask = 1;
    //unsigned char board_next_mask = 2;
    unsigned char current_mask;
    unsigned char next_mask;
    // Called for every cell
    void (*update_cell_fn)(cello_state *state, unsigned int x, unsigned int y);
};

cello_state *cello_init(unsigned int width,
                        unsigned int height,
                        void (*update_cell_fn)(
                                cello_state *state,
                                unsigned int x,
                                unsigned int y
                              )
                        )
{
    cello_state *state = malloc(sizeof(cello_state));
    
    state->width = width;
    state->height = height;
    state->board = malloc(width * height * sizeof(unsigned char));
    state->current_mask = 1;
    state->next_mask    = 2;
    state->update_cell_fn = update_cell_fn;
    
    srand(time(NULL));
    
    return state;
}

void cello_destroy(cello_state *state)
{
    free(state->board);
    free(state);
}

void cello_randomize(cello_state *state)
{
    int size = state->width * state->height;
    for(int i=0; i<size; ++i) {
        state->board[i] = ((int)rand() % 2) << (state->current_mask-1);
    }
}

/*
 * Get the current sate of a cell
 */
int cello_get(cello_state *state, int x, int y)
{
    // Wrap coordinates around
    x<0 ? (x += state->width)  : (x %= state->width);
    y<0 ? (y += state->height) : (y %= state->height);
    return (state->board[y*state->width+x] & 
            state->current_mask) >> state->current_mask-1;
}

/*
 * Set the next state of a cell
 */
void cello_set(cello_state *state, int x, int y, unsigned char v)
{
    unsigned char bit = v << (state->next_mask-1);
    state->board[y*state->width+x] = (state->board[y*state->width+x] &
                              ~state->next_mask) + bit;
}

void cello_print(cello_state *state)
{
    for(int y=0; y<state->height; ++y) {
        for(int x=0; x<state->width; ++x) {
            char c = (cello_get(state, x, y)) ? '@' : '.';
            printf("%c ", c);
        }
        printf("\n");
    }
}

int cello_count_neighbors(cello_state *state, int x, int y)
{
    int count = 0;
    count += cello_get(state, x-1, y);   // Left
    count += cello_get(state, x+1, y);   // Right
    
    // Is this a 2d cellular automata
    if(state->height > 1) {
        count += cello_get(state, x-1, y-1); // Top left
        count += cello_get(state, x  , y-1); // Top
        count += cello_get(state, x+1, y-1); // Top rigth
        count += cello_get(state, x-1, y+1); // Bottom left
        count += cello_get(state, x  , y+1); // Bottom
        count += cello_get(state, x+1, y+1); // Bottom rigth
    }
    return count;
}

/*
 * Exchange the current and next board mask
 */
void cello_swap(cello_state *state)
{
    unsigned char tmp = state->current_mask;
    state->current_mask = state->next_mask;
    state->next_mask = tmp;
}

void cello_update(cello_state *state)
{
    for(int y=0; y<state->height; ++y) {
        for(int x=0; x<state->width; ++x) {
            state->update_cell_fn(state, x, y);
        }
    }
    cello_swap(state);
}

