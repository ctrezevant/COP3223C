/*
  COP 3223C - Project 4
  Charlton Trezevant
  Professor Sarah Angell

  Assignment #4 (Landing Crew)
*/

#include <stdio.h>

int main(){
  int totalDays, totalTrips;
  float average, tripLength;

  printf("How many days will you observe the landing crew?\n");
  scanf(" %d", &totalDays);

  for(int i = 1; i < totalDays+1; i++){
    printf("How many trips were completed in day #%d?\n", i);
    scanf(" %d", &totalTrips);

    average = 0;

    for(int j = 1; j < totalTrips+1; j++){
      printf("\nHow long was trip #%d?\n", j);
      scanf(" %f", &tripLength);
      average += tripLength;
    }

   average /= totalTrips;

   printf("\n\nDay #%d: The average time was %.3f.\n\n", i, average);
  }

  return 0;
}
