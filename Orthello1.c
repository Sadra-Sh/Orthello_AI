#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void printBoard(char board[][26], int dim){
    // prints the top categories of board
    printf("  ");
    for (int i=0; i<dim; i++){
        printf("%c", 'a'+i);
    }
    printf("\n");
    // prints each row of the board and goes through every column
    for (int i=0; i<dim; i++){
        printf("%c ", 'a'+i);
        for (int j=0; j<dim; j++){
            printf("%c", board[i][j]);
        }
        printf("\n");
    }

}

void default_board(char board[][26], int dim){
    for (int i=0; i<dim; i++){
        for (int j=0; j<dim; j++){
        if ( i==(dim/2)-1 ){
            board[i][i]='W';   
            board[i][i+1]='B';
        }
        else if( i==dim/2){
            board[i][i]='W';
            board[i][i-1]='B';
        }
        else {
            board[i][j]='U';
        }

        }    
    }
}

bool positionInBounds(int dim, int row, int col) {
    bool result = true;

    if (row >= dim || row < 0 || col >= dim || col < 0){
        result = false;
    }
    return result; 
}



int checkDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol){ 
    int dim = n; 
    int score = 0;
    if (!positionInBounds(dim, row, col)){
        return 0; 
    }
    if (deltaRow==0 && deltaCol==0){
        return 0; 
    }
    if (colour == 'W'){ //check every direction for each colour
        if (board[row+deltaRow][col+deltaCol]=='B'){
            while (board[row+deltaRow][col+deltaCol]=='B'){
                row += deltaRow;
                col += deltaCol; 
                score ++; 
                if (!positionInBounds(dim, row, col)){
                    return 0; 
                }
            }
            if (board[row+deltaRow][col+deltaCol]=='W'){
                return score;
            }
            else {
                return 0;
            }
        }
    }

    
    if (colour == 'B'){
        if (board[row+deltaRow][col+deltaCol]=='W'){
            while (board[row+deltaRow][col+deltaCol]=='W'){
                row += deltaRow;
                col += deltaCol; 
                score ++; 
                if (!positionInBounds(dim, row, col)){
                    return 0;
                }
            }
            if (board[row+deltaRow][col+deltaCol]=='B'){
                return score; 
            }
            else{
                return 0;
            }
        }   
    }

    return score; 
}

void place_configuration(char string[], char board[][26]){   // need 2 pointers for every dimension of an array
    int i = string[1] -'a';                   // we can also make the function accept the actual string or array
    int j = *(string+2) -'a';   //you can use either use a pointer to the string or use the string itself
    char colour = string[0];
    
    board[i][j] = colour;       // cant use the pointer to the array since it stores the address of the it 
}

//checks if the move is valid, if it is it turns the correct spaces
void checkAndTurn(char board[][26], char answer[4], int dim, char computer){   
    int row = answer[1] - 'a';
    int col = answer[2] - 'a';

    for (int i=-1; i<2; i++){
        for (int j =-1; j<2; j++){
            if (i==0 && j==0){  // cant move at its own spot
                continue;
            }
            if (answer[0]=='W'){    //runs for every direction and turns when that direction is valid
                if (checkDirection(board, dim, row, col, 'W', i, j)!=0){    
                    board[row][col] = 'W';
                    int row2 = row;
                    int col2 = col; 
                    while (board[row2+i][col2+j]=='B'){
                        row2 += i;
                        col2 += j;
                        board[row2][col2]= 'W';
                    }
                }
            }
            if (answer[0]=='B'){
                if (checkDirection(board, dim, row, col, 'B', i, j)!=0){
                    board[row][col]='B';
                    int row2 = row;
                    int col2 = col; 
                    while (board[row2+i][col2+j]=='W'){
                        row2 += i;
                        col2 += j;
                        board[row2][col2]='B';
                    }
                }
            }
        }
    }
    
    if (board[row][col]=='U'){      // if the spot is left empty then it was an invalid move
        printf("Invalid move.\n");
    }
    else {
        printBoard(board, dim);
    }

}


bool gameOver(char board[][26], int dim){
    bool result = false; 
    int i, j;
    int countW = 0; 
    int countB = 0; 
    int countU = 0; 

    for (i=0; i<dim; i++){
        for (j=0; j<dim; j++){
            if (board[i][j]=='U'){
                countU += 1; 
            }
            if (board[i][j]=='W'){
                countW += 1;
            }
            if (board[i][j]=='B'){
                countB += 1; 
            }
        }
    }
    if (countW == 0 || countB == 0 || countU == 0){
        return true; 
    }
    
return result; 
}


bool haveValidMoves(char board[][26], int dim, char player){
    bool result = false; 
    int i, j, k, l;

    for (i=0; i<dim; i++){
        for (j=0; j<dim; j++){
            if (board[i][j]=='U'){
                for (l=-1; l<2; l++){   //checks every direction for every empty space
                    for (k=-1; k<2; k++){
                        if (k==0 && l==0){
                            continue; 
                        }
                        if (checkDirection(board, dim, i, j, player, l, k)!=0){
                            return true; 
                        }
                    }
                }
            }
        }
    }
    return result; 
}   



void makeBestMove(char board[][26], int dim, char computer){
    int score1 = 0;  
    int score2;     // compare 2 scores to keep track of which ever one is higher
    char row, col; 

    for (int i=0; i<dim; i++){
        for (int j=0; j<dim; j++){
            score2 = 0; 
            if (board[i][j]=='U'){
                for (int k=-1; k<2; k++){   //checks every direction while accumulating score2
                    for (int l=-1; l<2; l++){ 
                        score2 += checkDirection(board, dim, i, j, computer, k, l);
                    }
                }
                
                if (score2 > score1){
                    score1 = score2; 
                    row = 'a' + (char)i ;
                    col = 'a' + (char)j ;     // to store the score and position
                }
            }
        }
    }
    if (score1 != 0){
        char answer[4]= {computer, row, col, '\0'};
        printf("Computer places %c at %c%c.", computer, row, col);
        checkAndTurn(board, answer, dim, computer);
    }
}











int main(void){
    int dim =0;
    int numBlack = 0; 
    int numWhite = 0; 
    char board[26][26];
    char computer, user1, user2; 


// set the entire board to unoccupied
    for (int i=0; i<26; i++){
        for (int j=0; j<26; j++){
        board[i][j]='U';
        }
    }

    printf("Enter the board dimension: ");
    scanf("%d", &dim);
    printf("Computer plays (B/W): ");
    scanf(" %c", &computer);
    default_board(board, dim);
    printBoard(board, dim);  

    if (computer == 'B'){
        while(!gameOver(board, dim)){

            if (haveValidMoves(board, dim, 'B')){
                makeBestMove(board, dim, computer); 
            }
            else if (!haveValidMoves(board, dim, 'B')){
                printf("B player has no valid move.\n");
            }
            if (gameOver(board, dim)){
                break; 
            }
            
            printf("Enter move for colour W (RowCol): ");
            scanf(" %c%c", &user1, &user2);
            char userMove[4] = {'W', user1, user2, '\0'};

            if (!haveValidMoves(board, dim, 'W')){
                printf("W player has no valid move.\n");
            }
            else if (haveValidMoves(board, dim, 'W')){
                checkAndTurn(board, userMove, dim, computer);     //only need this function for the player
            }


            int row = user1 - 'a';
            int col = user2 - 'a'; 
            if (board[row][col]=='U'){
                break; 
            }
        }
        for (int i=0; i<dim; i++){
            for (int j=0; j<dim; j++){
                if (board[i][j]=='B'){
                    numBlack += 1;
                }
                else if(board[i][j]=='W'){
                    numWhite += 1; 
                }
            }
        }
    }

    if (computer == 'W'){
        while (!gameOver(board, dim)){
            printf("Enter move for colour B (RowCol): ");
            scanf(" %c%c", &user1, &user2);
            char userMove[4] = {'B', user1, user2, '\0'};
            if (!haveValidMoves(board, dim, 'B')){
                printf("B player has no valid move.\n");
            }
            else if (haveValidMoves(board, dim, 'B')){
                checkAndTurn(board, userMove, dim, computer);
            }
            if (gameOver(board, dim)){
                break; 
            }

            if (!haveValidMoves(board, dim, 'W')){
                printf("W player has no valid move.\n");
            }
            else if (haveValidMoves(board, dim, 'W')){
                makeBestMove(board, dim, computer);
            }

            int row = user1 - 'a';
            int col = user2 - 'a'; 
            if (board[row][col]=='U'){
                break;
            }
        }
        for (int i=0; i<dim; i++){
            for (int j=0; j<dim; j++){
                if (board[i][j]=='B'){
                    numBlack += 1;
                }
                else if(board[i][j]=='W'){
                    numWhite += 1; 
                }
            }
        }
    }
    if (numBlack > numWhite){       // determining the winner
        printf("B player wins.\n");
    }
    else if (numWhite > numBlack){
        printf("W player wins.\n");
    }
    else {
        printf("Draw!\n");
    }

    return 0;
}
