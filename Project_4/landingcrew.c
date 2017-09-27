/*
  COP 3223C - Project 4
  Charlton Trezevant
  Professor Sarah Angell

  Assignment #4 (Landing Crew)
*/

// Include stdio, so that we can talk to stdin and out
#include <stdio.h>

int main(){
  // These will hold the total number of days, and the total number of trips
  int totalDays, totalTrips;
  /* The following variables are temporary to each day's calculations, and are reused:
   * Average will be used to compute each day's average, and will be overwritten on each iteration
   * per-day
   * tripLength is similar, for each day its value will be overwritten.
   */
  float average, tripLength;

  printf("How many days will you observe the landing crew?\n");
  scanf(" %d", &totalDays); // Fetch total number of days in the trip from the user

  // As we now have the total number of days, we can move on to calculating per-day averages
  // This involves looping over each day (outer loop), then prompting the user for the number
  // of trips in that day
  for(int i = 1; i < totalDays+1; i++){
    printf("How many trips were completed in day #%d?\n", i);
    scanf(" %d", &totalTrips); // Fetch total number of trips per day from the user

    // (re)Initialize average as preparation to compute the current day's average
    average = 0;

    // Loop over the number of trips in each day (inner loop), in order to fetch the time values
    // for each trip in that day. All the trip lengths are then added to average
    for(int j = 1; j < totalTrips+1; j++){
      printf("\nHow long was trip #%d?\n", j);
      scanf(" %f", &tripLength);
      // Increment average by the value of tripLength, for each trip on the current day
      average += tripLength;
    }

   // Computes the daily average, equivalent to average = (average / totalTrips)
   // Since we already have the number of trips taken in the current day, and added
   // all the trip lengths together in the average variable earlier, this is the last step.
   average /= totalTrips;

   // Display the daily average
   printf("\n\nDay #%d: The average time was %.3f.\n\n", i, average);
  }

  // return a happy little exit status
  return 0;
}
