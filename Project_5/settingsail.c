/*
  COP 3223C - Project 5
  Charlton Trezevant
  Professor Sarah Angell

  Assignment #5 (settingsail.c)
*/

// Include stdio and stdlib, which will give us stdin/stdout/fs goodness.
#include <stdio.h>
#include <stdlib.h>

int main(){
  // After each month's percentage is calculated, the value is stored here.
  float monthTempPercentages[12];
  // Pointer to our temperatures file, used when accessing it
  FILE *temperatures;
  // char array, to hold the user-provided path to the temperature file (from scanf below)
  char filepath[30];

  // Prompt the user to input the path to the temperatures file.
  printf("\nPath to temperature data file: ");
  scanf(" %s", filepath);
  // Print the file path they entered, for confirmation
  printf("Will load temperatures from %s.\n\n", filepath);
  // Using the provided path, open the temperatures file, and have `temperatures` point to it.
  temperatures = fopen(filepath, "r");

  // Now we must retrieve the high and low temperature boundaries from the user,
  // so we'll initialize some variables for each of those and politely ask the
  // user to provide their values.
  int lowTempBound, highTempBound;
  printf("What's the lowest temperature your crew will sail in? (between 0-80): ");
  scanf(" %d", &lowTempBound);
  printf("What's the highest temperature your crew will sail in? (between %d and 120): ", lowTempBound);
  scanf(" %d", &highTempBound);


  // These are used as the index of the current month and the number of temperatures
  // provided in the data file for that month, respectively
  int currentMonth, numTemps;

  // Iterate over the file, first reading the integer number of temperature data points
  // recorded, and secondly reading each of those floating point values out for the current month.
  for(currentMonth = 0; currentMonth < 12; currentMonth++){
      fscanf(temperatures, "%d", &numTemps);
      // Initialize the current temperature/number of temps in range values for each month in the file
      // These are temporary values used only in the context of each month's calculations
      float currentTemp = 0, numTempsInRange = 0;

      // For each temperature recorded in the current month
      for(int i = 0; i < numTemps; i++){
        // Read the latest temperature value
        fscanf(temperatures, "%f", &currentTemp);

        // And, if the freshly-read value is within the user's temperature bounds, increment the
        // count of the total number of temperatures in that range by one.
        if(currentTemp < highTempBound && currentTemp > lowTempBound)
          numTempsInRange++;
      }

      // Finally, once all the temperatures have been loaded for the current month,
      // we find the percentage of the month's temperatures within our range by
      // dividing the number of temperatures in the user's bounds by the total number of temperatures.
      monthTempPercentages[currentMonth] = (numTempsInRange / numTemps);

  }

  // This will hold the index of the "best" month to sail in, as we loop.
  int candidateMonthIndex;
  // This value will be continuously updated with the highest percentage we've yet seen.
  // It's initialized to zero here to make things easier.
  float highestSeenPercentage = 0.0;

  printf("\nBreakdown of temperature data by month: \n");

  // The second loop (below) will print out and compare the temperature range percentages
  // for each month.

  // You could technically include this logic within the temperature calculation loop,
  // but since this is just a COP3223 assignment I've already written in this way,
  // there's no reason to concern ourselves with performance optimizations~
  for(int i = 0; i < 12; i++){
    // Get the temp. range percentage for the current month from our array of each month's percentages.
    float currentMonthPercentage = monthTempPercentages[i];
    // And print that out in a nice formatted list
    printf("\tMonth %d: %.1f percent of days in range.\n", i+1, currentMonthPercentage*100);

    // Finally, do a little comparison to see if it's the largest we've seen yet.
    if(currentMonthPercentage>highestSeenPercentage){
      candidateMonthIndex = i;
      highestSeenPercentage = currentMonthPercentage;
    }
  }

  // Information about which month is best to leave is nice to know.
  // That's why we print that information out, here.
  printf("\nYou should leave for the Caribbean in month %d!\n\n", candidateMonthIndex+1);

  // Return 0, because we are happy.
  return 0;
}
