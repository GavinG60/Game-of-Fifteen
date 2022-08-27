/**************************************************************************
 * This work originally copyright David J. Malan of Harvard University, or-
 * iginally released under:
 * CC BY-NC-SA 3.0  http://creativecommons.org/licenses/by-nc-sa/3.0/
 * licensing.
 *
 * It has been adapted for use in csci 1730.  Per the share-alike
 * clause of this license, this document is also released under the
 * same license.
 **************************************************************************/
/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _BSB_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
int move(int tile);
int won(void);

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (1)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("win!\n");
            break;
        }

        // prompt for move
        printf("Tile to move (0 to exit): ");
        int tile;
		scanf("%d", &tile);
		getchar();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(50000);
        }

        // sleep thread for animation's sake
        usleep(50000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(200000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
    int numTiles = (d * d) - 1;
    // Builds the board in reverse numerical order
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            board[i][j] = numTiles;
            numTiles--;
        } // for
    } // for
    // If there are an even number of tiles, 1 and 2 are switched
    if ((d % 2) == 0) {
        board[d - 1][d - 3] = 1;
        board[d - 1][d - 2] = 2;
    } // if
    // The last spot is empty
    board[d - 1][d - 1] = (d * d);
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // Prints the board with spaces between numbers
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if (board[i][j] == (d * d)) {
                // If there are 2-digit numbers, changes spaces to fit
                if (d == 3) {
                    printf("_ ");
                } else {
                     printf(" _ ");
                } // if
            } else {
                // Adjusts spaces for 2-digit numbers
                if (board[i][j] < 10 && (d * d) > 10) {
                    printf(" %i ", board[i][j]);
                } else {
                    printf("%i ", board[i][j]);
                } // if
            } // if
        } // for
        printf("\n");
    } // for
}
/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
int move(int tile)
{
    // If the number is not on the board, returns false
    if (tile > (d * d) - 1 || tile < 1) {
        return 0;
    } // if
    int row = -1;
    int col = -1;
    int check = -1;
    // Finds where the given number is on the board
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if (board[i][j] == tile) {
                check = board[i][j];
                row = i;
                col = j;
                break;
            } // if
        } // for
        if (check == tile) {
            break;
        } // if
    } // for
    int blank = (d * d);
    // Checks each spot around number to see if one is empty
    if (board[row - 1][col] == blank) {
        int temp = board[row][col];
        board[row][col] = board[row - 1][col];
        board[row - 1][col] = temp;
        return 1;
    } // if
    if (board[row][col - 1] == blank) {
        int temp = board[row][col];
        board[row][col] = board[row][col - 1];
        board[row][col - 1] = temp;
        return 1;
    } // if
    if (board[row + 1][col] == blank) {
        int temp = board[row][col];
        board[row][col] = board[row + 1][col];
        board[row + 1][col] = temp;
        return 1;
    } // if
    if (board[row][col + 1] == blank) {
        int temp = board[row][col];
        board[row][col] = board[row][col + 1];
        board[row][col + 1] = temp;
        return 1;
    } // if
    return 0;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
int won(void)
{
    int check = 1;
    // Checks if the board is in numerical order
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if (check != board[i][j]) {
                return 0;
            } // if
            check++;
        } // for
    } // for
    // checks if the blank space is last on the board
    if (board[d - 1][d - 1] != (d * d)) {
        return 0;
    } // if
    return 1;
}
