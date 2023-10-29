#include <stdio.h>
#include <stdbool.h>
//The following 2 libraries are used to generate random numbers
#include <stdlib.h>
#include <time.h>

#define SIZE 10
#define BOMBS 15
int bombCoord[15][2]; // This array keeps track of the coordinates of all bombs.

void placeBombs(char board[SIZE][SIZE])
{
    srand(time(NULL)); //I found the implementation of srand() function using time.h from ChatGPT
    bool randomIsSame = true;
    int randx = -1;
    int randy = -1;
    for (int i = 0; i < BOMBS; i++)
    {
        // This loop continues until a unique coordinate is generated
        while (randomIsSame)
        {
            randx = rand() % 10;
            randy = rand() % 10;
            randomIsSame = false; // the assumption is made that whatever random coordinate is generated is unique
            for (int j = 0; j < BOMBS; j++)
            {
                if (bombCoord[j][0] == -1)
                {
                    break; // The loop is terminated if the end of the bombCoord array is reached which proves that the generated coordinate is unique
                }
                else if (bombCoord[j][0] == randx && bombCoord[j][1] == randy)
                {
                    randomIsSame = true; // It is proven that the random coordinate was in fact generated before, so now a new coordinate will be generated in the next iteration
                    break;
                }
            }
        }
        bombCoord[i][0] = randx;
        bombCoord[i][1] = randy;
        board[randx][randy] = 'X';
        randomIsSame = true;
    }
}

// This function prints the board and decides whether the location of the bombs are revealed or not by checking if the game is over
void printBoard(char board[SIZE][SIZE], bool gameOver)
{
    if (!gameOver)
    {
        printf("  ");
        for (int a = 0; a < SIZE; a++)
        {
            printf("%d ", a);
        }
        printf("\n");
        for (int i = 0; i < SIZE; i++)
        {
            printf("%d ", i);
            for (int j = 0; j < SIZE; j++)
            {
                if (board[i][j] == 'X')
                {
                    printf("%c ", '-'); // replace the 'X' with '-' because we do not want the user to know the location of the bombs
                }
                else
                {
                    printf("%c ", board[i][j]);
                }
            }
            printf("\n");
        }
    }
    else
    {
        // This code block prints the board with the location of the bombs when the game is over
        printf("  ");
        for (int a = 0; a < SIZE; a++)
        {
            printf("%d ", a);
        }
        printf("\n");
        for (int i = 0; i < SIZE; i++)
        {
            printf("%d ", i);
            for (int j = 0; j < SIZE; j++)
            {
                printf("%c ", board[i][j]);
            }
            printf("\n");
        }
    }
}

// This function initializes the board with all cells as '-', this happens before the bombs are placed
void initializeBoard(char board[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            board[i][j] = '-';
        }
    }
}

// This function counts the all the adjacent bombs to the cell that the user has chosen
int countAdjacentBombs(char board[SIZE][SIZE], int x, int y)
{
    int numBombs = 0;
    // In the following loop, the value of i and j changes between -1, 0, 1 which is used to access the adjacent cells of board[x][y]
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue; // The cell that the user chose obviously does not have a bomb so no need to check it
            }
            else if (board[x + i][y + j] == 'X')
            {
                numBombs++;
            }
        }
    }

    return numBombs;
}

int main()
{
    char board[SIZE][SIZE];
    int ASCII[9] = {48, 49, 50, 51, 52, 53, 54, 55, 56}; // This array will be used to convert the number of bombs returned by the countAdjacentBombs function from an int to a char
    for (int i = 0; i < BOMBS; i++)
    {
        bombCoord[i][0] = -1;
        bombCoord[i][1] = -1;
    }
    initializeBoard(board);
    placeBombs(board);
    bool gameOver = false;
    int remainingCells = SIZE * SIZE - BOMBS;
    int userInput[2];                   // This array is used to store the user input of the coordinates
    int allUserInps[remainingCells][2]; // This array stores all the previous inputs by the user
    for (int i = 0; i < remainingCells; i++)
    {
        allUserInps[i][0] = -1;
        allUserInps[i][1] = -1;
    }
    bool invalidInput = false;
    int turnCounter = 0; // this counter helps keep track of the number of turns the user has taken and adds the user input to the appropriate index in allUserInps
    printf("Welcome to Minesweeper!\nEnter x and y coordinates (numbers from 0 to 9), with space between them, to reveal a cell\n");
    while (remainingCells > 0 && !gameOver)
    {
        printBoard(board, gameOver);
        printf("Enter x and y coordinates (numbers from 0 to 9), with space between them: ");
        int scanfReturn = scanf("%d %d", &userInput[0], &userInput[1]);

        //The following if statement was written with the help of ChatGPT and Google
        if(scanfReturn != 2){//This statement checks whether the user entered 2 integers or not 
            printf("The input must be a set of integers. Please try again.\n");
            while(getchar() != '\n') { 
                continue;//This statement prevents the program from crashing if the user does not enter a set of integers
            }
            continue;
        } 
        if (userInput[0] > 9 || userInput[0] < 0 || userInput[1] > 9 || userInput[1] < 0) // This condition checks if the user input corresponds to one of the cells in the board or not
        {
            printf("The input must be a number between 0(inclusive) and 9(inclusive) for (x y). Please try again.\n");
            invalidInput = true;
        }
        else
        {
            // This loop checks if the user has already entered the input
            for (int i = 0; i < SIZE * SIZE - BOMBS; i++)
            {
                if (allUserInps[i][0] == -1)
                {
                    break;
                }
                else
                {
                    if (userInput[0] == allUserInps[i][0] && userInput[1] == allUserInps[i][1])
                    {
                        printf("You have already revealed the cell at this location. Please try again.\n");
                        invalidInput = true;
                        break;
                    }
                }
            }
        }
        if (invalidInput)
        {
            invalidInput = false;
            continue; // the current iteration of the game is skipped if the user enters an invalid input
        }
        if (board[userInput[0]][userInput[1]] == 'X')
        {
            gameOver = true;
            printf("Game Over! You hit a bomb.\n");
            printBoard(board, gameOver); // reveal the location of the bombs because the game is over
        }
        else
        {
            board[userInput[0]][userInput[1]] = (char)ASCII[countAdjacentBombs(board, userInput[0], userInput[1])]; // replace '-' with the number of bombs adjacent to this cell
            allUserInps[turnCounter][0] = userInput[0];
            allUserInps[turnCounter][1] = userInput[1]; // update the allUserInps array to keep track of all previous user inputs
            turnCounter++;
            remainingCells--;
        }
    }
    if(remainingCells == 0) {
        printf("You won! Go celebrate!");
    }
}