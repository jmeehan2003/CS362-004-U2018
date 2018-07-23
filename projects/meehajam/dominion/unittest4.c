#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// global count of test failures
int fails = 0;

// custom assert function
// checks the equivalency of two ints
void assertTrue(int a, int b);

int main () {
    int seed = 2000;
    int numPlayers = 2;
    int player = 0;

    // kingdom cards
    int k[10] = {adventurer, council_room, feast, gardens, mine,
               remodel, smithy, village, baron, great_hall};
    
    struct gameState state;

    printf("\n***********************\n");
    printf("Testing shuffle()\n");
    memset(&state,23,sizeof(struct gameState));
    initializeGame(numPlayers, k, seed, &state);

    state.deckCount[player] = 0;
    printf("\nTEST 1: PLAYER DECK COUNT IS 0\n");
    assertTrue(shuffle(player,&state),-1);

    printf("\nTEST 2: PLAYER DECK COUNT REMAINS AT 10 AFTER SHUFFLE\n");
    state.deckCount[player] = 10;
    shuffle(player,&state);
    assertTrue(state.deckCount[player],10);

    printf("\nTEST 3: CARDS ARE ACTUALLY SHUFFLED\n");
    int preShuffle = state.deck[player][0];
    shuffle(player,&state);
    int postShuffle = state.deck[player][0];
    if (preShuffle != postShuffle) 
        assertTrue(1,1);
    else 
        assertTrue(0,1);

    if (fails) 
        printf("\nTEST FAILED WITH %d FAILS\n", fails);
    else 
        printf("\nTEST SUCCESSFULLY COMPLETED\n\n");

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
