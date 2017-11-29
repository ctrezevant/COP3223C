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

// I can worry about polluting the global namespace when I'm dead.
int ACCESSIBLE_HOURS_PER_DAY = 8;
int MAP_NUMSQUARES = 9;
int MAP_NUMPIRATES = 4;
int BOARD_SIZE;
int TOTAL_TREASURE_AVAILABLE = 0;

int time_elapsed = -1;
int total_treasure_collected = 0;

map island[3][3];
pirate pirates[4];

void read_parse_map();
void print_board();
void print_crew();
void print_time();
void prompt_moves();
void dig(int row, int col);
int check_ended();

int main(){
  if(time_elapsed < 0){
    printf("You have arrived at Treasure Island!");
    read_parse_map();
    time_elapsed = 0;
  }

  while(check_ended() == 0){
    check_ended();
    print_time();
    print_crew();
    prompt_moves();
  }

  if(total_treasure_collected == TOTAL_TREASURE_AVAILABLE){
    printf("All of the pirate's treasure belongs to you now!");
  } else {
    printf("You are forced to evacuate the island.  You have claimed %d pieces of the pirate's treasure!", total_treasure_collected);
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

  // Since the island is a square x by x grid, we can get
  // the number of rows/columns by square rooting the board size
  BOARD_SIZE = sqrt(MAP_NUMSQUARES);

  char filename[30];
  FILE* mapfile;

  printf("\nWhat is the name of your map? \n");
  scanf(" %s", filename);

  mapfile = fopen(filename, "r");

  for(int i = 0; i < MAP_NUMSQUARES; i++){
    fscanf(mapfile, "%d", &island[i].sand);
    fscanf(mapfile, "%d", &island[i].treasure);
    TOTAL_TREASURE_AVAILABLE += &island[i].treasure; // Computes total amount of treasure to clear
  }

  for(int i = 0; i < MAP_NUMPIRATES; i++){
    fscanf(mapfile, "%d", &pirates[i].dig);
    fscanf(mapfile, "%d", &pirates[i].carry);
  }

  fclose(mapfile);

}

void prompt_moves(){

  for(int i = 0; i < MAP_NUMPIRATES; i++){
    if(check_ended() == 1)
      break;

    int destrow, destcol;
    printf("\nWhere would you like to send crew member %d? \n", i+1);
    print_board();

    char input[3];
    fgets(input, sizeof(input), stdin);
    sscanf(input, " %d %d", &destrow, &destcol);

    dig(destrow, destcol);
  }
}

void dig(int row, int col){



  /*You have removed some of the sand from this section.
You have removed all the sand from this section!
You take all of the treasure back to the ship!
You take some of the treasure back to the ship.
This section has already been cleared.*/
}

void print_time(){
  printf("\nYou have %d hours left to dig up the treasure.\n", (ACCESSIBLE_HOURS_PER_DAY - time_elapsed));
}

int check_ended(){
  if(total_treasure_collected == TOTAL_TREASURE_AVAILABLE || time_elapsed >= ACCESSIBLE_HOURS_PER_DAY){
    return 1;
  } else {
    return 0;
  }
}
