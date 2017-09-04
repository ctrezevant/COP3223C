/*
  COP 3223C - Project 2
  Charlton Trezevant
  Professor Sarah Angell

  Assignment #2 (Crew Recruitment)
*/
#include <stdio.h>

const int MIN_SWIM_DISTANCE = 100;

int main(){

  char can_swim, likes_treasure;
  int swim_distance;
  float ship; // this is just a pun

  printf("Do you like digging for treasure? (Y/N)\n");
  // Note-to-self: Apparently doing a scanf without a space in front of the %code breaks things.
  scanf(" %c", &likes_treasure);

  printf("Are you able to swim?\n");
  scanf(" %c", &can_swim);

  printf("How many meters are you able to swim?\n");
  scanf(" %d", &swim_distance);

  if(can_swim == 'Y' && likes_treasure == 'Y' && swim_distance >= MIN_SWIM_DISTANCE)
    printf("You may join the pirate crew! Arrr!\n");
  else
    printf("You may not join the crew.\n");

  return 0;
}
