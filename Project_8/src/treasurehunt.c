/*
  COP 3223C - Project 8
  Charlton Trezevant
  Professor Sarah Angell

  Assignment #8 (treasurehunt.c)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Define structures for pirates and squares on the map, as per the spec.
typedef struct {
  int dig;
  int carry;
} pirate;

typedef struct {
  int sand;
  int treasure;
} map;

int ACCESSABLE_HOURS_PER_DAY = 8; // Total number of hours per day that the island is accessable
#define MAP_NUMPIRATES 4 // Total number of pirates in the crew
#define BOARD_SIZE 3 // sqrt(total squares on map). There are 9 squares, in our case
int TOTAL_TREASURE_AVAILABLE = 0; // Total amount of treasure on the island, also filled in later.

int time_elapsed = -1; // Total count of the time elapsed (i.e. hours between turns)
int total_treasure_collected = 0; // Total count of treasure collected by the user

map island[BOARD_SIZE][BOARD_SIZE]; // Global 2D array of squares
pirate pirates[MAP_NUMPIRATES]; // Global array of crew members

// Function declarations
void read_parse_map();
void print_board();
void print_crew();
void print_time();
void prompt_moves();
void dig(int row, int col, int crew_member);
int check_ended();

// I can worry about "polluting the global namespace" when I'm dead.

int main(){
  // time_elapsed is only < 0 when the user first starts the game
  // If this is the case, we need to get everything set up, first
  if(time_elapsed < 0){
    printf("You have arrived at Treasure Island!\n");
    read_parse_map(); // This function reads/parses the map, and initializes everything
    time_elapsed = 0; // Properly set time_elapsed after setup
  }

  // This while loop will run so long as the game has not ended
  while(check_ended() == 0){
    print_time(); // Print the time remaining in the game
    print_crew(); // Print the crew member statistics
    prompt_moves(); // This function prompts the user for input
  }

  // Depending on the user's score at the end of the game, we may need to
  // display one or the other of these messages
  if(total_treasure_collected == TOTAL_TREASURE_AVAILABLE){
    printf("All of the pirate's treasure belongs to you now!\n");
  } else {
    printf("You are forced to evacuate the island.  You have claimed %d pieces of the pirate's treasure!\n", total_treasure_collected);
  }

  return 0; // <3
}


// You may not have guessed it by now, but this function
// prints the game board (the island's squares)
void print_board(){

  int i, j;

  // For each row
  for(i = 0; i < BOARD_SIZE; i++){

    // For each square in the current row
    for(j = 0; j < BOARD_SIZE; j++){
      // Pointer to the map struct of the current square
      map* current_square = &island[i][j];
      // If there's sand covering the square currently
      if((*current_square).sand > 0){
        printf(" %ds", (*current_square).sand);
      // If there's no sand covering the current square, and the square contains treasure
      } else if((*current_square).sand <= 0 &&  (*current_square).treasure > 0){
        printf(" %dT", (*current_square).treasure);
      // Otherwise, if there's no sand *and* no treasure
      } else {
        // Placeholder value for cleared squares
        printf(" - ");
      }
    }

    printf("\n"); // end each row with a newline
  }
}

// This one also follows a confusing naming convention. My apologies
// To describe it in a straightforward fashion, print_crew prints out statistics
// about each crew member's index, as well as dig and carry scores
void print_crew(){

  // Print a descriptive header for the values in the table
  printf("Crew  Dig  Carry\n");

  for(int i = 0; i < MAP_NUMPIRATES; i++){
    // Pointer to entry for current crew member in the pirates array
    pirate* current_member = &pirates[i];
    // Print statistics out for the current crew member
    printf("%d     %d    %d", i+1, (*current_member).dig, (*current_member).carry);
    printf("\n"); // end each row with a newline
  }
}

// After main, this is the first function called in this program.
// Since the first thing our program has to do is read/parse a map file,
// this function is named (rather confusingly), read_parse_map (which I'm
// very sorry about, I truly could not think of a more concise or descriptive name)
void read_parse_map(){

  char filename[30];
  FILE* mapfile;

  printf("\nWhat is the name of your map? \n");
  scanf(" %s", filename);

  mapfile = fopen(filename, "r");

  for(int i = 0; i < BOARD_SIZE; i++){ // For each row in the board
    for(int j = 0; j < BOARD_SIZE; j++){ // For each column in the board
      fscanf(mapfile, "%d", &island[i][j].sand); // Read the next two sand and treasure values from the map file
      fscanf(mapfile, "%d", &island[i][j].treasure);
      TOTAL_TREASURE_AVAILABLE += island[i][j].treasure; // Compute total amount of treasure to clear (for later use)
    }
  }

  // For each member of the crew
  for(int i = 0; i < MAP_NUMPIRATES; i++){
    fscanf(mapfile, "%d", &pirates[i].dig); // Read the next two dig/carry values from the map file
    fscanf(mapfile, "%d", &pirates[i].carry);
  }

  // Close the file stream peacefully
  fclose(mapfile);

}

// This function will prompt the user for some moves to execute
// In the event that a winning move is played, the game will conclude immediately
// (even if there is additional time or there are moves remaining)
void prompt_moves(){

  for(int i = 0; i < MAP_NUMPIRATES; i++){
    if(check_ended() == 1) // Before prompting, check if the game has ended
      break; // Break if it has

    int destrow, destcol; // Temp variables to hold user selections
    printf("\nWhere would you like to send crew member %d? \n", i+1);
    print_board(); // Display the board

    scanf(" %d %d", &destrow, &destcol);

    dig(destrow, destcol, i); // After selection, dig for treasure at the chosen index
  }

  time_elapsed++; // Increment the elapsed time by 1 hour following each round of moves
}


// This function's purpose is to have a particular crew member (at a given index crew_index)
// dig from a particular square (whose index is passed as the values row and col)
// In my stupor, however, I named it after the DNS querying utility
// I truly am a fool
void dig(int row, int col, int crew_index){

  // Convert the values of row and col to be in terms of index=0
  row--;
  col--;

  map* current_square = &island[row][col]; // Pointer to the square currently being dug on
  pirate* crew_member = &pirates[crew_index]; // Pointer to the crew member who'll be doing the digging
  // Sometimes there is no sand or no treasure left
  // We check for that and store the results in these variables, because this
  // is one of those rare cases where sanity is valued in software development
  int sand_already_cleared = 0, treasure_already_cleared = 0;

  // If there is no sand to be cleared, we note this so that we dont' attempt to
  // clear any ourselves.
  if((*current_square).sand == 0)
    sand_already_cleared = 1;

  // If the chosen crew member has a dig statistic that is greater than or equal
  // to the amount of sand to remove (and additionally if there is sand to remove)
  if((*crew_member).dig <= (*current_square).sand && sand_already_cleared == 0){
    // Remove an amount of sand from the current square equal to crew_member.dig
    (*current_square).sand -= (*crew_member).dig;

  // Otherwise, if the member has a dig value greater than the remaining clearable
  // sand, we can simply set the current square's sand value to 0
  } else if(sand_already_cleared == 0) {
    // Crew member has dig value greater than amount of sand to clear
    (*current_square).sand = 0;
  }

  // If there's no sand in the current square, and the square was not already
  // cleared of sand before the crew member arrived
  if((*current_square).sand == 0 && sand_already_cleared == 0){
    printf("You have removed all the sand from this section!\n");
  } else {
    printf("You have removed some of the sand from this section.\n");
  }

  // If there is no treasure to be cleared, note this so that we dont' attempt to
  // clear any. This also serves for the "already cleared" message.
  if((*current_square).treasure == 0){
    treasure_already_cleared = 1;
  }

  // If the carrying capacity of the current crew member is less than the amount of
  // treasure present, and said treasure has not already been cleared from this square
  if((*crew_member).carry <= (*current_square).treasure && treasure_already_cleared == 0){
    total_treasure_collected += (*crew_member).carry; // Add amount to be removed to the global total collected
    (*current_square).treasure -= (*crew_member).carry; // Decrement the square's value by the amount removed
    printf("You take some of the treasure back to the ship.\n");

  // Otherwise, if treasure hasn't been cleared off, the crew member can carry at least as much as to be
  // removed, and there is treasure present
  } else if(treasure_already_cleared == 0){
    total_treasure_collected += (*current_square).treasure; // Add treasure in square to total collected
    (*current_square).treasure = 0; // Clear treasure from current square
    printf("You take all of the treasure back to the ship!\n");
  }

  // None of the above will run if treasure_already_cleared = 1
  // If there's no treasure on this square, we tell the user here
  if(treasure_already_cleared == 1)
        printf("This section has already been cleared.\n");
}

// This function prints the remaining time in the game
// I made this its own function because I felt like it.
void print_time(){
  printf("\nYou have %d hours left to dig up the treasure.\n", (ACCESSABLE_HOURS_PER_DAY - time_elapsed));
}

// Checks if the game has ended
// This is true if all treasure has been collected, or if the user has run out of time.
int check_ended(){
  if(total_treasure_collected == TOTAL_TREASURE_AVAILABLE || time_elapsed >= ACCESSABLE_HOURS_PER_DAY){
    return 1;
  } else {
    return 0;
  }
}
