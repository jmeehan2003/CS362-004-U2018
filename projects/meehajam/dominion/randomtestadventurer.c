#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

//global variables
int cardEffectFail = 0;
int shuffleFail = 0;
int drawFail = 0;
int cardCountFail = 0;
int treasureCountFail = 0;

void adventurerTest(int, struct gameState*);

int main () 
{
    srand(time(NULL));
    printf("Adventurer Card Random Test\n");
    int tests = 20000;
    
    int numTreasures;
    int i, n, player;
    struct gameState game;
    int treasures[] = {copper,silver,gold};

    // should be at least 3 cards in the deck and each hand
    int minCards = 3;
    
    // randomly initialized the game state
    for (n = 0; n < tests; n++) 
    {
      // SOURCE: This method to randomly set the gamestate was discussed on the unofficial OSU student CS362 slack channel  
      for (i = 0; i < sizeof(struct gameState); i++) 
      {
        ((char*)&game)[i] = floor(Random() * 256);
      }

      // randomly select values.  Values are not completely random but are values that 
      // are valid within the game
      player = floor(Random() * MAX_PLAYERS);
      game.deckCount[player] = floor(Random() * ((MAX_DECK - minCards) + 1) + minCards);
      numTreasures = floor(Random() * ((game.deckCount[player] - minCards) + 1) + minCards);

      // make sure each player gets at least treaures cards
      for (i = 0; i < numTreasures; i++) 
      {
        game.deck[player][i] = treasures[rand() % 3];
      }

      game.discardCount[player] = 0;
      game.handCount[player] = floor(Random() * ((MAX_HAND - minCards) + 1) + minCards);
      game.whoseTurn = player;

      // test the function
      adventurerTest(player, &game);
    }

    int totalFails = cardEffectFail + drawFail + shuffleFail +
                    cardCountFail + treasureCountFail;

    if (tests - totalFails <= 0) 
    {
      printf("Passed Tests: %d\n",0);
      printf("Failed Tests: %d\n",tests);
    }
    else 
    {
      printf("Passed Tests: %d\n",tests - totalFails);
      printf("Failed Tests: %d\n",totalFails);
    }

    if (totalFails == 0) 
    {
        printf ("Random Test Passed: 0 fails\n\n");
    }
    else 
    {
        printf("\nBug Report:\n");
         printf("Shuffle failed: %d\n",shuffleFail);
        printf("DrawCard failed: %d\n",drawFail);
        printf("Hand and/or Deck Count failed: %d\n",cardCountFail);
        printf("CardEffect failed: %d\n",cardEffectFail);
        printf("Treasure Count failed: %d\n",treasureCountFail);
        
    }
    return 0;
}

// adventurerCard random tester
void adventurerTest(int plyr, struct gameState *final) {
    int i, t, j = 0, bonus = 0;
    int treasureEnd = 0, treasureStart = 0;
    int temphand[MAX_HAND];
    int treasuresDrawn = 0;
    struct gameState start;
    int cEffect, shuffler;
    int cardDrawn;
    int card;
    int preHandCount, preDiscard, preDeckCount;
    int handCount, deckCount, discardCount; 

    // get gamestate
    memcpy(&start,final,sizeof(struct gameState));

    // call card effect with adventurerCard
    cEffect = cardEffect(adventurer,0,0,0,final,0,&bonus);

    // check if cardeffect failed
    if (cEffect) 
    {
        cardEffectFail++;
    }

    while(treasuresDrawn < 2) 
    {
          
            // check if deck is empty
            if (start.deckCount[plyr] < 1) 
            {
                    shuffler = shuffle(plyr, &start);
                    // check shuffle 
                    if (shuffler == -1 && start.deckCount[plyr] >= 1) {
                      shuffleFail++;
                    }
            }
            t = drawCard(plyr, &start);

            // check draw
            if (t == -1 && start.deckCount[plyr] != 0) 
            {
                drawFail++;
            }

            // check treaures/deck/hand count
            cardDrawn = start.hand[plyr][start.handCount[plyr] - 1];
            if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold)
                    treasuresDrawn++;
            else{
                    temphand[j] = cardDrawn;
                    start.handCount[plyr]--; 
                    j++;
            }
    }


    while(j - 1 >= 0) 
    {
            start.discard[plyr][start.discardCount[plyr]++] = temphand[j - 1]; 
            j = j - 1;
    }



    // get starting treasure count
    for (i = 0; i < start.handCount[plyr]; i++) 
    {
        card = start.hand[plyr][i];
        if (card == copper || card == silver || card == gold) 
        {
            treasureStart++;
        }
    }

    // get final treaure count
    for (i = 0; i < final->handCount[plyr]; i++) 
    {
        card = final->hand[plyr][i];
        if (card == copper || card == silver || card == gold) 
        {
            treasureEnd++;
        }
    }

    // get starting treasure count
    for (i = 0; i < start.handCount[plyr]; i++) 
    {
        card = start.hand[plyr][i];
        if (card == copper || card == silver || card == gold) 
        {
            treasureStart++;
        }
    }

    // verify final treasure count equal starting treasure count
    if (treasureEnd != treasureStart) 
    {
      treasureCountFail++;
    }
    // get starting hand/deck/discard counts
    preHandCount = start.handCount[plyr];
    preDeckCount = start.deckCount[plyr];
    preDiscard = start.discardCount[plyr];

    // get final hand/deck/discard/counts
    handCount = final->handCount[plyr];
    deckCount = final->deckCount[plyr];
    discardCount = final->discardCount[plyr];

    // verify card counts match up
    if (!(handCount == preHandCount && deckCount == preDeckCount && discardCount == preDiscard)) 
    {
        cardCountFail++;
    }
}
