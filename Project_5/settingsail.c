/*
  COP 3223C - Project 5
  Charlton Trezevant
  Professor Sarah Angell

  Assignment #5 (settingsail.c)
*/

#include <stdio.h>
#include <stdlib.h>

int main(){
  float monthTempPercentages[12];
  FILE *temperatures;
  char filepath[30];

  printf("\nPath to temperature data file: ");
  scanf(" %s", filepath);
  printf("\nWill load temperatures from: %s\n\n", filepath);
  temperatures = fopen(filepath, "r");

  int lowTempBound, highTempBound;
  printf("What's the lowest temperature your crew will sail in? (between 0-80): ");
  scanf(" %d", &lowTempBound);
  printf("What's the highest temperature your crew will sail in? (between %d and 120): ", lowTempBound);
  scanf(" %d", &highTempBound);

  int numTemps, currentMonth;

  for(currentMonth = 0; currentMonth < 12; currentMonth++){
      fscanf(temperatures, "%d", &numTemps);
      float currentTemp = 0, numTempsInRange = 0;

      for(int i = 0; i < numTemps; i++){
        fscanf(temperatures, "%f", &currentTemp);

        if(currentTemp < highTempBound && currentTemp > lowTempBound)
          numTempsInRange++;
      }

      monthTempPercentages[currentMonth] = (numTempsInRange / numTemps);

  }

  int candidateMonthIndex;
  float prevMonthPercentage = 0.0;
  printf("\nBreakdown of temperature data by month: \n");
  for(int i = 0; i < 12; i++){
    float currentMonthPercentage = monthTempPercentages[i];
    printf("\tMonth %d: %.2f percent of days in range.\n", i+1, currentMonthPercentage);

    if(currentMonthPercentage>prevMonthPercentage)
      candidateMonthIndex = i;
  }

  printf("\nYou should leave for the Caribbean in month %d!\n\n", candidateMonthIndex);

  return 0;
}
