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

int randomNum(int, int);
void updateSnake(struct snake[], int, int);
void printSnake(WINDOW *win, struct snake[], int);
void addSnake(struct snake[], int, int);
void initSnake(struct snake[], int, int, int, int, int, int);

int main(){
    int x, y, xMax, yMax, ch;
    int length = 15;
    int score = 0;
    int gameover = 0;

    // Used for generating trophy location
    srand(time(0));
    int randX, randY;
    int trophyMaxTime = 10;
    int trophyTimer;
    int dir = (rand() % 4) + 1;

    initscr();
        clear();
        x = y = 1;
        xMax = COLS-1;
        yMax = LINES-1;
        time_t t;
        int nextX, nextY;
        

        // Hide the cursor
        curs_set(0);
        //Creates the window
        WINDOW *win = newwin(yMax, xMax, 0,0);
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
        initSnake(snakearr, length, dir, yMax, xMax, nextY, nextX);
        printSnake(win, snakearr, length);
        // creates the trophy and timer
        time_t begin, end;
        time(&begin);
        randX = (rand()% (((COLS-1)-15)-10) +1)+10;
        randY = (rand()% (((LINES-1)-10)-5) +1)+5;
        int trophy = randomNum(1, 9);
        //trophyTimer = (rand() % 9)+1;
        mvwprintw(win,randY, randX, "%d", trophy);
        wrefresh(win);
        //moves the snake by coping the snakes position from the snake struct in front of it unless it is the head of snake then that will move depending on the key that is pressed
        while(1){
            ch = wgetch(win);
            if(ch == KEY_UP) {
                if (dir == DOWN) {
                    mvwprintw(win, yMax/2, xMax/2, "For better or worse, you died!");
                    wrefresh(win);
                    usleep(3000000);
                    break;
                }
                else dir = UP;
            }
            else if(ch == KEY_DOWN) {
                if (dir == UP) {
                    mvwprintw(win, yMax/2, xMax/2, "For better or worse, you died!");
                    wrefresh(win);
                    usleep(3000000);
                    break;
                }
                else dir = DOWN;
            }
            else if(ch == KEY_LEFT) {
                if (dir == RIGHT) {
                    mvwprintw(win, yMax/2, xMax/2, "For better or worse, you died!");
                    wrefresh(win);
                    usleep(1000000);
                    break;
                }
                else dir = LEFT;
            }
            else if(ch == KEY_RIGHT) {
                if (dir == LEFT) {
                    mvwprintw(win, yMax/2, xMax/2, "For better or worse, you died!");
                    wrefresh(win);
                    usleep(3000000);
                    break;
                }
                else dir = RIGHT;
            }
            // Close game if x is pressed
            else if(ch == 'x' || ch == 'X'){
                break;
            }
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
            char* nextpos = (char*) calloc(2, sizeof(char));
            //prints the snake
            if (nextpos[0] == 'o'){
                gameover = TRUE;
                free(nextpos);
                break;
            }
            free(nextpos);
            // Checks if snake has no reached the trophy in given amount of time
            if (((snakearr[length-1].xloc != randX) || (snakearr[length-1].yloc != randY)) && (time(&end) - begin) >= trophy) {
                mvwprintw(win, randY, randX, " ");
                randX = (rand()% (((COLS-1)-15)-10) +1)+10;
                randY = (rand()% (((LINES-1)-10)-5) +1)+5;
                while(randX >= xMax){
                    randX = rand()% (COLS-1);
                }
                while(randY >= yMax){
                    randY = rand()% (LINES-1);
                }
                trophy = randomNum(1, 9);
                //trophyTimer = (rand() % 9)+1;
                mvwprintw(win,randY, randX, "%d", trophy);
                time(&begin);
            }
            // If the snake reaches the trophy update the score and move the location
            else if((snakearr[length-1].xloc == randX) && (snakearr[length-1].yloc == randY)) {
                score = score + trophy;
                for (int i=0; i<trophy; i++){
                    length += 1;
                    addSnake(snakearr, length, dir);
                }
                mvwprintw(win, 0, xMax - 30, "Score: %d", score);
                mvwprintw(win, randY, randX, " ");
                randX = (rand()% (((COLS-1)-15)-10) +1)+10;
                randY = (rand()% (((LINES-1)-5)-2) +1)+2;
                trophy = randomNum(1, 9);
                //trophyTimer = (rand() % 9)+1;
                mvwprintw(win,randY, randX, "%d", trophy);
                time(&begin);
            }
            // If the snake hits an edge end the game
            if(snakearr[length-1].xloc == xMax || snakearr[length-1].yloc == yMax || snakearr[length-1].xloc == 0 || snakearr[length-1].yloc == 0) {
                gameover = 1;
            }
            for(int i = 0; i < length-1; i++){
                if(snakearr[length-1].xloc == snakearr[i].xloc && snakearr[length-1].yloc == snakearr[i].yloc){
                gameover = 1;
                }
            }
            if (gameover){
                mvwprintw(win, yMax/2, xMax/2, "For better or worse, you died!");
                wrefresh(win);
                usleep(3000000);
                break;
            }
            printSnake(win, snakearr, length);
            wrefresh(win);
            usleep(100000);  
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

int randomNum(int min, int max){
   return min + (rand() % max);
}

void printSnake(WINDOW *win, struct snake arr[], int length){
    for(int i=0; i <length; i++){
        mvwaddch(win,arr[i].yloc,arr[i].xloc,'o');
    }
}

void addSnake(struct snake snakearr[], int length, int dir){
    for (int i=0;i<length;i++){
        if (i == length-2){
            snakearr[i].head = 0;
        }
        else if (i == length-1){
            snakearr[i].head = 1;
            if (dir == UP){
                snakearr[i].yloc = snakearr[i-1].yloc--;
                snakearr[i].xloc = snakearr[i-1].xloc;
            }
            else if (dir == DOWN){
                snakearr[i].yloc = snakearr[i-1].yloc++;
                snakearr[i].xloc = snakearr[i-1].xloc;
            }
            else if (dir == LEFT){
                snakearr[i].xloc = snakearr[i-1].xloc--;
                snakearr[i].yloc = snakearr[i-1].yloc;
            }
            else if (dir == RIGHT){
                snakearr[i].xloc = snakearr[i-1].xloc++;
                snakearr[i].yloc = snakearr[i-1].yloc;
            }
        }
    }
}

void initSnake(struct snake snakearr[], int length, int dir, int yMax, int xMax, int nextY, int nextX){
    if(dir == UP){
        for(int i=0;i<length;i++){
                snakearr[i].xloc=(xMax/2);
                snakearr[i].yloc=(yMax/2)-i;
                if (i == length-1)
                    snakearr[i].head = 1;
                    nextX = snakearr[i].xloc;
                    nextY = snakearr[i].yloc;
            }
    }
    if(dir == DOWN){
        for(int i=0;i<length;i++){
                snakearr[i].xloc=(xMax/2);
                snakearr[i].yloc=(yMax/2)+i;
                if (i == length-1)
                    snakearr[i].head = 1;
                    nextX = snakearr[i].xloc;
                    nextY = snakearr[i].yloc;
            }
    }
    if(dir == LEFT){
        for(int i=0;i<length;i++){
                snakearr[i].xloc=(xMax/2)-i;
                snakearr[i].yloc=(yMax/2);
                if (i == length-1)
                    snakearr[i].head = 1;
                    nextX = snakearr[i].xloc;
                    nextY = snakearr[i].yloc;
            }
    }
    if(dir == RIGHT){
        for(int i=0;i<length;i++){
                snakearr[i].xloc=(xMax/2)+i;
                snakearr[i].yloc=(yMax/2);
                if (i == length-1)
                    snakearr[i].head = 1;
                    nextX = snakearr[i].xloc;
                    nextY = snakearr[i].yloc;
            }
    }
}