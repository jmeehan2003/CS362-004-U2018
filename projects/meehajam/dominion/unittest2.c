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
	int numPlayers = 2;;
	int k[10] = {adventurer, gardens, embargo, village, minion,
							 mine, cutpurse, sea_hag, tribute, smithy};
	
	// create state
	struct gameState state;
	
	printf("\n***********************\n");
	printf("Testing initializeGame():\n");

	memset(&state,23,sizeof(struct gameState));
	
	printf("\nTEST 1: VALID RETURN VALUE RECEIVED\n");
	printf("RETURNING 0 INDICATES GAME INITIALIZED PROPERLY, RETURNING ANYTHING ELSE IS AN ERROR\n");
	assertTrue(initializeGame(numPlayers, k, seed, &state), 0);
	
	printf("\nTEST 2: CORRECT NUMBER OF PLAYERS\n");
	assertTrue(numPlayers, state.numPlayers);
	
	printf("\nTEST 3: CORRECT STARTING VICTORY AND CURSE CARD AMOUNTS\n");
	printf("CURSE: ");
	assertTrue(state.supplyCount[curse], 10);
	printf("ESTATE: ");
	assertTrue(state.supplyCount[estate], 8);
	printf("DUCHY: ");
	assertTrue(state.supplyCount[duchy], 8);
	printf("PROVINCE: ");
	assertTrue(state.supplyCount[province], 8);

	printf("\nTEST 4: CORRECT STARTING TREASURE AMOUNTS\n");
	printf("COPPER: ");
	assertTrue(state.supplyCount[copper], 60-(7*numPlayers));
	printf("SILVER: ");
	assertTrue(state.supplyCount[silver], 40);
	printf("GOLD: ");
	assertTrue(state.supplyCount[gold], 30);
	
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
