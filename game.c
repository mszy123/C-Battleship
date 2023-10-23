/*****
 * Project 02: Grid game
 * COSC 208, Introduction to Computer Systems, Fall 2023
 *****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define SHIP_CHAR '#'
#define HIT_CHAR 'X'
#define MISS_CHAR '%'
#define SUNK_CHAR '$'
#define BOARD_SIZE 10
#define NUM_SHIPS BOARD_SIZE/2

void printInstructions(){
    printf("\n--------------------------------------------\n");
    printf("Welcome to Battleship | Instructions:\n");
    printf("\t- Battleship is a two-player strategy game played on a grid\n");
    printf("\t- Each player arranges their fleet of ships, such as aircraft carriers, battleships, submarines, cruisers, and destroyers, on their grid without revealing their placement\n");
    printf("\t- Players take turns entering coordinates, like 'B3,' to attempt to hit and sink the opponent's ships\n");
    printf("\t- The first player to sink all of their opponent's ships wins the game\n");
    printf("\t- Ship = '#' | Hit = 'X' | Miss = '%%' | Sunk = '$'\n");
    printf("--------------------------------------------\n\n");
}

struct Ship {
    int size;
    char name[20];
    int hits;
    char location[8];
};

//initalized a list of ship structs
struct Ship** initializeShips(){
    struct Ship **shipArray = malloc(NUM_SHIPS * sizeof(struct Ship));

    for (int i = 0; i < NUM_SHIPS; i++) {
        shipArray[i] = (struct Ship*)malloc(sizeof(struct Ship));
    }

    shipArray[0]->size = 5;
    strcpy(shipArray[0]->name, "Carrier");
    shipArray[0]->hits = 0;

    shipArray[1]->size = 4;
    strcpy(shipArray[1]->name, "Battleship");
    shipArray[1]->hits = 0;

    shipArray[2]->size = 3;
    strcpy(shipArray[2]->name, "Destroyer");
    shipArray[2]->hits = 0;

    shipArray[3]->size = 3;
    strcpy(shipArray[3]->name, "Submarine");
    shipArray[3]->hits = 0;

    shipArray[4]->size = 2;
    strcpy(shipArray[4]->name, "Patrol Boat");
    shipArray[4]->hits = 0;

    return shipArray;
}


//prints one grid
void printGrid(char **grid){
    printf("    A B C D E F G H I J\n");
    printf("   ---------------------\n");


    for(int i = 0; i < BOARD_SIZE; i++){
        printf("%d | ", i);

        for(int j = 0; j < BOARD_SIZE; j++){
            printf("%c ", grid[i][j]);
        }
        printf("|");
        printf("\n");
    }
    printf("    ---------------------\n");
}


//prints two grids side-by-side
void printBothGrids(char **playerGrid, char **playersOpposingGrid) {
    printf("       Player's Grid             Opponent's Grid\n");
    printf("    A B C D E F G H I J         A B C D E F G H I J\n");
    printf("   ---------------------       ---------------------\n");

    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d | ", i);

        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", playerGrid[i][j]);
        }

        printf("| ");

        printf("  %d | ", i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", playersOpposingGrid[i][j]);
        }

        printf("|\n");
    }

    printf("    ---------------------      ---------------------\n");
}

//Checks that placement is in bounds, correct size, doesn't overlap, in a vertical or horizontal line
int checkValidShipPlacement(char **grid, int shipSize, int frontRow, int frontCol, int backRow, int backCol, int isPlayer){
    //ship goes off grid
    if (frontRow < 0 || frontRow >= BOARD_SIZE || frontCol < 0 || frontCol >= BOARD_SIZE || backRow < 0 || backRow >= BOARD_SIZE || backCol < 0 || backCol >= BOARD_SIZE) {
        if(isPlayer)
            printf("Ship placement out of bounds!\n");
        return 0;
    }

    //check ship facing horizontal or vertical
    if (frontRow != backRow && frontCol != backCol) {
        if(isPlayer)
            printf("Invalid ship placement. Ships must be placed either horizontally or vertically.\n");
        return 0;
    }

    // Check if the placement is horizontal and has enough space for the ship
    if (frontRow == backRow) {

        if (backCol - frontCol + 1 != shipSize) {
            if(isPlayer)
                printf("Ship size does not match the specified coordinates!\n");
            return 0;
        }

        int col = 0;
        for (col = frontCol; col <= backCol; col++) {
            if (grid[frontRow][col] == SHIP_CHAR) {
                if(isPlayer)
                    printf("Ship placement overlaps with another ship!\n");
                return 0;
            }
        }
    }

    // Check if the placement is vertical and has enough space for the ship
    if (frontCol == backCol) {
        if (backRow - frontRow + 1 != shipSize) {
            if(isPlayer)
                printf("Ship size does not match the specified coordinates!\n");
            return 0;
        }

        int row = 0;
        for (row = frontRow; row <= backRow; row++) {
            if (grid[row][frontCol] == SHIP_CHAR) {
                if(isPlayer)
                    printf("Ship placement overlaps with another ship!\n");
                return 0;
            }
        }
    }

    return 1;
}




// '#' Represents Ship
// Allows player to place their ships on the grid in specified format
void placePlayerShip(char **grid, struct Ship *ship){
    int shipSize = ship->size;
    char shipName[10];
    strcpy(shipName, ship->name);
    printGrid(grid);
    printf("\n");
    printf("Place your ship on the grid by entering coordinates of front and back of ship (ex. A7:C7)\n");
    printf("Current ship: %s of size %d\n", shipName, shipSize);
    char buffer[8];
    
    int validShipPlacement = 0;
    int frontRow = 0;
    int frontCol = 0;
    int backRow = 0;
    int backCol = 0;
    while (!validShipPlacement){
        printf("Enter coordinate for ship: ");
        fgets(buffer, sizeof(buffer), stdin);
        strcpy(ship->location, buffer);
        frontRow = buffer[1] - '0';
        frontCol = buffer[0] - 'A';
        backRow = buffer[4] - '0';
        backCol = buffer[3] - 'A';
        if (frontCol == backCol && backRow < frontRow) {
            int temp = backRow;
            backRow = frontRow;
            frontRow = temp;
        } 
        else if (frontRow == backRow && backCol < frontCol) {
            int temp = backCol;
            backCol = frontCol;
            frontCol = temp;
        }
        validShipPlacement = checkValidShipPlacement(grid, shipSize, frontRow, frontCol, backRow, backCol, 1);
    }
    if (frontCol == backCol) {
        for (int row = frontRow; row <= backRow; row++) {
            grid[row][frontCol] = SHIP_CHAR;
        }
    }

    if (frontRow == backRow) {
        for (int col = frontCol; col <= backCol; col++) {
            grid[frontRow][col] = SHIP_CHAR;
        }
    }

    printf("\n");
    printGrid(grid);
    printf("\n");
    shipSize--;
    
}


// has computer place ships randomly
void placeOpponentShip(char **grid, struct Ship *ship) {
    int shipSize = ship->size;
    int validShipPlacement = 0;
    int frontRow = 0;
    int frontCol = 0;
    int backRow = 0;
    int backCol = 0;

    srand(time(NULL));

    while (!validShipPlacement) {
        frontRow = rand() % 10;  
        frontCol = rand() % 10; 

        //random direction 0 = horizontal, 1 = vertical
        int direction = rand() % 2;

        if (direction == 0) {
            backRow = frontRow;
            backCol = frontCol + shipSize - 1;
        } else {
            backRow = frontRow + shipSize - 1;
            backCol = frontCol;
        }

        validShipPlacement = checkValidShipPlacement(grid, shipSize, frontRow, frontCol, backRow, backCol, 0);
    }

    if (frontCol == backCol) {
        for (int row = frontRow; row <= backRow; row++) {
            grid[row][frontCol] = SHIP_CHAR;
        }
    }

    if (frontRow == backRow) {
        for (int col = frontCol; col <= backCol; col++) {
            grid[frontRow][col] = SHIP_CHAR;
        }
    }

    char location[8];
    sprintf(location, "%c%d:%c%d", 'A' + frontCol, frontRow, 'A' + backCol, backRow);
    strcpy(ship->location, location);

    
}

// checks if chosen firing coordinate is valid
int checkValidFiringCoordinates(int row, int col, char **opposingGrid){
    if(row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE){
        return 0;
    }

    else if(opposingGrid[row][col] == HIT_CHAR || opposingGrid[row][col] == SUNK_CHAR || opposingGrid[row][col] == MISS_CHAR){
        return 0;
    }

    return 1;
}

//Checks if coordinate selected has a coordinate of opponent's ship
int checkIfHit(struct Ship *ship, int x, int y){
    char coords[10];
    strcpy(coords, ship->location);
    int frontRow = coords[1] - '0';
    int frontCol = coords[0] - 'A';
    int backRow = coords[4] - '0';
    int backCol = coords[3] - 'A';
    

    if (frontCol == backCol) {
        for (int row = frontRow; row <= backRow; row++) {
            if(row == x && frontCol == y){
                ship->hits++;
                return 1;
            }
        }
    }

    if (frontRow == backRow) {
        for (int col = frontCol; col <= backCol; col++) {
            if(frontRow == x && col == y){
                ship->hits++;
                return 1;
            }
        }
    }
    
    return 0;
}

//Checks if all coordinates of a ship have been hit
int checkIfSunk(char **opponentGrid, struct Ship *ship, int player){
    if(ship->hits == ship->size){
        char coords[10];
        strcpy(coords, ship->location);
        int frontRow = coords[1] - '0';
        int frontCol = coords[0] - 'A';
        int backRow = coords[4] - '0';
        int backCol = coords[3] - 'A';

        if (frontCol == backCol) {
            for (int row = frontRow; row <= backRow; row++) {
                opponentGrid[row][frontCol] = SUNK_CHAR;
            }
        }

        if (frontRow == backRow) {
            for (int col = frontCol; col <= backCol; col++) {
                opponentGrid[frontRow][col] = SUNK_CHAR;
            }
        }
        char name[10];
        strcpy(name, ship->name);
        if(player)
            printf("The opponent's ");
        else
            printf("Your ");
        printf("%s has been sunk!\n", name);
        return 1;
    }
    return 0;
}

// plays one player turn, getting input and firing on opponent
int playerTurn(char **playerGrid, char **playersOpponentGrid, char **opponentGrid, struct Ship **opposingShips){
    char buffer[8];
    int validCoord = 0;
    int row = 0;
    int col = 0;
    printBothGrids(playerGrid, playersOpponentGrid);
    while(!validCoord){
        printf("Enter coordinate you want to fire at (ex. A3): ");
        fgets(buffer, sizeof(buffer), stdin);
        row = buffer[1] - '0';
        col = buffer[0] - 'A';
        validCoord = checkValidFiringCoordinates(row, col, opponentGrid);
        if(!validCoord)
            printf("Invalid Coordinate! Try again\n");
    }
    int isHit = 0;
    for(int i = 0; i < NUM_SHIPS; i++){
        isHit = checkIfHit(opposingShips[i], row, col);
        if(isHit){
            printf("HIT!\n");
            playersOpponentGrid[row][col] = HIT_CHAR;
            opponentGrid[row][col] = HIT_CHAR;
            checkIfSunk(playersOpponentGrid, opposingShips[i], 1);
            return 1;
        }
    }
    printf("MISS\n");
    playersOpponentGrid[row][col] = MISS_CHAR;
    opponentGrid[row][col] = MISS_CHAR;
    return 0;
    

}

// plays one computer turn, firing on players ships
int computerTurn(char **opponentGrid, struct Ship **opposingShips){
    int valid = 0;
    int row = 0;
    int col = 0;
    srand(time(NULL));

    while(!valid){
        row = rand() % 10;
        col = rand() % 10;
        valid = checkValidFiringCoordinates(row, col, opponentGrid);
    }

    int isHit = 0;

    for(int i = 0; i < NUM_SHIPS; i++){
        isHit = checkIfHit(opposingShips[i], row, col);
        if(isHit){
            printf("You have been HIT at %c%d!\n", 'A' + col, row);
            opponentGrid[row][col] = HIT_CHAR;
            checkIfSunk(opponentGrid, opposingShips[i], 0);
            return 1;
        }
    }

    printf("Computer MISSED at %c%d!\n", 'A' + col, row);
    opponentGrid[row][col] = MISS_CHAR;
    return 0;
}

//Checks if all ships have sunk
int checkIfWin(struct Ship **ships){
    for(int i = 0; i < NUM_SHIPS; i++){
        if(!(ships[i]->hits == ships[i]->size)){
            return 0;
        }
    }
    return 1;
}

//Create game boards
char ** initializeOneGrid(){
    char ** toReturn = malloc(BOARD_SIZE * sizeof(char*));
    for(int i = 0; i < BOARD_SIZE; i++){
        toReturn[i] = malloc(BOARD_SIZE * sizeof(char));
        for(int j = 0; j < BOARD_SIZE; j++){
            toReturn[i][j] = 'O';
        }
    }
    return toReturn;
}



int main() {
    printInstructions();
    char **playerGrid = initializeOneGrid();
    char **opponentGrid = initializeOneGrid();
    char **playersOpponentGrid = initializeOneGrid();

    

    struct Ship** playerShips = initializeShips();
    struct Ship** opponentShips = initializeShips();

    

    for(int i = 0; i < NUM_SHIPS; i++){
        placePlayerShip(playerGrid, playerShips[i]);
        placeOpponentShip(opponentGrid, opponentShips[i]);
    }


    //Uncomment to make easier for testing -> see opponent ship placement
    //printGrid(opponentGrid);

    

   
    int playerWon = 0;
    int computerWon = 0;
    while(!playerWon && !computerWon){
        playerTurn(playerGrid, playersOpponentGrid, opponentGrid, opponentShips);
        playerWon = checkIfWin(opponentShips);
        if(playerWon){
            printf("You have won!\n");
            break;
        }

        computerTurn(playerGrid, playerShips);
        computerWon = checkIfWin(playerShips);
        if(computerWon){
            printf("The computer won. You lost!\n");
            break;
        }
    }
    for(int i = 0; i < NUM_SHIPS; i++){
        free(playerShips[i]);
        free(opponentShips[i]);
    }
    free(playerShips);
    free(opponentShips);

    for(int i = 0; i < BOARD_SIZE; i++){
        free(playerGrid[i]);
        free(opponentGrid[i]);
        free(playersOpponentGrid[i]);
    }
    free(playerGrid);
    free(opponentGrid);
    free(playersOpponentGrid);
        

    
}