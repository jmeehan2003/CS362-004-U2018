#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"


// running total of test failures
int fails = 0;

// custom assert function
// checks the equivalence of two integers
void assertTrue(int a, int b);

int main () {
    int seed = 2000;
    int treasure = 0;
    int treasureCountPrev = 0;
    int numPlayers = 2;
    int player1 = 0;
    int player2 = 1;
    int card;
    int handpos = 0;
    int choice1 = 0, choice2 = 0, choice3 = 0;
    int bonus = 0;

    // kingdom cards
    int k[10] = {adventurer, council_room, feast, gardens, mine,
               remodel, smithy, village, baron, great_hall};


    // game states
    struct gameState state, stateOriginal;

    printf("\n***********************\n");
    printf("Testing adventurerCard()\n");
    memset(&state,23,sizeof(struct gameState));
    memset(&stateOriginal,23,sizeof(struct gameState));

    // create a state
    initializeGame(numPlayers, k, seed, &state);

    // preserve the state
    memcpy(&stateOriginal, &state, sizeof(struct gameState));

    cardEffect(adventurer, choice1, choice2, choice3, &state, handpos, &bonus);

    printf("\nTEST 1: PLAYER 1 RECEIVES 2 CARDS\n");
    printf("STARTING HAND COUNT: %d || EXPECTED NEW HAND COUNT %d\n", stateOriginal.handCount[player1], state.handCount[player1]); 
    assertTrue(state.handCount[player1],stateOriginal.handCount[player1]+2);

    // checks treasure
    for (int i = 0; i < state.handCount[player1]; i++) {
        card = state.hand[player1][i];
        if (card == copper || card == silver || card == gold) {
            treasure++;
        }
    }

    // checks original treasure amount
    for (int i = 0; i < stateOriginal.handCount[player1]; i++) {
        card = stateOriginal.hand[player1][i];
        if (card == copper || card == silver || card == gold) {
            treasureCountPrev++;
        }
    }

    printf("\nTEST 2: PLAYER 1 GETS 2 TREASURE CARDS\n");
    printf("STARTING TREASURE: %d || EXPECTED NEW TREASURE %d\n", treasureCountPrev, treasure); 
    assertTrue(treasureCountPrev+2,treasure);

    printf("\nTEST 3: VICTORY CARD COUNTS REMAIN THE SAME\n");
    printf("STARTING PROVINCES: %d\n", stateOriginal.supplyCount[province]);
    assertTrue(state.supplyCount[province],stateOriginal.supplyCount[province]);
    printf("STARTING DUCHY: %d\n", stateOriginal.supplyCount[duchy]);
    assertTrue(state.supplyCount[duchy],stateOriginal.supplyCount[duchy]);
    printf("STARTING ESTATES: %d\n", stateOriginal.supplyCount[estate]);
    assertTrue(state.supplyCount[estate],stateOriginal.supplyCount[estate]);

    printf("\nTEST 4: NO CHANGES TO KINGDOM CARD COUNTS\n");
    for (int i = 0; i < 10; i++) {
        printf("CARD COUNTS: \n");
        assertTrue(state.supplyCount[k[i]],stateOriginal.supplyCount[k[i]]);
    }

    printf("\nTEST 5: PLAYER 2 WAS NOT AFFECTED\n");
    printf("HAND COUNT: ");
    assertTrue(state.handCount[player2],stateOriginal.handCount[player2]);
    printf("DECK COUNT: ");
    assertTrue(state.deckCount[player2],stateOriginal.deckCount[player2]);

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
