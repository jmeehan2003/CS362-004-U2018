#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// running total of test fails
int fails = 0;

// custom assert function
// checks the equivalency of two ints
void assertTrue(int a, int b);


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

    // game state
    struct gameState state, stateOriginal;
    printf("\n**************************\n");
    printf("Testing smithyCard()\n");
    memset(&state,23,sizeof(struct gameState));
    memset(&stateOriginal,23,sizeof(struct gameState));

    // create game state
    initializeGame(numPlayers, k, seed, &state);

    // preserve state
    memcpy(&stateOriginal, &state, sizeof(struct gameState));

    cardEffect(smithy, choice1, choice2, choice3, &state, handpos, &bonus);

    printf("\nTEST 1: PLAYER 1 GAINS 3 CARDS\n");
    printf("STARTING HAND COUNT: %d || EXPECTED HAND COUNT: %d\n", stateOriginal.handCount[player1], stateOriginal.handCount[player1]+2);
    assertTrue(state.handCount[player1],stateOriginal.handCount[player1]+2);

    printf("\nTEST 2: PLAYER 1's 3 CARDS COME FROM HIS OWN DECK\n");
    printf("STARTING DECK COUNT: %d || EXPECTED DECK COUNT: %d\n", stateOriginal.deckCount[player1], stateOriginal.deckCount[player1]-3);
    assertTrue(state.deckCount[player1],stateOriginal.deckCount[player1]-3);

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

    printf("\nTEST 5: PLAYER 2 WAS NOT AFFECTED\n");
    printf("PLAYER 2 HAND COUNT\n");
    assertTrue(state.handCount[player2],stateOriginal.handCount[player2]);
    printf("PLAYER 2 DECK COUNT\n");
    assertTrue(state.deckCount[player2],stateOriginal.deckCount[player2]);

    if (fails) 
        printf("\nTEST FAILED WITH %d FAILS\n", fails);
    else 
        printf("\nTEST SUCCESSFULLY COMPLETED\n\n");

    return 0;
}

void assertTrue(int a, int b) {
    if (a == b) {
        printf("TEST SUCCESSFULLY COMPLETED:  %d == %d\n", a, b);
    }
    else {
        printf("TEST FAILED:  %d != %d\n", a, b);
        fails++;
    }
}
