#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "rngs.h"

// global count of test failures
int fails = 0;

// custom assert function
// checks the equivalency of two ints
void assertTrue(int a, int b);

int main () {
	// randomly select two or 3 players
	int seed = 2000;	
	int numPlayers = 2;
	int k[10] = {adventurer, gardens, embargo, village, minion,
		mine, cutpurse, sea_hag, tribute, smithy};
	struct gameState state;

	printf("\n***********************\n");
	printf("Testing numHandCards():\n");
	printf("ONLY ONE PLAYER SHOULD HAVE CARDS IN THEIR HAND AT ONE TIME\n");
	memset(&state,23,sizeof(struct gameState));

	// create a state
	initializeGame(numPlayers, k, seed, &state);
	
	printf("\nTEST 1: CORRECT NUMCARDS TURN WHEN WHOSETURN IS 0\n");
	state.whoseTurn = 0;
	assertTrue(numHandCards(&state), state.handCount[0]);
	
	printf("\nTEST 2: CORRECT NUMCARDS TURN WHEN WHOSETURN IS 1\n");
	state.whoseTurn = 1;
	assertTrue(numHandCards(&state), state.handCount[1]);
	
	//printf("\nTEST 3: CORRECT PLAYERS TURN WHEN WHOSETURN IS 2\n");
	state.whoseTurn = 2;
	assertTrue(numHandCards(&state), state.handCount[2]);

	// only execute if there are four players
	if (numPlayers == 4)
	{	
		printf("\nTEST 4: CORRECT PLAYERS TURN WHEN WHOSETURN IS 3\n");
		state.whoseTurn = 3;
		assertTrue(numHandCards(&state), state.handCount[3]);
	}

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
