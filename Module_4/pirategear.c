/*
  COP 3223C - Project 3
  Charlton Trezevant
  Professor Sarah Angell

  Assignment #3 (Outfitting the Crew)
*/
#include <stdio.h>

const int USED_GEAR_COST = 5;
const int NEW_GEAR_COST = 15;

int main(){
  int usedGearAmount = 0, newGearAmount = 0, gearAmount, choice;
  int quit = 0;

  while(quit == 0){
    printf("\nWelcome to the market!\nWhat would you like to do\?\n");
    printf("\t1. Buy New Gear.\n\t2. Buy Used Gear.\n\t3. Quit\n");
    scanf(" %d", &choice);

    if(choice == 3)
      quit = 1;

    if(choice < 1 || choice > 3){
      printf("Sorry, %d is not a valid choice.\n\n", choice);
      choice = 0;
    }

    if(choice == 1){
        printf("How many pieces of new gear would you like to buy?\n");
        scanf(" %d", &gearAmount);
        newGearAmount += gearAmount;
    }

    if(choice == 2){
        printf("How many pieces of used gear would you like to buy?\n");
        scanf(" %d", &gearAmount);
        usedGearAmount += gearAmount;
    }

  }

  int totalCost  = (newGearAmount * NEW_GEAR_COST) + (usedGearAmount * USED_GEAR_COST);
  printf("Your total cost is %d gold pieces.\n", totalCost);
  printf("You obtained %d pieces of new gear and %d pieces of used gear.\n", newGearAmount, usedGearAmount);

  if((newGearAmount + usedGearAmount) > 0)
    printf("The cost per piece of gear is %.2f pieces of gold.\n\n", (float)(totalCost / (newGearAmount + usedGearAmount)));

  return 0;
}
