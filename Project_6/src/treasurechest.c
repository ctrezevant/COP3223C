/*
  COP 3223C - Project 6
  Charlton Trezevant - 2017
  Professor Sarah Angell

  Assignment #6 (treasurechest.c)
*/

// "Hey Parents, are you a header file?
// Because I'm totally dependent on you"
#include <stdio.h>
#include <stdlib.h>

// You can globally set the number of keys used throughout the program here,
// because only Satan and interns use magic numbers.
const int NUM_KEYS = 7;

// Helper function prototypes
// For descriptions of each helper's purpose, check their respective declarations
int array_has_duplicates(int arr[], int arr_sz);
int arrays_are_equal(int check[], int ref[], int arrays_sz);
int array_contains_value(int arr[], int arr_sz, int val);
int total_correct_values(int check[], int ref[], int arrays_sz);

int main(){
  // CHEST_COMBO holds the combination to the chest,
  // as read from the key file
  int CHEST_COMBO[NUM_KEYS];
  // USER_COMBO is eventually populated with the user's supplied
  // key combination
  int USER_COMBO[NUM_KEYS];

  // FILE pointer to (surprise!) the file with the chest combo in it
  FILE *combo_file;

  // Maxlen of the file path wasn't given in the project spec
  // So I'll alloc 40 characters to the user, because I'm feeling
  // extra generous today.
  char file_path[40];

  // "Is the chest locked right now?"
  // "I dunno man, ask chest_locked"
  // (hint: chest_locked keeps track of the state of the chest)
  int chest_locked = 1;

  // Retrieve the path of the combo file from the user
  printf("\nWhat is the name of the file? \n");
  scanf(" %s", file_path);

  // Now that we have the path to our key file, we'll try to load it and read from it
  // Otherwise we're just going to crash and burn
  // Anyone who knowingly gives an invalid file path to a poor, hapless little
  // C program is obviously a monster, anyways.
  combo_file = fopen(file_path, "r");

  // After calling fopen above, we can start reading values from the key file
  // That way we can take the master list of keys and copy them into CHEST_COMBO
  for(int i = 0; i < NUM_KEYS; i++){
    fscanf(combo_file, "%d", &CHEST_COMBO[i]);
  }

  // Call fclose, because we're done reading from the key file
  fclose(combo_file);

  printf("-------------------------------\n");
  printf("To get to me treasure you'll have to figure out which of me 100 keys are usedin the %d locks of me treasure chest.\n", NUM_KEYS);

  // Our main loop, where users will give us keys and we'll check 'em
  while(chest_locked == 1){

    printf("\nWhich keys will ye use?\n");
    // Scan in however many keys the user is going to supply to us,
    // and store their combination in USER_COMBO
    for(int i = 0; i < NUM_KEYS; i++)
      scanf("\n %d", &USER_COMBO[i]);

    // First things first: let's see if the user got any of their key guesses right
    // So we'll grab the total number of correct values guessed, first
    int num_correct_keys = total_correct_values(CHEST_COMBO, USER_COMBO, NUM_KEYS);

    // The next case the spec asked us to handle was one in which there were duplicate keys
    // So we've gotta check those dubs before anything else happens
    if(array_has_duplicates(USER_COMBO, NUM_KEYS) == 1){

      // I subconsciously read all of these messages in the voice of Mr.Krabs. It's been this way for every single project
      // I don't know why my subconscious didn't pick a cooler character, like Capt. Jack Sparrow or something
      // But no, my only company while coding these assignemnts is an anthromorphized cartoon crab
      printf("You can only use each key once, matey!\n");

    // Assuming there are no duplicates, the next thing to check for is whether or not the keys are in
    // the right order. An obvious precondition to this that the user has to have guessed at least one
    // of the keys correctly
    } else if(num_correct_keys > 0 && arrays_are_equal(CHEST_COMBO, USER_COMBO, NUM_KEYS) == 1) {

      printf("Arr! You've opened me treasure chest and found...\nA map! To the rest of me treasure on Treasure Island.\nHahaha!\n");
      // "Unlock" the chest
      // This breaks us out of the main loop so that your CPU
      // can spin on something more worthwhile, like your Facebook homepage
      chest_locked = 0;

    // So there aren't any duplicates, and the user clearly guessed at least some of the keys correctly
    // Let's inform them of that
    } else if(num_correct_keys > 0){

      printf("%d of those keys are correct, matey!  But are they in the right order?\n", num_correct_keys);

    // But oh no, what if they didn't guess *any* of the keys correctly?!
    } else {

      // Putting on my best pirate impersonation, here, as no scripted response for this case was provided.
      printf("None of those keys are correct! Try 'yer hand again before I throw ye off the plank. Arrr!\n");

    }

  }

  // 0 is best exit code~
  return 0;
}

// Here there be helper functions
// Like, the next 33 LoC -> EOF are just array-related helpers I've written
// (Thanks for keeping things primitive, Dennis)
// I'd normally put this in its own header file, but not when submitting work


/*
 * Function:  array_has_duplicates
 * --------------------
 * Checks the given array for duplicate values
 *
 *  arr: an array of integers to check
 *  arr_sz: the size of the array being passed in (we can't compute this
 *  inside of the function scope, since args are only pointers)
 *
 *  returns: 1 if duplicates were found, 0 otherwise
 */
int array_has_duplicates(int arr[], int arr_sz){
  for(int i = 0; i < arr_sz; i++)
    for(int j = 0; j < arr_sz; j++)
      if(arr[i] == arr[j] && i != j)
        return 1;

  return 0;
}

/*
 * Function:  arrays_are_equal
 * --------------------
 * Compares a given array against a reference array, ensuring that the two are
 * exactly equal
 *
 *  check: an array of integers to check
 *  ref: the array of integers to be used as a reference
 *  arrays_sz: the integer size of the two arrays
 *
 *  returns: 1 if check is ordered properly relative to ref, 0 otherwise
 */
int arrays_are_equal(int check[], int ref[], int arrays_sz){
  for(int i = 0; i < arrays_sz; i++)
    if(check[i] != ref[i])
      return 0;

  return 1;
}

/*
 * Function:  array_contains_value
 * --------------------
 * Searches a given array for a given value, lets us know if the value was found
 *
 *  arr: an array of integers to search through
 *  arr_sz: the integer size of the array being passed in
 *  val: the integer value you want to search arr for
 *
 *  returns: 1 if the given array does, in fact, contain the specified value. 0
 *  if otherwise
 */
int array_contains_value(int arr[], int arr_sz, int val){
  for(int i = 0; i < arr_sz; i++)
    if(arr[i] == val)
      return 1;

  return 0;
}

/*
 * Function:  total_correct_values
 * --------------------
 * Computes the total number of shared values between two given arrays.
 * Put into the context of Project 6, we can use this to determine the total
 * number of correct guesses that the user supplied.
 *
 * **Note: This function does not consider the order of those values whatsoever
 *
 *  check: an integer array containing the user's guesses
 *  ref: an integer reference array (e.g. CHEST_COMBO)
 *  arrays_sz: the integer sizes of the two arrays being passed in
 *
 *  returns: integer total amount of shared values between the two arrays
 */
int total_correct_values(int check[], int ref[], int arrays_sz){
  int num_correct = 0;

  for(int i = 0; i < arrays_sz; i++)
    if(array_contains_value(ref, arrays_sz, check[i]) == 1)
      num_correct++;

  return num_correct;
}
