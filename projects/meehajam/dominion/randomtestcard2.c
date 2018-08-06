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
int buyFail = 0;
int opponentHandFail = 0;
int cardEffectFail = 0;
int discardFail = 0;
int drawFail = 0;
int cardCountFail = 0;

void testCouncilRoomCard(int, struct gameState*);

int main () 
{
    srand(time(NULL));
    printf("Council Room Card Random Test\n");

    int tests = 20000;
    int i, j, player, deckCount, handCount, discardCount;
    int numberOfPlayers[] = {2,3,4};
    struct gameState game;
    
    // randomly initialized the game state
    for (j = 0; j < tests; j++) 
    {
        // SOURCE: This method to randomly initialize game state was discussed in the unofficial OSU 
        // student slack channel for CS362   
        for (i = 0; i < sizeof(struct gameState); i++) 
            ((char*)&game)[i] = floor(Random() * 256);

        // Select random values.  These values are not completely random but 
        // are random while still being valid within the game.
        game.numPlayers = numberOfPlayers[rand() % 3];
        game.numBuys = 1;
        game.playedCardCount = floor(Random() * (MAX_DECK-1));
        player = game.numPlayers - 2;
        deckCount = floor(Random() * MAX_DECK);
        handCount = floor(Random() * MAX_HAND);
        discardCount = floor(Random() * MAX_DECK);
        game.whoseTurn = player;

        for (i = 0; i < game.numPlayers; i++) 
        {
            game.deckCount[i] = deckCount;
            game.handCount[i] = handCount;
            game.discardCount[i] = discardCount;
        }

        // test Council Room
        testCouncilRoomCard(player,&game);
    }
    
    int totalFails = cardEffectFail + discardFail + drawFail + cardCountFail + buyFail;
 
    printf("Passed Tests: %d\n",tests - totalFails);
    printf("Failed Tests: %d\n",totalFails);

    if (totalFails == 0) 
        printf ("Passed Test\n\n");
    else 
    {
        printf("\nBug Report: \n");
        printf("DrawCard fails: %d\n",drawFail);
        printf("CardEffect fails: %d\n",cardEffectFail);
        printf("DiscardCard fails: %d\n",discardFail);
        printf("Buy Count fails: %d\n",buyFail);
        printf("Opponent Hand/deck Count fails: %d\n",opponentHandFail);
        printf("Player Hand/deck Count fails: %d\n",cardCountFail);
    }

    return 0;
}

// test Council Room
void testCouncilRoomCard(int plyr, struct gameState *final) {
    int draw1, draw2, draw3, draw4, drawVal;
    int cEffect, discards;
    int handCount, deckCount;
    int startHandCount, startDeckCount;
    int i, bonus = 0;

    struct gameState start;

    // get gameState
    memcpy(&start,final,sizeof(struct gameState));

    // call card effect with council room
    cEffect = cardEffect(council_room,0,0,0,final,0,&bonus);

    // draw 4 cards
    draw1 = drawCard(plyr,&start);
    draw2 = drawCard(plyr,&start);
    draw3 = drawCard(plyr,&start);
    draw4 = drawCard(plyr,&start);

    start.numBuys++;

    for (i = 0; i < start.numPlayers; i++) 
    {
        if (i != plyr) 
        {
            drawVal = drawCard(i,&start);
            if (drawVal == -1 && start.deckCount[i] != 0) 
                drawFail++;
        }
    }

    // call discardCard
    discards = discardCard(0, plyr, &start, 0);

    // get values of hand and deck counts
    handCount = final->handCount[plyr];
    deckCount = final->deckCount[plyr];
    startHandCount = start.handCount[plyr];
    startDeckCount = start.deckCount[plyr];

    // check buys
    if (start.numBuys != final->numBuys) 
        buyFail++;

    // check draws
    if (draw1 == -1 && start.deckCount[plyr] != 0) 
        drawFail++;
    if (draw2 == -1 && start.deckCount[plyr] != 0) 
        drawFail++;
    if (draw3 == -1 && start.deckCount[plyr] != 0) 
        drawFail++;
    if (draw4 == -1 && start.deckCount[plyr] != 0) 
        drawFail++;

    // check cardeffect and discard
    if (!(cEffect == 0 && discards == 0)) {
        if (discards) 
            discardFail++;
        if (cEffect) 
            cardEffectFail++;
    }

    // check if the hand and deck counts dont match up
    if (!(handCount == startHandCount && deckCount == startDeckCount)) 
        cardCountFail++;

    // check if the other players hand and deck counts match
    for (i = 0; i < start.numPlayers; i++) 
    {
        if (i != plyr) 
        {
            if (!(final->handCount[i] == start.handCount[i] &&
                  final->deckCount[i] == start.deckCount[i])) 
                      opponentHandFail++;   
        }
    }
}
