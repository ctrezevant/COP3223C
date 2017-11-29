/*
  COP 3223C - Project 8
  Charlton Trezevant
  Professor Sarah Angell

  Assignment #8 (treasurehunt.c)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
  int dig;
  int carry;
  int resting; // ok?
} pirate;

typedef struct {
  int sand;
  int treasure;
} map;

// I'll have time to worry about polluting the global namespace when I'm dead.
int ACCESSIBLE_HOURS_PER_DAY = 8;
int MAP_NUMSQUARES = 9;
int MAP_NUMPIRATES = 4;
// Since the island is a square x by x grid, we can get
// the number of rows/columns by square rooting the board size (but not for constants at compile time)
int BOARD_SIZE = 3;
int time_elapsed = -1;

map island[3][3];
pirate pirates[4];

void read_parse_map();
void print_board();
void print_crew();
void print_time();
void time_remaining();
void dig(int crewnumber, int spacenumber);

int main(){
  if(time_elapsed < 0){
    printf("You have arrived at Treasure Island!");
    read_parse_map(island, pirates);
    time_elapsed = 0;
  }

  return 0;
}


void print_board(){

  int i, j;

  // For each row
  for(i = 0; i < BOARD_SIZE; i++){
    printf("\t"); // Start off each row with an indentation

    // For each square in the current row
    for(j = 0; j < BOARD_SIZE; j++){
      // Pointer to the map struct of the current square
      map* current_square = &island[i][j];
      // If there's sand covering the square currently
      if((*current_square).sand > 0){
        printf("	%ds", (*current_square).sand);
      // If there's no sand covering the current square, and the square contains treasure
      } else if((*current_square).sand <= 0 &&  (*current_square).treasure > 0){
        printf("	%dT", (*current_square).treasure);
      // Otherwise, if there's no sand *and* no treasure
      } else {
        printf("	- ");
      }
    }

    printf("\n"); // end each row with a newline
  }
}

void print_crew(){

  // Print a descriptive header for the values in the table
  printf("Crew  Dig  Carry\n");

  for(int i = 0; i < MAP_NUMPIRATES; i++){
    // Pointer to entry for current crew member in the pirates array
    pirate* current_member = &pirates[i];
    // Print statistics for the current crew member
    printf("%d  %d  %d", i+1, (*current_member).dig, (*current_member).carry);
    printf("\n"); // end each row with a newline
  }
}

void read_parse_map(){
  char filename[30];
  FILE* mapfile;

  printf("\nWhat is the name of your map? \n");
  scanf(" %s", filename);

  mapfile = fopen(filename, "r");

  for(int i = 0; i < MAP_NUMSQUARES; i++){
    fscanf(mapfile, "%d", &island[i].sand);
    fscanf(mapfile, "%d", &island[i].treasure);
  }

  for(int i = 0; i < MAP_NUMPIRATES; i++){
    fscanf(mapfile, "%d", &pirates[i].dig);
    fscanf(mapfile, "%d", &pirates[i].carry);
  }

  fclose(mapfile);

}

void dig(int crewnumber, int spacenumber){



  /*You have removed some of the sand from this section.
You have removed all the sand from this section!
You take all of the treasure back to the ship!
You take some of the treasure back to the ship.
This section has already been cleared.*/
}
