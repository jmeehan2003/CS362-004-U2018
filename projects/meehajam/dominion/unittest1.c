#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// running total of test fails
int fails; 

// custom assert function
// checks the equivalency of two ints
void assertTrue(int a, int b);


int main () {
    int seed = 2000;
    int numPlayers = 2;

    // kingdom cards
    int k[10] = {adventurer, council_room, feast, gardens, mine,
               remodel, smithy, village, baron, great_hall};

    // create game state    
    struct gameState state;

    printf("\n********************************\n");
    printf("Testing isGameOver():\n");
    printf("GAME IS OVER WHEN PROVINCE CARDS = 0 OR THE THREE SUPPLY PILES = 0\n");
    printf("FUNCTION SHOULD RETURN 1 IF GAME IS OVER AND RETURN 0 IF IT IS NOT\n");
    memset(&state,23,sizeof(struct gameState));
    initializeGame(numPlayers, k, seed, &state);

    state.supplyCount[province] = 0;

    printf("\nTEST 1: PROVINCE CARDS EMPTY\n");
    assertTrue(isGameOver(&state),1);

    state.supplyCount[province] = 1;

    state.supplyCount[0] = 0;
    state.supplyCount[1] = 0;
    state.supplyCount[2] = 0;

    printf("\nTEST 2: SUPPLY PILES EMPTY\n");
    assertTrue(isGameOver(&state),1);

    state.supplyCount[0] = 1;
    state.supplyCount[1] = 1;
    state.supplyCount[2] = 1;

    printf("\nTEST 3: SUPPLY AND PROVINCE CARDS NOT EMPTY\n");
    assertTrue(isGameOver(&state),0);

    if (fails) 
        printf("\nTEST FAILED WITH %d FAILS\n", fails);
    else 
        printf("\nTEST SUCCESSFULLY COMPLETED\n");

    return 0;
}

void assertTrue(int a, int b) {
    if (a == b) {
        printf("TEST SUCCESSFULLY COMPLETED: %d == %d\n", a, b);
    }
    else {
        printf("TEST FAILED: %d != %d\n", a, b);
        fails++;
    }
}
