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

// runs the tests
int main () {
    int seed = 2000;
    int numPlayers = 2;
    int player1 = 0;
    int player2 = 1;
    int handpos = 0;
    int choice1 = 0, choice2 = 0, choice3 = 0;
    int bonus = 0;

    // kingdom cards
    int k[10] = {adventurer, council_room, feast, gardens, mine,
               remodel, smithy, village, baron, great_hall};

    struct gameState state, stateOriginal;

    printf("\n****************************\n");
    printf("Testing council_roomCard():\n");
    memset(&state,23,sizeof(struct gameState));
    memset(&stateOriginal,23,sizeof(struct gameState));

    // create game state
    initializeGame(numPlayers, k, seed, &state);

    // preserve game state
    memcpy(&stateOriginal, &state, sizeof(struct gameState));

    cardEffect(council_room, choice1, choice2, choice3, &state, handpos, &bonus);

    printf("\nTEST 1: PLAYER 1 GAINS 4 CARDS\n");
    printf("STARTING HAND COUNT %d || EXPECTED HAND COUNT: %d\n", stateOriginal.handCount[player1], stateOriginal.handCount[player1]+4);
    assertTrue(state.handCount[player1],stateOriginal.handCount[player1]+3);

    printf("\nTEST 2: PLAYER 1 GAINS 4 CARDS FROM HIS OWN DECK\n");
    printf("STARTING DECK COUNT %d || EXPECTED DECK COUNT: %d\n", stateOriginal.deckCount[player1], stateOriginal.deckCount[player1]-4);
    assertTrue(state.deckCount[player1],stateOriginal.deckCount[player1]-4);

    printf("\nTEST 3: PLAYER 1 NUMBER OF BUYS INCREASES BY 1\n");
    printf("STARTING BUYS %d || EXPECTED BUYS: %d\n", stateOriginal.numBuys, stateOriginal.numBuys+1);
    assertTrue(state.numBuys,stateOriginal.numBuys+1);

    printf("\nTEST 3: VICTORY CARD COUNTS REMAIN THE SAME\n");
    printf("STARTING PROVINCES: %d\n", stateOriginal.supplyCount[province]);
    assertTrue(state.supplyCount[province],stateOriginal.supplyCount[province]);
    printf("STARTING DUCHY: %d\n", stateOriginal.supplyCount[duchy]);
    assertTrue(state.supplyCount[duchy],stateOriginal.supplyCount[duchy]);
    printf("STARTING ESTATES: %d\n", stateOriginal.supplyCount[estate]);
    assertTrue(state.supplyCount[estate],stateOriginal.supplyCount[estate]);

    printf("\nTEST 4: NO CHANGE TO KINGDOM CARD COUNTS\n");
    for (int i = 0; i < 10; i++) {
        printf("CARD COUNT: \n");
        assertTrue(state.supplyCount[k[i]],stateOriginal.supplyCount[k[i]]);
    }

    printf("\nTEST 5: PLAYER 2 GAINS 1 CARD\n");
    printf("STARTING PLYR 2 HAND COUNT %d || EXPECTED PLYR 2 HAND COUNT: %d\n", stateOriginal.handCount[player2], stateOriginal.handCount[player2]+1);
    assertTrue(state.handCount[player2],stateOriginal.handCount[player2]+1);


    printf("\nTEST 6: PLAYER 2 GAINS A CARD FROM HIS OWN DECK\n");
    printf("STARTING PLYR 2 DECK COUNT %d || EXPECTED PLYR 2 DECK COUNT: %d\n", stateOriginal.deckCount[player2], stateOriginal.deckCount[player2]-1);
    assertTrue(state.deckCount[player2],stateOriginal.deckCount[player2]-1);

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
