#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

// global variables
int cardEffectFail = 0;
int discardFail = 0;
int drawFail = 0;
int cardCountFail = 0;

void smithyCardTest(int, struct gameState*); 

int main () 
{
    srand(time(NULL));
    printf("Smithy Card Random Test\n");

    int tests = 20000;
    int i, j, player;
    struct gameState game;
    

    // randomly initialize game state
    // SOURCE: This method to randomly initialize game state was discussed in the unofficial OSU 
    // student slack channel for CS362
    for (j = 0; j < tests; j++) 
    {
        for (i = 0; i < sizeof(struct gameState); i++) 
            ((char*)&game)[i] = floor(Random() * 256);

        // Select random values.  These values are not completely random but 
        // are random while still being valid within the game.
        player = floor(Random() * MAX_PLAYERS);
        game.deckCount[player] = floor(Random() * MAX_DECK);
        game.discardCount[player] = floor(Random() * MAX_DECK);
        game.handCount[player] = floor(Random() * MAX_HAND);
        game.playedCardCount = floor(Random() * (MAX_DECK-1));
        game.whoseTurn = player;
        
        // test Smithy Card
        smithyCardTest(player,&game);
    }
    
    int totalFails = cardEffectFail + discardFail + drawFail + cardCountFail;

    printf("Passed Tests: %d\n",tests - totalFails);
    printf("Failed Tests: %d\n",totalFails);

    if (totalFails == 0) 
        printf ("Passed Test\n\n");
    else 
    {
        printf("\nBug Report: \n");
        printf("DrawCard fails: %d\n",drawFail);
        printf("CardEffect fails: %d\n",cardEffectFail);
        printf("Discard fails: %d\n",discardFail);
        printf("Hand/Deck Count Fails: %d\n",cardCountFail);
    }

    return 0;
}

// check Smithy Card for faults
void smithyCardTest(int plyr, struct gameState *final) 
{
    int draw1, draw2, draw3;
    int cEffect, discardCall;
    int handCount, deckCount;
    int startHandCount, startDeckCount;

    struct gameState start;

    // get game state
    memcpy(&start,final,sizeof(struct gameState));
    int bonus = 0;

    // call card effect function with Smithy card
    cEffect = cardEffect(smithy,0,0,0,final,0,&bonus);

    // call draw card 3 times
    draw1 = drawCard(plyr,&start);
    draw2 = drawCard(plyr,&start);
    draw3 = drawCard(plyr,&start);

    discardCall = discardCard(0, plyr, &start, 0);

    // get hand/deck counts
    handCount = final->handCount[plyr];
    deckCount = final->deckCount[plyr];
    startHandCount = start.handCount[plyr];
    startDeckCount = start.deckCount[plyr];

    // check if drawcard failed
    if (draw1 == -1 && start.deckCount[plyr] != 0) 
        drawFail++;
    if (draw2 == -1 && start.deckCount[plyr] != 0) 
        drawFail++;
    if (draw3 == -1 && start.deckCount[plyr] != 0) 
        drawFail++;

    // check cardeffect and discardCard 
    if (!(cEffect == 0 && discardCall == 0)) 
    {
        if (discardCall) 
            discardFail++;
        if (cEffect) 
            cardEffectFail++;  
    }

    // check hand/deck counts
    if (!(handCount == startHandCount && deckCount == startDeckCount)) 
        cardCountFail++;
}
