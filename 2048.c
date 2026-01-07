#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <time.h>

// width (and height) of the board
#define width 4

char number_box[7][15] = {"*------------*",
                          "|            |",
                          "|            |",
                          "|    2^n     |",
                          "|            |",
                          "|            |",
                          "*------------*"};

void drawBoard(int *board) {
  for (int y = 0; y < width; y++) {
    char space_values[4][12] = {"", "", "", ""};
    for (int x = 0; x < width; x++) {
      if (*(board + x + y * 4)) {
        sprintf(space_values[x], "%d", *(board + x + y * 4));
      }
    }
    
    // print the current row of boxes
    for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 4; j++) {
        // place the number in the center of each box, then print it
        if (i == 3) {
          memset(&number_box[3][1], ' ', 11);
          int padding = (14 - strlen(space_values[j])) / 2;
          sprintf(&number_box[3][padding], "%s", space_values[j]);
          memset(&number_box[3][strlen(number_box[3])], ' ', 13-strlen(number_box[3]));
        }
        
        printf("%s", number_box[i]);
      }
      
      printf("\n");
    }
  }
  
  return;
}


void verticalShift(int board[][4], int start, int end, int inc) {
  // iterate accross columns
  for (int x = 0; x < 4; x++) {
    // move numbers towards end, working backwards through the column
    for (int scan = end; scan != start-inc; scan -= inc) {
      int y = scan;
      int tmp = board[y][x];
      board[y][x] = 0;
      while (1) {
        if (board[y][x] == tmp) {
          board[y][x] *= 2;
          break;
        } else
        if (board[y][x]) {
          board[y-inc][x] = tmp;
          break;
        } else
        if (y == end) {
          board[y][x] = tmp;
          break;
        }
        else {
          y += inc;
        }
      }
    }
  }
  
  return;
}


void horizontalShift(int board[][4], int start, int end, int inc) {
  // iterate accross rows
  for (int y = 0; y < 4; y++) {
    // move numbers towards end, working backwards through the row
    for (int scan = end; scan != start-inc; scan -= inc) {
      int x = scan;
      int tmp = board[y][x];
      board[y][x] = 0;
      while (1) {
        if (board[y][x] == tmp) {
          board[y][x] *= 2;
          break;
        } else
        if (board[y][x]) {
          board[y][x-inc] = tmp;
          break;
        } else
        if (x == end) {
          board[y][x] = tmp;
          break;
        }
        else {
          x += inc;
        }
      }
    }
  }
      
  return;
}


// handle player input (up, down, left, right)
void playerInput(int board[][4], char direction) {
  switch (direction) {
    case 'u':
      // start at y = 3, increment by -1
      verticalShift(board, 3, 0, -1);
      break;
    
    case 'd':
      // start at y = 0, increment by +1
      verticalShift(board, 0, 3, 1);
      break;
    
    case 'l':
      // start at x = 3, increment by -1
      horizontalShift(board, 3, 0, -1);
      break;
    
    case 'r':
      // start at x = 0, increment by +1
      horizontalShift(board, 0, 3, 1);
  }
  
  return;
}



int win(int board[]) {
  for (int i = 0; i < width * width; i++) {
    if (board[i] == 1<<30) {
      printf("You win!\n");
      return 1;
    }
  }
  
  return 0;
}


// detect loss and display loss message (not done)
int loss(int board[]) {
  return 0;
}


int main(int argc, char *argv[]) {
  srand(time(NULL));
  int spaces = width * width;
  int board[width][width];
  memset(board, 0, sizeof(board));
  
  printf("Press enter to begin\n");
  
  // gameloop
  while (getchar()) {
    // generate a new number in a random empty space
    int y, x;
    do {
      y = rand() >> 29;  // keep value of rand below 4
      x = rand() >> 29; 
    } while (board[y][x]);
    board[y][x] = 2;
    
    // exit on win or loss condition
    if (win(&board[0][0]) || loss(&board[0][0])) {
      return 0;
    }
    
    char inputchar;
    do {
      system("clear");
      drawBoard(&board[0][0]);
      printf("input direction (u, d, l, r): ");
      inputchar = getchar();
    } while (inputchar == '\n');
    playerInput(board, inputchar);
  }
}
