#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define BOARD_WIDTH 80
#define BOARD_HEIGHT 80

void init_random(unsigned char *board)
{
    int size = BOARD_WIDTH * BOARD_HEIGHT;
    for(int i=0; i<size; ++i) {
        board[i] = (int)rand() % 2;
    }
}

void init_glider(unsigned char *board)
{
    board[2*BOARD_WIDTH+5] = 1;
    board[2*BOARD_WIDTH+6] = 1;
    board[2*BOARD_WIDTH+7] = 1;
    board[1*BOARD_WIDTH+7] = 1;
    board[0*BOARD_WIDTH+6] = 1;
}

int get_val(unsigned char *board, int x, int y)
{
    // Wrap coordinates around
    x<0 ? (x += BOARD_WIDTH)  : (x %= BOARD_WIDTH);
    y<0 ? (y += BOARD_HEIGHT) : (y %= BOARD_HEIGHT);
    return board[y*BOARD_WIDTH+x];
}

void set_val(unsigned char *board, int x, int y, int v)
{
    board[y*BOARD_WIDTH+x] = v;
}

void print(unsigned char *board)
{
    printf("\e[1;1H\e[2J");
    // Clear screen
    for(int y=0; y<BOARD_HEIGHT; ++y) {
        for(int x=0; x<BOARD_WIDTH; ++x) {
            char c = (get_val(board, x, y)) ? 'O' : '.';
            printf("%c ", c);
        }
        printf("\n");
    }
}

int count_neighbors(unsigned char *board, int x, int y)
{
    int count = 0;
    count += get_val(board, x-1, y-1); // Top left
    count += get_val(board, x  , y-1); // Top
    count += get_val(board, x+1, y-1); // Top rigth
    count += get_val(board, x-1, y);   // Left
    count += get_val(board, x+1, y);   // Right
    count += get_val(board, x-1, y+1); // Top left
    count += get_val(board, x  , y+1); // Top
    count += get_val(board, x+1, y+1); // Top rigth
    return count;
}

void update(unsigned char *current, unsigned char *next)
{
    for(int y=0; y<BOARD_HEIGHT; ++y) {
        for(int x=0; x<BOARD_WIDTH; ++x) {
            int n_neighbors = count_neighbors(current, x, y);
            if(get_val(current, x, y)) {
                // Alive
                if(n_neighbors < 2 || n_neighbors > 3) {
                    set_val(next, x, y, 0);
                } else {
                    set_val(next, x, y, 1);
                }
            } else if(n_neighbors == 3) {
                set_val(next, x, y, 1);
            } else {
                set_val(next, x, y, 0);
            }
        }
    }
}

int main()
{
    unsigned char board_a[BOARD_WIDTH * BOARD_HEIGHT] = {0};
    unsigned char board_b[BOARD_WIDTH * BOARD_HEIGHT] = {0};
    unsigned char *current = board_a;
    unsigned char *next = board_b;
    
    //init_glider(current);
    init_random(current);
    
    unsigned char *tmp;
    while(1) {
        print(current);
        update(current, next);
        
        // Switch next with current
        tmp = current;
        current = next;
        next = tmp;
        
        usleep(100000); // microseconds
    }
    return 0;
}
