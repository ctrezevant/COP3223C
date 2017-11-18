/*
  COP 3223C - Project 7
  Charlton Trezevant
  Professor Sarah Angell

  Assignment #7 (oceanvoyage.c)
*/

//Included Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Constants for Arrays
#define STRLENGTH 30
#define NUMCREW 5
#define NUMSUPPLIES 4

//Constants for Distances (measured in miles)
#define CANARY 1261
#define GRENADA 3110
#define FINAL 500
#define DISTANCE 4871

//Function Signatures - do not change these
void setup(char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW], int supplies[NUMSUPPLIES], int *captaintype, int *funds, int *distanceperday);
int countcrew(int crewstatus[NUMCREW]);
void printstatus(char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW]);
void getsupplies(char supplytypes[NUMSUPPLIES][STRLENGTH], int supplies[NUMSUPPLIES], int *funds);
void dailyreport(char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW], int supplies[NUMSUPPLIES], int funds, int traveled);
void rest(int supplies[NUMSUPPLIES], char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW], int *days);
int fish();
int max(int a, int b);
int min(int a, int b);
void event(char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW], int *days, int supplies[NUMSUPPLIES]);

// Below are prototypes for helper functions that I (charlton) have written
int feed_crew();

//Main function - This is the final version of main.  Any changes you make while
//creating the functions should be removed prior to submission.
int main(void) {
    //crewnames and supplytypes are arrays of strings to store the names of the crew members
    //and the types of supplies that can be purchased and taken on the voyage
    char crewnames[NUMCREW][STRLENGTH];
    char supplytypes[NUMSUPPLIES][STRLENGTH] = {"Food", "Clothes", "Ship Parts", "Shovels"};
    //stop indicates whether or not the user would like to stop at a port
    //crewstatus indicates the status of each crew member, corresponding to the order of names
    //supplies has a total for each type of supply, corresponding to the order of supplies
    char stop;
    int crewstatus[NUMCREW], supplies[NUMSUPPLIES];
    //the distanceperday and funds depends on the captaintype the user selects
    //day is the current day, traveled is the total miles traveled, i is a loop counter
    //and action stores the intended action of the user for the day
    int distanceperday, captaintype, funds, traveled=0, day=1;
    int i, action;

    //seed the pseudorandom number generator
    srand(time(0));

    //initialize each variable with information from the user
    setup(crewnames, crewstatus, supplies, &captaintype, &funds, &distanceperday);

    //begin the game by purchasing initial supplies
    printf("\nBefore leaving Port Marin, you should purchase some supplies.\n");
    getsupplies(supplytypes, supplies, &funds);

    //continue the voyage until the ship reaches the intended destination
    //if all crew members perish, the journey cannot continui
    while (traveled < DISTANCE && countcrew(crewstatus) > 0) {
        printf("\n\n--It is day #%d.--\n", day);

        //check to see if the ship has reached the next port
        if(traveled >= (GRENADA+CANARY) && traveled < (GRENADA + CANARY + distanceperday) ) {
            printf("You have arrived at Grenada, at the edge of the Carribbean Sea.\n");
            printf("Would you like to make port? (Y/N)\n");
            scanf(" %c", &stop);

            if (stop == 'Y' || stop == 'y')
                getsupplies(supplytypes, supplies, &funds);

            traveled = (GRENADA+CANARY) + distanceperday;
        }
        else if (traveled >= CANARY && traveled < (CANARY + distanceperday) ) {
            printf("You have arrived at the Canary Islands.\n");
            printf("Would you like to make port? (Y/N)\n");
            scanf(" %c", &stop);

            if (stop == 'Y' || stop == 'y')
                getsupplies(supplytypes, supplies, &funds);

            traveled = CANARY + distanceperday;
        }
        //if between destinations: print the daily report and process the user's action for the day
        else {
            dailyreport(crewnames, crewstatus, supplies, funds, traveled);

            printf("\nWhat would you like to do?\n");
            printf("1 - Fish\n");
            printf("2 - Rest\n");
            printf("3 - Continue\n");
            scanf("%d", &action);

            if(action == 1) {
                supplies[0] += fish();
            }
            else if(action == 2) {
                day--;
                rest(supplies, crewnames, crewstatus, &day);
            }
            else {
                traveled += distanceperday;
                supplies[0] = max(supplies[0] - countcrew(crewstatus) * 2, 0);
                event(crewnames, crewstatus, &day, supplies);
            }
        }
        day++;
    }

    printf("\n\n");

    //The final printout changes based on which condition broke the while loop
    if (countcrew(crewstatus) == 0) {
        printf("Your crew has perished in the search for treasure. :(\n");

        printstatus(crewnames, crewstatus);
    }
    else {
        printf("Your crew has made it safely to the island.\n");

        printstatus(crewnames, crewstatus);

        if(supplies[3] >= countcrew(crewstatus))
            printf("You have enough shovels to dig up the treasure!\n");
        else
            printf("Unfortuantely, you will not be able to dig up the treasure.\n");
    }

    return 0;
}

//Pre-conditions:  none
//Post-conditions: each input parameter should be assigned an initial value

//What to do in this function: Provide the starting message for the user and ask how they plan to travel.
//  Based on their response initialize captaintype, funds, and distanceperday
//  -captaintype 1 should get 1000 funds and 80 distanceperday
//  -captaintype 2 should get 900 funds and 90 distanceperday
//  -captaintype 3 should get 800 funds and 100 distanceperday

//  Ask the user for their name.  Place this in the first row of crewnames, representing the captain.  Treat
//  crewnames as a 1-Dimensional array of strings. As an example: printf("%s", crewnames[0]); would print the
//  first string or the captains name to the screen.  Then ask for the names of the other 4 crew members in a loop.

//  Set the crew status to 2 for each crew member, representing healthy
//  Set the initial amount of supplies to be 0 for each supply
void setup(char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW], int supplies[NUMSUPPLIES], int *captaintype, int *funds, int *distanceperday) {

  printf("You may now take your ship and crew from Port Marin, Spain to the hidden island in the Caribbean on the old pirate's map.\n");

  // Prompt the user for the captain type
  printf("How will you travel?\n1 - As a merchant\n2 - As a privateer\n3 - As a pirate\n");
  scanf("\n %d", captaintype);

  // Act upon the user's selection, setting funds and distance per day accordingly for each captain type
  // This will also notify the user of those attributes
  switch(*captaintype){
    case 1:
      printf("As a merchant, you begin your trip with 1000 gold pieces.\nYou will be sailing your Carrack, with an average speed of 80 miles per day.\n");
      *funds = 1000;
      *distanceperday = 80;
      break;

    case 2:
      printf("As a privateer, you begin your trip with 900 gold pieces.\nYou will be sailing your Galleon, with an average speed of 90 miles per day.\n");
      *funds = 900;
      *distanceperday = 90;
      break;

    case 3:
      printf("As a pirate, you begin your trip with 800 gold pieces.\nYou will be sailing your Clipper, with an average speed of 100 miles per day.\n");
      *funds = 800;
      *distanceperday = 100;
      break;
  }

  // Retrieve the player's name from the user
  printf("\nWhat is your name, Captain?\n");
  scanf("\n %s", crewnames[0]);

  // Loop over each member of the crew, so the user can assign names for them
  printf("\nWho are the other members of your crew?\n");
  for(int i = 1; i < NUMCREW; i++){
    printf("%d:", i);
    scanf("%s", crewnames[i]);
  }

  // Initialize crew member statuses, giving them all the gift of health (at least initially)
  for(int i = 0; i < NUMCREW; i++)
    crewstatus[i] = 2;

  // Initialize the values of all supplies to 0.
  for(int i = 0; i < NUMSUPPLIES; i++)
    supplies[i] = 0;
}

//Pre-conditions:  crewstatus is an array of numerical indicators for the status of each crew member
//                 0 - deceased, 1 - ill, 2 - healthy
//Post-conditions: returns the number of crew members that are alive

//What to do in this function: Traverse the crew status array and count how many crew members
//  have a status that is not 0.  Return this count.
int countcrew(int crewstatus[NUMCREW]) {
  // alive will maintain a tally of all currently living crew members
  int alive = 0;
  for(int i = 0; i < NUMCREW; i++)
    if(crewstatus[i] > 0) // For each living member of the crew, increment alive by 1
      alive++;

  return alive;
}

//Pre-conditions:  crew names is an array of strings for the crew members
//                 crewstatus is an array of numerical indicators for the status of each crew member
//                 0 - deceased, 1 - ill, 2 - healthy
//Post-conditions: none

//What to do in this function: print each crew members name and their status.
//  You may use a status array to shorten this process: char status[3][STRLENGTH] = {"Deceased", "Ill", "Healthy"};
void printstatus(char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW]) {
  // e-z lookup table (as suggested in the comments)
  char statuses[3][STRLENGTH] = {"Deceased", "Ill", "Healthy"};
  for(int i = 0; i < NUMCREW; i++){
    int member_status = crewstatus[i]; // Temp int to hold the current member's status (for readability)
    // For each crew member, print the name and current status (healthy/ill/deceased)
    printf("%s: %s\n", crewnames[i], statuses[member_status]);
  }

}

//Pre-conditions:  supplytypes in an array of strings and gives the name of each supply type
//                 supplies is an array of integers representing how many of each type the crew has
//                 funds represents how many gold pieces the crew has to spend
//Post-conditions: the user may choose to buy supplies: incrementing values in supplies and
//                 decrementing funds

//What to do in this function: First print out the amount of gold the crew has.
//  Then print each supply type and the amount it costs.  You may use a cost array to shorten
//  this process: int supplycosts[NUMSUPPLIES] = {1, 2, 20, 10};
//  Based on the user's selection, ask the user how many of that supply they would like to buy.
//  Verify that the user has enough gold for their purchase and update the correct index of supplies.
//  Deduct the corresponding amount from the user's funds.
void getsupplies(char supplytypes[NUMSUPPLIES][STRLENGTH], int supplies[NUMSUPPLIES], int *funds) {
  // Temp. variable to hold the user's current menu selection
  int selection = 0;

  // So long as the selection isn't 5 (exit), keep looping
  while(selection != 5){
    printf("\nBuying Supplies:\nYou have %d gold pieces.\nAvailable Supples:", *funds);
    printf("\n1. Food - 1 gold pieces\n2. Clothes - 2 gold pieces\n3. Ship Parts - 20 gold pieces\n4. Shovels - 10 gold pieces\n5. Leave Store\n");
    // Depending on whether or not you put in unexpected characters, scanf will fail and this'll just enter an infinite loop
    // a call to fflush usually makes this a bit more stable, but since this call is technically undefined behavior I'm leaving it out
    // and just assuming the user won't mess up the stdin buffer
    // fflush(stdin);
    scanf("\n %d", &selection);

    // After gathering user input, we must act on it
    // Let's get out our handy dandy switch statement for that
    switch(selection){
      case 1: { // If the user wants to buy food
        // All of these cases are effectively the same, so I'll comment them here.
        // In all of these cases, amount is the amount of supplies the user wants to buy
        // This is later multiplied by the price per unit to determine the cost to the user.
        int amount = 0;
        printf("Food\nHow many pounds of food do you want to buy?\n");
        scanf("\n %d", &amount);

        // The user needs to have enough funds to purchase the items (less than or equal to their current balance)
        if(amount <= *funds) {
          // Subtract supply cost from funds
          *funds -= amount; // Subtract the cost of supplies from the user's funds. 1 unit of food = 1 gold
          // Add the purchased supplies to the users inventory, at the appropriate index
          supplies[0] += amount;
        } else {
          // If the user doesn't have enough funds to purchase supplies,
          // let them know that they're poor
          printf("Sorry, you cannot afford that much food.\n");
        }
        break;
      }

      case 2: { // User is buying clothes
        int amount = 0;
        printf("Clothes\nHow many sets of clothes do you want to buy?\n");
        scanf("\n %d", &amount);

        if(amount <= *funds) {
          *funds -= amount*2; // 2 gold per set of clothes
          supplies[1] += amount;
        } else {
          printf("Sorry, you cannot afford that many sets of clothes.\n");
        }
        break;
      }

      case 3: { // User is buying ship parts
        int amount = 0;
        printf("Ship Parts\nHow many extra ship parts do you want to buy?\n");
        scanf("\n %d", &amount);

        if(amount <= *funds) {
          *funds -= amount*20;  // 20 gold per set of parts
          supplies[2] += amount;
        } else {
          printf("Sorry, you cannot afford that many ship parts.\n");
        }
        break;
      }

      case 4: { // User is buying shovels
        int amount = 0;
        printf("Shovels\nHow many shovels do you want to buy?\n");
        scanf("\n %d", &amount);

        if(amount <= *funds) {
          *funds -= amount*10; // 10 gold per shovel
          supplies[3] += amount;
        } else {
          printf("Sorry, you cannot afford that many shovels.\n");
        }
        break;
      }
    }
  }
}

//Pre-conditions:  crew names is an array of strings for the crew members
//                 crewstatus is an array of numerical indicators for the status of each crew member
//                 0 - deceased, 1 - ill, 2 - healthy
//                 supplies is an array of integers representing how many of each type the crew has
//                 funds represents how many gold pieces the crew has to spend
//                 traveled represents the total number of miles the ship has traveled from the beginning
//Post-conditions: none

//What to do in this function: Print a daily status report by telling the user how many miles have been
//  traveled.  Then, print the status of the crew by calling printstatus.  Print the funds and amount
//  of food that the ship has.  Then, let the user know how far they are from their next destination.
void dailyreport(char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW], int supplies[NUMSUPPLIES], int funds, int traveled) {
  // Display the daily report
  printf("You have traveled %d miles.\n", traveled);
  printstatus(crewnames, crewstatus);
  printf("\nYou have %d gold pieces.\n", funds);

  // In the daily report, we need to include the amount of food the user has
  // Since it's impossible to have negative food, let the user know when they're out of food instead
  if(supplies[0] > 0)
    printf("You have %d pounds of food.\n", supplies[0]);
  else
    printf("You have no food.");

  printf("You are %d miles from your destination", (CANARY+GRENADA+FINAL+DISTANCE) - traveled);

}

//Pre-conditions:  a and b are both integers
//Post-conditions: the larger value will be returned
int max(int a, int b) {
  return a > b ? a : b;
}

//Pre-conditions:  a and b are both integers
//Post-conditions: the smaller value will be returned
int min(int a, int b) {
  return a < b ? a : b;
}

//Pre-conditions:  supplies is an array of integers representing how many of each type the crew has
//                 crew names is an array of strings for the crew members
//                 crewstatus is an array of numerical indicators for the status of each crew member
//                 0 - deceased, 1 - ill, 2 - healthy
//                 days represents the current day
//Post-conditions: the user will select a number of days to rest for. update days to reflect this number
//                 there is a small chance an ill crew member will recover during rest days

//What to do in this function: ask the user how many days they would like to rest for.
//  Updates days to indicate that that many days has now passed.  Deduct 2 pounds of food
//  for each crew member for each day rested.

//  Generate a pseudorandom value that will be either 0 or 1.  Generate a second value
//  between 0 and the number of original crew members.  If the first number is a 1,
//  the crew member in the index of the second number may recover if they are sick.
//  If they are healthy or deceased, nothing happens.  If the first number is a 0,
//  nothing happens.
void rest(int supplies[NUMSUPPLIES], char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW], int *days) {
  // Total tally of days spent resting
  int num_resting_days;
  // Ask the user to provide the number of days to rest
  printf("How many days would you like to rest for? \n");
  scanf("\n%d", &num_resting_days);

  // Add the number of days spent resting to the total number of days spent journeying
  *days += num_resting_days;

  // Calculate the amount of food needed to feed the crew, decrease supplies accordingly
  supplies[0] -= feed_crew(num_resting_days, 2, &crewstatus);

  // Generate a random number, check if the random number is 1
  if(rand() % 1 == 1){
    // Randomly choose a crew member
    int candidate = rand() % NUMCREW;

    // If the chosen crew member isn't dead, give them full health
    if(crewstatus[candidate] != 0)
      crewstatus[candidate] = 2;
  }

}

//Pre-conditions:  none
//Post-conditions: returns the number of pounds of fish gained by the ship

//What to do in this function: Generate a pseudorandom value between 0 and 3, inclusive.
//  Multiply this number by 50 and tell the user how many fish were caught.  Return
//  this value.
int fish() {
  // Randomly determine the amount of fish to catch, let the user know
  int fish_amount = (rand() % 3) * 50;
  printf("Your crew lowers the nets and pulls up %d pounds of fish.", fish_amount);
  return fish_amount;
}

//Pre-conditions:  crew names is an array of strings for the crew members
//                 crewstatus is an array of numerical indicators for the status of each crew member
//                 0 - deceased, 1 - ill, 2 - healthy
//                 days represents the current day
//                 supplies is an array of integers representing how many of each type the crew has
//Post-conditions: the status of a crew member or supplies may be affected by a random event,
//                 some events cause the ship to be delayed a certain number of days

//What to do in this function: Generate a pseudorandom number between 0 and 9, inclusive.  If the user
//  has run out of food, tell the user they have no food and increase your number by 2.  This number cannot
//  exceed 9.

//  Numbers 0, 1, and 2 do not correspond with any events.
//  Numbers 3 and 4 correspond with "positive events"
//      on a 3 the user will gain between 1 and 4 ship parts determined randomly
//      on a 4 the user will gain between 10 and 60 pounds of food determined randomly
//      both events take 1 day
//  Number 5 means the user will lose between 5 and 55 pounds of food
//      this event takes 1 day
//  Number 6 means the ship is stuck in fog, which takes 1 day
//  Number 7 means the ship is in a storm, which takes 2 days
//  Number 8 means something on the ship has broken
//      check the number of extra ship parts the crew has
//      if they have at least one ship part, they can replace it and continue
//      this takes 1 day
//      if they have no extra parts, they must stop and repair
//      this takes 3 days
//  Number 9 indicates sickness.  Generate a random number between 0 and the original
//      number of crew members.  Check the status of that crew member: if they are healthy,
//      they now fall ill.  If they are already ill, they now perish.  If they are already
//      deceased, nothing happens.

//  For any days spent in this way, deduct 2 pounds of food per crew member per day.
void event(char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW], int *days, int supplies[NUMSUPPLIES]) {
  // Randomly generate an "event code" between 0 and 9
  int event = rand() % 10;
  // Total tally of the days spent. This will be updated according to the event
  int days_spent = 0;

  // Let the user know whether they have food or not, for some reason
  if(supplies[0] < 1){
    printf("You have no food.\n");
    // Increment event code by 2 as per the spec
    if(event + 2 < 9)
      event += 2;
  }

  switch(event){
    case 3: { // Gain parts
      // Randomly determine the number of parts to bestow upon the player
      int gained_parts = (rand() % 4) + 1;
      printf("Another pirate ship pulls alongside and attacks!\nYou fend them off and take %d extra ship parts. You spend the day recovering.\n", gained_parts);
      // Add gained parts to total supply of parts in inventory
      supplies[2] += gained_parts;
      // Increment days spent by one
      days_spent++;
      break;
    }

    case 4: { // Gain food
      // Randomly determine amount of food to give
      int food_gained = (rand() % 6) * 10;
      printf("Another pirate ship pulls alongside and attacks!\nYou fend them off and take %d pounds of their food. You spend the day recovering.\n", food_gained);
      supplies[0] += food_gained; // Add food to inventory
      days_spent++;
      break;
    }

    case 5: { // Lose food
      // Determine the amount of food to steal from the user
      int food_lost = ((rand() % 5) * 10) + 5;
      printf("Another pirate ship pulls alongside and attacks!\nThey took %d pounds of food and you spend the day recovering.\n", food_lost);
      supplies[0] -= food_lost; // Remove food from inventory
      days_spent++;
      break;
    }

    case 6: { // Ship stuck in fog
      printf("Fog surrounds your ship.  Drop anchor for one day.\n");
      days_spent += 2; // Fog causes you to spend 2 days in limbo
      break;
    }

    case 7: { // Ship stuck in fog
      printf("An ocean storm batters your ship.  Drop anchor for two days.\n");
      days_spent++;
      break;
    }

    case 8: { // Broken parts
      printf("A part of your ship has broken!\n");

      if(supplies[2] > 0) {
        printf("You replace the broken part. It takes one day.\n");
        days_spent++;
      } else {
        printf("You have no replacement parts.  It takes three days to repair.\n");
        days_spent += 3;
      }

      break;
    }

    case 9: { // Illness
      // unlucky_index is the integer position of the unlucky crew member in the crew array
      int unlucky_index = (rand() % NUMCREW);
      // Create a pointer to the value of the unlucy crew member's health
      int* unlucky_person_health = &crewstatus[unlucky_index];

      // If the unlucky member isn't dead...
      if(*unlucky_person_health > 0) {
        // Decrement their health by 1
        *unlucky_person_health = *unlucky_person_health - 1;
      } else {
        break;
      }

      // Print the status of their health
      if(*unlucky_person_health == 0){
        printf("%s has died.\n", crewnames[unlucky_index]); // F
      } else {
        printf("%s has fallen ill.\n", crewnames[unlucky_index]);
      }

      break;
    }

    default:
      break;
  }

  // Determine the amount of food the crew will have consumed during the period the player spent
  // dealing with events. Then decrease the amount of food in their inventory by this amount
  supplies[0] -= feed_crew(days_spent, 2, &crewstatus);
  // Add the number of days spent dealing with events to the total number of days
  *days += days_spent;

}

// Helper function which calculates the total amount of food consumed by all living members of the crew.
int feed_crew(int num_days, int food_per_member, int* crewstatus){
  return countcrew(crewstatus) * food_per_member * num_days;
}
