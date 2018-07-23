/*****************************************
** Author: James Meehan
** Date: 7/22/18
** Description: CS362 - Random Testing
*******************************************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
  // get character from ascii table range 32 - 125
    int ch = rand() % 94 + 32;
    return ch;
}

char *inputString()
{
  // fixed length string of 5 characters + null terminator
	static char myString[6];
	
  for (int i = 0; i < 5; i++)
  {
      // input limited to letters used in the target statement
      int randNum = rand() % 4 + 1;
      switch (randNum) 
      {
        case 1: myString[i] = 'r';
                  break;
        case 2: myString[i] = 'e';
                  break;
        case 3: myString[i] = 's';
                  break;
        case 4: myString[i] = 't';
                  break;
        }
  }
	myString[5] = '\0';

  return myString;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();

    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
