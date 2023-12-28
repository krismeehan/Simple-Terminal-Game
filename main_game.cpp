#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
  
using namespace std;

void printBoard(char **, int);
void move(char **, int&, int&, char, int, bool&);
void enemyMove(char **, int);

int main()
{   
    srand(time(NULL));

    int difficulty = 0;
    int size;
    int row, col;
    int rowPos = 1, colPos = 1;
    int enemies, traps;
    int i, j;
    char direction;
    bool gameOver = false;

    cout << "Choose difficulty (1 = easy, 2 = medium, 3 = hard): ";

    // set size of board based on difficult chosen
    while(difficulty < 1 || difficulty > 3)
    {
      cin >> difficulty;
      if(difficulty == 1)
        size = 10;
      else if(difficulty == 2)
        size = 20;
      else if(difficulty == 3)
        size = 30;
      else
        cout << "Enter a valid difficulty (1, 2 or 3): ";
    }
    // set enemies and traps to a percentage of the board size 
    enemies = (double)size*0.3;
    traps = (double)size*0.2;       

    // create a board of the size specified
    char **board;
    board = new char*[size];

    // initialize the new board
    for (int row = 0; row < size; row++)
      board[row] = new char[size]; 
    
    // fill board with periods
    for (i = 0; i < size; i++) 
    {   
      for (j = 0; j < size; j++) 
      {  
        board[i][j] = '.';
      }      
    }
    // place enemies randomly on board
    for(i = 0; i < enemies; i++)
    {
      row = rand() % size;
      col = rand() % size;
      // don't place an enemy if it's too close to the player or the treasure
      if((row < 3 && col < 3) || (row > size-3 && col > size-3))
        i--;
      // don't place an enemy if the spot is already being used
      else if(board[row][col] != '.')
        i--;
      // place an enemy  
      else
        board[row][col] = 'E';
    }
    // place traps randomly on board
    for(i = 0; i < traps; i++)
    {
      row = rand() % size;
      col = rand() % size;
      // don't place a trap if it's too close to the player or the treasure
      if((row < 3 && col < 3) || (row > size-3 && col > size-3))
        i--;
      // don't place a trap if the spot is already being used
      else if(board[row][col] != '.')
        i--;
      // place a trap  
      else
        board[row][col] = 'T';
    }
    // place player and treasure on board
    board[1][1] = 'G';
    board[size-1][size-1] = 'X';

    // until game over or treasure is found
    while((board[size-1][size-1] != 'G') && !gameOver)
    {
      printBoard(board, size);
      cout << "Move up(W) down(S) left(A) or right(D)?: ";
      cin >> direction;
      move(board, rowPos, colPos, direction, size, gameOver);
    }
    printBoard(board, size);
    if(!gameOver)      
      cout << "Congratulations, you win!" << endl;    
    else
      cout << "Game Over" << endl;   

    return 0;
}
// prints the current board
void printBoard(char **array, int dim)
{
  for (int i = 0; i < dim; i++) 
  {   
    for (int j = 0; j < dim; j++) 
    {  
      cout << array[i][j];
    }
    cout << endl;      
  }
}

void move(char **array, int &r, int &c, char dir, int s, bool &gOver)
{
  // move up unless at top of board
  if((dir == 'W' || dir == 'w') && r != 0)
  {
    array[r][c] = '.';
    r = r-1;   
  }
  // move left unless at far left of board
  else if((dir == 'A' || dir == 'a') && c != 0)
  {
    array[r][c] = '.';
    c = c-1; 
  }
  // move down unless at bottom of board      
  else if((dir == 'S' || dir == 's') && r != s-1)
  {
    array[r][c] = '.';
    r = r+1;
  }
  // move right unless at far right of board      
  else if((dir == 'D' || dir == 'd') && c != s-1)
  {
    array[r][c] = '.';
    c = c+1;       
  }
  // move character. If spot to move is enemy or trap, game over       
  if(array[r][c] == '.' || array[r][c] == 'X')
      array[r][c] = 'G';
  else if(array[r][c] == 'E' || array[r][c] == 'T')
    gOver = true;
  
  // move enemies if game is not over
  if(!gOver)
    enemyMove(array, s);
  // check if enemy moved on to player
  if(!gOver && array[r][c] == 'E')
    gOver = true;
}
void enemyMove(char **array, int s)
{
  int dir;
  for (int i = 0; i < s; i++) 
    {   
      for (int j = 0; j < s; j++) 
      {
        // if an enemy is found, move enemy
        if(array[i][j] == 'E')
        {
          // randomize direction to move
          dir = rand() % 4;
          array[i][j] = '.';

          // move up as long as space to move is not off board and does not already have
          // an enemy or trap
          if(dir == 0)
          {            
            if(i != 0 && (array[i-1][j] != 'E' && array[i-1][j] != 'T' && array[i-1][j] != 'X'))              
              array[i-1][j] = 'E';            
            else
              array[i][j] = 'E';                   
          }
          // move down as long as space to move is not off board and does not already have
          // an enemy or trap
          else if(dir == 1)
          {            
            if(i != s-1 && (array[i+1][j] != 'E' && array[i+1][j] != 'T' && array[i+1][j] != 'X'))                         
              array[i+1][j] = 'E';            
            else
              array[i][j] = 'E';    
          }
          // move left as long as space to move is not off board and does not already have
          // an enemy or trap
          else if(dir == 2)
          {            
            if(j != 0 && (array[i][j-1] != 'E' && array[i][j-1] != 'T' && array[i][j-1] != 'X'))
              array[i][j-1] = 'E';
            else
              array[i][j] = 'E';    
          }
          // move right as long as space to move is not off board and does not already have
          // an enemy or trap
          else if(dir == 3)
          {            
            if(j != s-1 && (array[i][j+1] != 'E' && array[i][j+1] != 'T' && array[i][j+1] != 'X'))
            {
              array[i][j+1] = 'E';
              j++;
            }              
            else
              array[i][j] = 'E';    
          }
        }
      }      
    }  
}







