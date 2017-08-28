/*
  COP 3223C - Project 1
  Charlton Trezevant
  Professor Sarah Angell

  Assignment #1 (Pirate Time)
*/

#include <stdio.h>

const int DIST_TO_CARIB = 7228;
const float ORANGES_PER_SAILOR = 0.5;

int main(){

  int km_day, crew_size;

  printf("How many kilometers can your ship travel in one day?\n");
  scanf("%d", &km_day);

  printf("How many crew members can your ship hold?\n");
  scanf("%d", &crew_size);

  double num_days = (float)DIST_TO_CARIB / km_day;
  double num_oranges = (ORANGES_PER_SAILOR * crew_size) * num_days;

  printf("You will need %.2lf oranges to make the trip!\n", num_oranges);

  return 0;
}
