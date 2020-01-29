/*
    This program simulates a black jack game with the user playing the computer. The player
    will go first and can draw cards until they bust (go over 21). Then the computer goes
    and will draw until it reaches the value of 15. When the cards run out, the program will
    generate a new deck a shuffle it. The player and computer start with 10 cherries and bet
    one each time until there is a winner who gets all 20. The user can choose to play another
    game but can also choose to quit after any round.
    Author: David Onak & Zahra Manochehri
    Date : Nov/18/2018
*/

#include <stdio.h>
#include <stdlib.h>

/*prototypes*/
void displayScreen(int, int, int, int, int [], int []);
int playerTurn(int, int, int, int, int [], int [], int[]);
int compTurn(int, int, int, int, int [], int [], int[]);
void displayCard(int);
int overalWin(int, int);
int roundWin(int, int);
char makeChoice(int);
void clean(void);
void openDeck(int []);
void shuffleDeck (int []);
int randomNum(int, int);
int drawCard(int[], int[], int);

/*constants for card holding sizes*/
#define HANDSIZE 10
#define DECKSIZE 52

int main()
{
    int win = 0; //0 if no one won, 1 when a player has all the cherries
    char choice = '1'; //1 is to contunou playing, while 0 is choice to end game
    int pCherries = 10;
    int cCherries = 10;
    int pScore = 0; //card value score for human
    int cScore = 0; //card value score for computer
    int pCardV[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //cards the human has drawn
    int cCardV[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //cards the computer has drawn
    int deck[DECKSIZE];
    int i;//in for loop for cleaning hands

    srand((unsigned)time (NULL));

    printf("Welcome to the super Black Jack game!\n\n");
    printf("The goal is to get your value of cards as close to 21 as possible without going over!\n");
    printf("To win a round you must have a higher total value than the computer or the computer goes\nover 21 while not going over.");
    printf("You and the computer start with 10 cherries and one cherry is bet into\neach round, to win the game get all 20 cherries!\n");
    printf("An ace is worth 1, number cards are worth their face value, and jack, queen, king are worth 10.\n\n");
    printf("Press enter when your ready to begin!");
    clean();//pause game

    openDeck(deck);
    shuffleDeck(deck);

    do{
        displayScreen(pCherries, cCherries, pScore, cScore, pCardV, cCardV);
        printf(" Press enter to draw your first card\n");
        clean();//pause game

        pScore = playerTurn(pCherries, cCherries, pScore, cScore, pCardV, cCardV, deck);
        cScore = compTurn(pCherries, cCherries, pScore, cScore, pCardV, cCardV, deck);

        if (roundWin(pScore, cScore) == 0)//if coumputer wins
        {
            cCherries++;
            pCherries--;
        }
        else if (roundWin(pScore, cScore) == 1)//if player wins
        {
            cCherries--;
            pCherries++;
        }

        displayScreen(pCherries, cCherries, pScore, cScore, pCardV, cCardV);
        win = overalWin(pCherries, cCherries);

        choice = makeChoice(roundWin(pScore, cScore));

        pScore = 0;
        cScore = 0;

        for(i = 0; i < HANDSIZE; i ++)
        {
            pCardV[i] = 0;
            cCardV[i] = 0;
        }

    }while(win == 0 && choice == '1');

    if (win == 1)//the human player won the game
        printf(" You have won the game!!! Press enter to end game.\n");
    else if (win == 2)//the computer has won the game
        printf(" Unfortunatly you have lost!\n Press enter to end game.\n");

    clean();//pause

    system("cls");
    printf("Thank you for playing, please do return!\n");

    return 0;
}

/*********************************************************************
 * displayScreen(int, int, int, int, int[], int[])
 *		This function displays the full display screen which is a table
 *      that shows the player's and computer's cherries, cards drawn,
 *      and total value as score.
 * PRE: Player's and computer's cherries, cards, and score.
 * POST: no output returned, prints full display screen.
 *********************************************************************/
void displayScreen(int pC, int cC, int pS, int cS, int pCar[], int cCar[])
{
    /*clear the screen*/
    system("cls");

    int i; //for rows to display of cards

    printf(" Human: %d(cherries)  Computer: %d(cherries)\n\n ", pC, cC);

    /*adding cards to display*/
    for(i = 0; cCar[i] != 0 || pCar[i] != 0; i++)
    {
        if (pCar[i] == 0)
            printf("   ");
        else
            displayCard(pCar[i]);
        printf("\t\t\t\t");
        if (cCar[i] == 0)
            printf("   ");
        else
            displayCard(cCar[i]);
        printf("\n ");
    }

    printf("\n Score: %d\t\t\tScore: %d\n\n", pS, cS);
}

/*********************************************************************
 * displayCard(int)
 *		This function displays the face of a card based on the value
 *      it contains from the deck, (type and suit).
 * PRE: Gets a value from the deck and prints corrisponding card face.
 * POST: no output returned, prints out face of a card.
 *********************************************************************/
void displayCard(int card)
{
    /*print the card type*/
    if ((card%13) == 0)
        printf("K");
    else if ((card%13) == 1)
        printf("A");
    else if ((card%13) < 11)//case for all number face cards
        if ((card/13) == 0)
            printf("%d", card);
        else if ((card/13) == 1)
            printf("%d", card-13);
        else if ((card/13) == 2)
            printf("%d", card-26);
        else
            printf("%d", card-39);
    else if ((card%13) == 11)
        printf("J");
    else
        printf("Q");

    /*print the card suite*/
    if (((float)card/13) <= 1)
        printf("C");
    else if (((float)card/13) <= 2)
        printf("D");
    else if (((float)card/13) <= 3)
        printf("H");
    else
        printf("S");
}

/*********************************************************************
 * int roundWin(int, int)
 *		This function checks to see who won the round based on received
 *      scores.
 * PRE: Gets the score from player and computer.
 * POST: Returns the outcome, 0 for computer win, 1 for player win and
 *       2 for a tie.
 *********************************************************************/
int roundWin(int pS, int cS)
{
    int result = 2;//value to is for tie

    /*win cases*/
    if (pS > cS)//player win case
        result = 1;
    else if (cS > pS)//computer win case
        result = 0;

    return result;
}

/*********************************************************************
 * int overalWin(int, int)
 *		This function checks to see who won the game based on received
 *      number of cherries from player and computer.
 * PRE: Gets the cherry numbers from player and computer.
 * POST: Returns the winner(int), 0 for none yet, 1 for human, 2 for computer.
 *********************************************************************/
int overalWin(int pC, int cC)
{
    int gWin = 0;

    if (pC == 20)//human won the game
        gWin = 1;
    else if (cC == 20)//computer won the game
        gWin = 2;

    return gWin;
}

/*********************************************************************
 * char makeChoice(int, int)
 *		This function announces any winner for game or round and gives
 *      the choice to contioue or quit.
 * PRE: Gets winner results from round and game.
 * POST: Returns the choice(char), '1' to continoue and '0' to quit.
 *********************************************************************/
char makeChoice(int rWin)
{
    char choose = '1';//value 1 is for yes to continouing

    /*send print messages*/
    switch(rWin)
    {
        case 0: printf(" The computer has won this round, press 1 if you would like\n");
                printf(" to continoue playing or 0 if you want to quit\n");
                break;
        case 1: printf(" You have won this round!!! Press 1 if you would like to\n");
                printf(" continoue playing or 0 if you want to quit\n");
                break;
        default: printf(" This round is a tie!!! Press 1 if you would like to\n");
                printf(" continoue playing or 0 if you want to quit\n");
    }

    /*get input and check for error*/
    scanf("%c", &choose);
    clean();//clean out extra chars
    while (choose != '0' && choose != '1')
    {
        printf(" Sorry that is not a valid anwser, please try again.\n");
        scanf("%c", &choose);
        clean();//clean out extra chars
    }

    return choose;
}

/*********************************************************************
 * clean()
 *		Cleans input string when a choice has extra characters attached
 *      and allows the program to pause until enter is pressed other
 *      times when called.
 * PRE: no input.
 * POST: no output return.
 *********************************************************************/
void clean (void)
{
    char dummy;

    do
    {
        /* read each characters seprate */
        scanf("%c",&dummy);
    }
    while(dummy != '\n');  /* if the user enters this function will stop */
}

/*********************************************************************
 * shuffleDeck(int[])
 *		This function receives a new deck and shuffles it.
 * PRE: Gets the new deck.
 * POST: no output return, deck is changed to be shuffled.
 *********************************************************************/
void shuffleDeck (int deck[])
{
    int counter;
    int index;
    int temp;

    for (counter = DECKSIZE-1; counter > 0; counter--)
    {
        index = randomNum(0, counter);
        temp = deck[index];
        deck[index] = deck[counter];
        deck[counter] = temp;
    }
}

/*********************************************************************
 * openDeck(int[])
 *		This function receives an empty deck and fills it.
 * PRE: Gets an empty deck.
 * POST: no output return, deck is filled with cards in order.
 *********************************************************************/
void openDeck(int deck[])
{
    int i;
    int j;

    for(i = 0; i < DECKSIZE; i++)
    {
       deck[i] = i+1;
    }
}

/**Recives a range and returns a random number in that range**/
int randomNum(int minNum, int maxNum)
{
    return rand()%(maxNum-minNum+1)+minNum;
}

/** This function obtain an array and randomly choose a card that is not already picked by the computer or a player*/
int drawCard(int deck[], int cards[], int i)
{
    int index;
    int temp;
    int value;


    /* picking a random card which the value is not zero ,
    if the value is zero means that card is already used.*/
    do {
        index = randomNum(0, DECKSIZE-1);
    }while (deck[index] == 0);

    if(deck[index]%13 == 0)
        value = 10;
    else if(deck[index]%13 <= 10)
        value = deck[index]%13;
    else
        value = 10;

    temp = deck[index];
    deck[index] = 0; /*setting the value to zero,which means this card is already used */
    cards[i] = temp;

    return value;
}

/*********************************************************************
 * playerTurn(int, int, int, int, int[], int[])
 *		This function plays the turn for the human player where they
 *      can draw as many cards until they get over 21.
 * PRE: Gets all value for display function and the deck.
 * POST: Returns the score at the end of turn, deck looses some cards and
 *       gives it to players hand.
 *********************************************************************/
int playerTurn(int pC, int cC, int pS, int cS, int pCar[], int cCar[], int deck[])
{
    char draw = '1';/*1 for draw card and 0 for don't*/

    int count = 0;

    /*draw other cards while not busting*/
    while(draw == '1' && pS <= 21)
    {
        pS = pS + drawCard(deck, pCar, count);
        displayScreen(pC, cC, pS, cS, pCar, cCar);

        if(pS <= 21)
        {
            printf(" Would you like to draw another card?\n Type 1 for yes and 0 for no.");
            scanf("%c", &draw);
            clean();//clean out extra chars
            while (draw != '0' && draw != '1')
            {
                printf(" Sorry that is not a valid anwser, please try again.\n");
                scanf("%c", &draw);
                clean();//clean out extra chars
            }
        }
        count ++;
    }

    if (pS > 21)
    {
        printf(" You have busted! Your total card value is above 21!\n Score will be reset to 0!");
        printf("\n Press enter to continoue.");
        pS = 0;
        clean();
    }

    return pS;
}

/*********************************************************************
 * compTurn(int, int, int, int, int[], int[])
 *		This function plays the turn for the computer where it draws
 *      cards until they the score adds up to 15.
 * PRE: Gets all value for display function and the deck.
 * POST: Returns the score at the end of turn, deck looses some cards and
 *       gives it to computers hand.
 *********************************************************************/
int compTurn(int pC, int cC, int pS, int cS, int pCar[], int cCar[], int deck[])
{
    int count = 0;

    /*draw other cards while not busting*/
    while(cS <= 15)
    {
        cS = cS + drawCard(deck, cCar, count);
        displayScreen(pC, cC, pS, cS, pCar, cCar);

        printf(" The computer has drawn a card! Press enter.");
        clean();//pause game
        count ++;
    }

    if (cS > 21)
    {
        printf(" The computer has busted! Its total card value is above 21!\n Score will be reset to 0!");
        printf("\n Press enter to continoue.");
        cS = 0;
        clean();
    }

    return cS;
}

