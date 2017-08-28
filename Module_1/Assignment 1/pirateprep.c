/*
  COP 3223C - Project 1
  Charlton Trezevant
  Professor Sarah Angell

  Assignment #1 (Pirate Time)
*/

// Include the stdio library to unlock I/O goodness
#include <stdio.h>

// Declare our two constants (distance to caribbean and
// the number of oranges needed per sailor).
const int DIST_TO_CARIB = 7228;
const float ORANGES_PER_SAILOR = 0.5;

int main(){

  //km_day will hold the maximum possible kilometers traveled per day
  //crew_size will hold the size of the crew
  int km_day, crew_size;

  // Prompt user to supply values for both km_day and crew_size
  printf("How many kilometers can your ship travel in one day?\n");
  scanf("%d", &km_day);

  printf("How many crew members can your ship hold?\n");
  scanf("%d", &crew_size);

  // Calculate the number of days in the voyage.
  // Note: keeping everything in doubles here to avoid integer math errors
  double num_days = (float)DIST_TO_CARIB / km_day;
  // Use the calculated total days spent journeying to
  // determine the number of oranges needed (.5 per crew member per day)
  double num_oranges = (ORANGES_PER_SAILOR * crew_size) * num_days;

  // Spit that value to stdout
  printf("You will need %.2lf oranges to make the trip!\n", num_oranges);

  // Return 0 because presumably everything went well
  return 0;
}
