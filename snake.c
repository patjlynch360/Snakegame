#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

#define UP    1
#define DOWN  2
#define LEFT  3
#define RIGHT 4

struct snake{
    int xloc;
    int yloc;
    int head;
};


void updateSnake(struct snake[], int, int);

int main(){

    int trophyMax = 10;
    int trophyTime;
    int dir = RIGHT;
    int x, y, xMax, yMax, ch;
    int length = 5;
    int score = 0;

    // Used for generating trophy location
    srand(time(NULL));
    int randX, randY;
    int trophy = 'T';

    initscr();
        clear();
        x = y = 1;
        xMax = COLS-1;
        yMax = LINES-1;

        // Hide the cursor
        curs_set(0);
        //Creates the window
        WINDOW *win = newwin(yMax, xMax, 1,1);
        // grabs key inputs
        keypad(win,true);
        //turns of blocking
        nodelay(win, TRUE);
        //creates the pit
        box(win,0,0);
        // Draw score
        mvwprintw(win, 0, xMax - 30, "Score: %d", score);
        //creates the snake
        struct snake snakearr[(xMax*yMax)/2];
        for(int i=0;i<length;i++){
            snakearr[i].xloc=i+1;
            snakearr[i].yloc=1;
            if (i == 4)
                snakearr[i].head = 1;
        }
        for(int i=0; i <length; i++){
            mvwaddch(win,snakearr[i].yloc,snakearr[i].xloc,'o');
        }
        // creates the trophy and timer
        time_t begin, end;
        time(&begin);
        
        randX = (rand()%COLS-2) +2;
        randY = (rand()%LINES-2) +2;
        mvwaddch(win, randY, randX, trophy);
        trophyTime =(rand()%trophyMax) + 1;
        wrefresh(win);
        //moves the snake by coping the snakes position from the snake struct in front of it unless it is the head of snake then that will move depending on the key that is pressed
        while(1){
            // Checks if snake has no reached the trophy in given amount of time
            if (((snakearr[4].xloc != randX) || (snakearr[4].yloc != randY)) && (time(&end) - begin) >= trophyTime) {
                mvwaddch(win, randY, randX, ' ');
                randX = (rand()%COLS-2) +2;
                randY = (rand()%LINES-2) +2;
                mvwaddch(win, randY, randX, trophy);
                trophyTime =(rand()%trophyMax) + 1;
                time(&begin);
            }
            // If the snake reaches the trophy update the score and move the location
            else if((snakearr[4].xloc == randX) && (snakearr[4].yloc == randY)) {
                score++;
                mvwprintw(win, 0, xMax - 30, "Score: %d", score);
                mvwaddch(win, randY, randX, ' ');
                randX = (rand()%COLS-2) +2;
                randY = (rand()%LINES-2) +2;
                mvwaddch(win, randY, randX, trophy);
                trophyTime =(rand()%trophyMax) + 1;
                time(&begin);
            }

            ch = wgetch(win);
            if(ch == KEY_UP && dir != DOWN) dir = UP;
            else if(ch == KEY_DOWN && dir != UP) dir = DOWN;
            else if(ch == KEY_LEFT && dir != RIGHT) dir = LEFT;
            else if(ch == KEY_RIGHT && dir != LEFT) dir = RIGHT;
            // Close game if x is pressed
            else if(ch == 'x' || ch == 'X') break;

            for(int i=0; i <length; i++){
                    mvwaddch(win,snakearr[i].yloc,snakearr[i].xloc,' ');
                }
            if (dir==UP){
                updateSnake(snakearr, length, dir);
            }
            if (dir==DOWN){
                updateSnake(snakearr, length, dir);
            }
            if (dir==LEFT){
                updateSnake(snakearr, length, dir);
            }
            if (dir==RIGHT){
                updateSnake(snakearr, length, dir);
            }
            //prints the snake
            for(int i=0; i <length; i++){
                mvwaddch(win,snakearr[i].yloc,snakearr[i].xloc,'o');
            }
            wrefresh(win);
            usleep(250000);  

            // If the snake hits an edge end the game
            if(snakearr[4].xloc == xMax || snakearr[4].yloc == yMax || snakearr[4].xloc == 0 || snakearr[4].yloc == 0) {
                break;
            }
        }      
    wrefresh(win);
    endwin();
return 0;
}

void updateSnake(struct snake arr[], int length, int dir){
    if (dir == UP){
        for(int i = 0;i<length;i++){
            if(arr[i].head){
                arr[i].yloc--;
            }
            else{
                arr[i].xloc = arr[i+1].xloc;
                arr[i].yloc = arr[i+1].yloc;
            }
        }
    }
    if (dir == DOWN){
        for(int i = 0;i<length;i++){
            if(arr[i].head){
                arr[i].yloc++;
            }
            else{
                arr[i].xloc = arr[i+1].xloc;
                arr[i].yloc = arr[i+1].yloc;
            }
        }
    }
    if (dir==LEFT){
        for(int i = 0;i<length;i++){
            if(arr[i].head){
                arr[i].xloc--;
            }
            else{
                arr[i].xloc = arr[i+1].xloc;
                arr[i].yloc = arr[i+1].yloc;
            }
        }
    }
    if (dir==RIGHT){
        for(int i = 0;i<length;i++){
            if(arr[i].head){
                arr[i].xloc++;
            }
            else{
                arr[i].xloc = arr[i+1].xloc;
                arr[i].yloc = arr[i+1].yloc;
            }
        }
    }
}
