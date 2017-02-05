#ifndef TILEBOARD_H
#define TILEBOARD_H

#include <Arduino.h>
#include "globals.h"

//This holds the meat of the game during game play
//Shuffling the stack of tiles
//Drawing the initial stack
//Draw board function to keep the board fresh
//Determines how the tiles should move and how they can combine
//Determines which tile is to be added next from the stack and if a bonus tile should be inserted
//Determines the location to add the new tile to the board
//Handles the tile animation for combining and adding tiles
//scores the game based on the tiles on the board
//checks for a game over

//Shuffle the tile stack
void shuffleStack(){
  //Fill a temp stack just to get ready for the next deck.  This way the first tile in the upcoming stack can be displayed
  for (int i = 0; i < stackSize; i++){
    shuffledStack[i] = defaultStack[i];
  }
  for(int i = 0; i < stackSize; i++){
    //assign a random index
    int j = random() % stackSize;
    //swap the two tiles 
    int temp = shuffledStack[i];
    shuffledStack[i] = shuffledStack[j];
    shuffledStack[j] = temp;     
  }   
}

void drawInitBoard(){
  //Fill back up with blank tiles for new game that isn't from boot
  for (byte x = 0; x < 4; x++){
    for (byte y = 0; y < 4; y++){
        tileBoard[x][y] = TILE_BLANK;            
    }
  }
  
  //fill board with 9 tiles from the shuffled stack
  byte i = 0;
  for (byte x = 0; x < 4; x++){
    for (byte y = 0; y < 4; y++){
      if (i < 9){
        tileBoard[x][y] = shuffledStack[i];
        i++;              
      }
    }
  }

  // randomly shuffle the locations of initial Board for game play
  for (byte x = 0; x < 4; x++){
    for (byte y = 0; y < 4; y++){
      //assign a random index
      byte k = random() % 4;
      byte g = random() % 4;
      //swap the tiles 
      byte temp = tileBoard[x][y];
      tileBoard[x][y] = tileBoard[k][g];
      tileBoard[k][g] = temp;     
    }
  }
  
  //draw the initial board
  sprites.drawSelfMasked(64, 0, inGameBackground, 0);
  for (byte x = 0; x < 4; x++){
    for (byte y = 0; y < 4; y++){  
        sprites.drawSelfMasked(tileX[x], tileY[y], tiles, tileBoard[x][y]);
    }   
  }
  //shuffle a fresh stack of tiles to start playing
  shuffleStack();
}

//Keep board fresh
void drawBoard(){
  //draw board
  sprites.drawSelfMasked(64, 0, inGameBackground, 0);
  for (byte x = 0; x < 4; x++){
    for (byte y = 0; y < 4; y++){  
        sprites.drawSelfMasked(tileX[x], tileY[y], tiles, tileBoard[x][y]);
    }   
  }
  //Keep score updated everytime the board is refreshed
  arduboy.setTextColor(WHITE);
  arduboy.setTextBackground(BLACK);
  arduboy.setCursor(76,48);
  arduboy.print(gameScore);
}

//Shift tiles up and combine if you can
void shiftUp(){
  //check if the tiles can move into blank spots or combine
  for (byte x = 0; x < 4; x++){
    for (byte y = 0; y < 3; y++){  
      if ((tileBoard[x][y] == TILE_1 and tileBoard[x][y+1] == TILE_2) or (tileBoard[x][y] == TILE_2 and tileBoard[x][y+1] == TILE_1)){
        tileBoard[x][y] = TILE_3;
        tileBoard[x][y+1] = TILE_BLANK;
        newTileBottom[x] = true;  
        tileAnimate[x][y] = true;   
      }
      else if ((tileBoard[x][y] == tileBoard[x][y+1]) and tileBoard[x][y] > 2){
        tileBoard[x][y] += 1;
        tileBoard[x][y+1] = TILE_BLANK;
        newTileBottom[x] = true;  
        tileAnimate[x][y] = true;    
      }
      else if ((tileBoard[x][y] == TILE_BLANK) and (tileBoard[x][y+1] != TILE_BLANK) and (y < 3)){
        tileBoard[x][y] = tileBoard[x][y+1];
        tileBoard[x][y+1] = TILE_BLANK;
        newTileBottom[x] = true;
      }
    }   
  }    
}

//Shift tiles down and combine if you can
void shiftDown(){
  //check if the tiles can move into blank spots or combine
  for (byte x = 0; x < 4; x++){
    for (byte y = 3; y > 0; y--){  
      if ((tileBoard[x][y] == TILE_1 and tileBoard[x][y-1] == TILE_2) or (tileBoard[x][y] == TILE_2 and tileBoard[x][y-1] == TILE_1)){
        tileBoard[x][y] = TILE_3;
        tileBoard[x][y-1] = TILE_BLANK; 
        newTileTop[x] = true; 
        tileAnimate[x][y] = true;      
      }
      else if ((tileBoard[x][y] == tileBoard[x][y-1]) and tileBoard[x][y] > 2){
        tileBoard[x][y] += 1;
        tileBoard[x][y-1] = TILE_BLANK; 
        newTileTop[x] = true; 
        tileAnimate[x][y] = true;       
      }      
      else if ((tileBoard[x][y] == TILE_BLANK) and (tileBoard[x][y-1] != TILE_BLANK) and (y > 0)){
        tileBoard[x][y] = tileBoard[x][y-1];
        tileBoard[x][y-1] = TILE_BLANK;
        newTileTop[x] = true;
      }
    }   
  }  
}

//Shift tiles left and combine if you can
void shiftLeft(){
  //check if the tiles can move into blank spots or combine
  for (byte x = 0; x < 3; x++){
    for (byte y = 0; y < 4; y++){  
      if ((tileBoard[x][y] == TILE_1 and tileBoard[x+1][y] == TILE_2) or (tileBoard[x][y] == TILE_2 and tileBoard[x+1][y] == TILE_1)){
        tileBoard[x][y] = TILE_3;
        tileBoard[x+1][y] = TILE_BLANK; 
        newTileRight[y] = true;
        tileAnimate[x][y] = true;       
      }
      else if ((tileBoard[x][y] == tileBoard[x+1][y]) and tileBoard[x][y] > 2){
        tileBoard[x][y] += 1;
        tileBoard[x+1][y] = TILE_BLANK; 
        newTileRight[y] = true; 
        tileAnimate[x][y] = true;       
      }      
      else if ((tileBoard[x][y] == TILE_BLANK) and (tileBoard[x+1][y] != TILE_BLANK) and (x >= 0)){
        tileBoard[x][y] = tileBoard[x+1][y];
        tileBoard[x+1][y] = TILE_BLANK;
        newTileRight[y] = true;
      }
    }   
  } 
}

//Shift tiles right and combine if you can
void shiftRight(){
  //check if the tiles can move into blank spots or combine
  for (byte x = 3; x > 0; x--){
    for (byte y = 0; y < 4; y++){  
      if ((tileBoard[x][y] == TILE_1 and tileBoard[x-1][y] == TILE_2) or (tileBoard[x][y] == TILE_2 and tileBoard[x-1][y] == TILE_1)){
        tileBoard[x][y] = TILE_3;
        tileBoard[x-1][y] = TILE_BLANK;
        newTileLeft[y] = true; 
        tileAnimate[x][y] = true;       
      }
      else if ((tileBoard[x][y] == tileBoard[x-1][y]) and tileBoard[x][y] > 2){
        tileBoard[x][y] += 1;
        tileBoard[x-1][y] = TILE_BLANK; 
        newTileLeft[y] = true; 
        tileAnimate[x][y] = true;       
      }
      else if ((tileBoard[x][y] == TILE_BLANK) and (tileBoard[x-1][y] != TILE_BLANK) and (x < 4)){
        tileBoard[x][y] = tileBoard[x-1][y];
        tileBoard[x-1][y] = TILE_BLANK;
        newTileLeft[y] = true;
      }
    }   
  }  
}

//Draws the next tile to be inserted onto the board.  inserts a bonus card every other stack.
//The bonus card is no more than 1/8th of the highest card on the board at the time
void drawNextTile(){
  //check for the end of the deck
  if (nextTile == 11){
    shuffleStack();
    nextTile = 0;
    stackCount += 1;
    //Find high card and set the next tile to be the bonus card
    if (stackCount == 2){
       //stackCount = 0;
       for (byte x = 0; x < 4; x++){
          for (byte y = 0; y < 4; y++){  
            if (tileBoard[x][y] > highTile and tileBoard[x][y] > 5){ //has to be a 24 or higher
              highTile = tileBoard[x][y];                                    
            }
          }
       }
       if (highTile > 5){
         switch(highTile){
          case 6:
            bonusTileIndex = 0;
            break;
          case 7:
            bonusTileIndex = 1;
            break;
          case 8:
            bonusTileIndex = random(1,2);
            break;
          case 9:
            bonusTileIndex = random(2,3);
            break;
          case 10:
            bonusTileIndex = random(3,4);
            break;
          case 11:
            bonusTileIndex = random(4,5);
            break;
          case 12:
            bonusTileIndex = random(5,6);
            break;
          case 13:
            bonusTileIndex = random(6,7);
            break;
          case 14:
            bonusTileIndex = random(7,8);
            break;         
         }   
       }
       sprites.drawSelfMasked(102,15,tiles,bonusTile[bonusTileIndex]);
    }
    else sprites.drawSelfMasked(102,15,tiles,shuffledStack[0]);
  }
  else {
    if (stackCount == 2){
      sprites.drawSelfMasked(102,15,tiles,bonusTile[bonusTileIndex]);
    }
    else sprites.drawSelfMasked(102,15,tiles,shuffledStack[nextTile]);
  }
}

//Animate the tiles by flipping them.  All combos made will animate at the same time
void animateTile(){      
   for (byte frame = 0; frame < 16; frame++){
   //Animate all of the combos 
      drawBoard();
      for (byte x = 0; x < 4; x++){
        for (byte y = 0; y < 4; y++){
          if (!tileAnimate[x][y]){
            sprites.drawSelfMasked(tileX[x], tileY[y], tiles, tileBoard[x][y]);          
          } 
          else sprites.drawOverwrite(tileX[x], tileY[y], tileAnimation, frame);  
        } 
      } 
      arduboy.display();
      delay(10);
   }
   for (byte x = 0; x < 4; x++){
      for (byte y = 0; y < 4; y++){
           tileAnimate[x][y] = false;
      }
   }
}

//The next four functions add tiles to the appropriate side of the board.  The new tile is animated in by sliding it.
//The location of thenew tile is based on the rows or columns that are moved during the previous move.
//If one row or column moves, then that row or column gets the new tile
//If multiple rows or columns move, then the row or column is chosen randomely to get the new tile
void addTileBottom(){
  byte tempBottom[4] = {0,0,0,0};
  byte countTrue = 0;
  for (byte x = 0; x < 4; x++){
    if(newTileBottom[x]){
      tempBottom[countTrue] = x;
      countTrue++;
    }
  }
  if (countTrue > 0){
    byte i = random(0,(countTrue-1));
    if (stackCount != 2){ 
      for (byte frame = 16; frame > 0; frame--){ 
            sprites.drawOverwrite(tileX[tempBottom[i]], tileY[3]+frame, tiles, shuffledStack[nextTile]);
            drawBoard();
            arduboy.display();
            delay(10);
      }
      tileBoard[tempBottom[i]][3] = shuffledStack[nextTile];
      nextTile++;
    }
    else if (stackCount == 2){
      for (byte frame = 16; frame > 0; frame--){ 
            sprites.drawOverwrite(tileX[tempBottom[i]], tileY[3]+frame, tiles, bonusTile[bonusTileIndex]);
            drawBoard();
            arduboy.display();
            delay(10);
      }
      tileBoard[tempBottom[i]][3] = bonusTile[bonusTileIndex];
      stackCount = 0;      
    }
    for (byte x = 0; x < 4; x++) newTileBottom[x] = false;   
  }           
}

void addTileTop(){
  byte tempTop[4] = {0,0,0,0};
  byte countTrue = 0;
  for (byte x = 0; x < 4; x++){
    if(newTileTop[x]){
      tempTop[countTrue] = x;
      countTrue++;
    }
  }
  if (countTrue > 0){
    byte i = random(0,(countTrue-1));
    if (stackCount != 2){
      for (byte frame = 16; frame > 0; frame--){ 
            sprites.drawOverwrite(tileX[tempTop[i]], tileY[0]-frame, tiles, shuffledStack[nextTile]);
            drawBoard();
            arduboy.display();
            delay(10);
      }
      tileBoard[tempTop[i]][0] = shuffledStack[nextTile];
      nextTile++;     
    }
    else if (stackCount == 2){
      for (byte frame = 16; frame > 0; frame--){ 
            sprites.drawOverwrite(tileX[tempTop[i]], tileY[0]-frame, tiles, bonusTile[bonusTileIndex]);
            drawBoard();
            arduboy.display();
            delay(10);
      }
      tileBoard[tempTop[i]][0] = bonusTile[bonusTileIndex];
      stackCount = 0;;       
    }
  for (byte x = 0; x < 4; x++) newTileTop[x] = false;
  }   
}

void addTileRight(){
  byte tempRight[4] = {0,0,0,0};
  byte countTrue = 0;
  for (byte y = 0; y < 4; y++){
    if(newTileRight[y]){
      tempRight[countTrue] = y;
      countTrue++;
    }
  }
  if (countTrue > 0){
    byte i = random(0,(countTrue-1));
    if (stackCount != 2){
      for (byte frame = 16; frame > 0; frame--){ 
            sprites.drawOverwrite(tileX[3]+frame, tileY[tempRight[i]], tiles, shuffledStack[nextTile]);
            arduboy.fillRect(66,0,16,64,BLACK);
            drawBoard();
            arduboy.display();
            delay(10);
      }
      tileBoard[3][tempRight[i]] = shuffledStack[nextTile];
      nextTile++;    
    }
    else if (stackCount == 2){
      for (byte frame = 16; frame > 0; frame--){ 
            sprites.drawOverwrite(tileX[3]+frame, tileY[tempRight[i]], tiles, bonusTile[bonusTileIndex]);
            arduboy.fillRect(66,0,16,64,BLACK);
            drawBoard();
            arduboy.display();
            delay(10);
      }
      tileBoard[3][tempRight[i]] = bonusTile[bonusTileIndex];
      stackCount = 0;      
    }
    for (byte y = 0; y < 4; y++) newTileRight[y] = false; 
  }        
}

void addTileLeft(){
  byte tempLeft[4] = {0,0,0,0};
  byte countTrue = 0;
  for (byte y = 0; y < 4; y++){
    if(newTileLeft[y]){
      tempLeft[countTrue] = y;
      countTrue++;
    }
  }
  if (countTrue > 0){
    byte i = random(0,(countTrue-1));
    if (stackCount !=2 ){
      for (byte frame = 16; frame > 0; frame--){ 
            sprites.drawOverwrite(tileX[0]-frame, tileY[tempLeft[i]], tiles, shuffledStack[nextTile]);
            drawBoard();
            arduboy.display();
            delay(10);
      }
      tileBoard[0][tempLeft[i]] = shuffledStack[nextTile];
      nextTile++;     
    }
    else if (stackCount == 2){
      for (byte frame = 16; frame > 0; frame--){ 
            sprites.drawOverwrite(tileX[0]-frame, tileY[tempLeft[i]], tiles, bonusTile[bonusTileIndex]);
            drawBoard();
            arduboy.display();
            delay(10);
      }
      tileBoard[0][tempLeft[i]] = bonusTile[bonusTileIndex];
      stackCount = 0;       
    }
    for (byte y = 0; y < 4; y++) newTileLeft[y] = false;
  }          
}

//Add up the score
void scoreGame(){
  gameScore = 0;
  for (byte x = 0; x < 4; x++){
    for (byte y = 0; y < 4; y++){
      gameScore += tileScore[tileBoard[x][y]];   
    }
  }
  //scored = true;
  arduboy.setTextColor(WHITE);
  arduboy.setTextBackground(BLACK);
  arduboy.setCursor(76,48);
  arduboy.print(gameScore);
}

//Check if the board has any moves left.  If not, game over
void isGameOver(){
  for (byte x = 0; x < 4; x++){
    for (byte y = 0; y < 4; y++){ 
      if (x<3 and y<3){
        if ((tileBoard[x][y] == TILE_1 and tileBoard[x][y+1] == TILE_2) or (tileBoard[x][y] == TILE_2 and tileBoard[x][y+1] == TILE_1)) return;      
        else if ((tileBoard[x][y] == TILE_1 and tileBoard[x+1][y] == TILE_2) or (tileBoard[x][y] == TILE_2 and tileBoard[x+1][y] == TILE_1)) return;      
        else if ((tileBoard[x][y] == tileBoard[x][y+1]) and (tileBoard[x][y] > 2)) return;
        else if ((tileBoard[x][y] == tileBoard[x+1][y]) and (tileBoard[x][y] > 2)) return;
      }
      if (y==3 and x<3){
        if ((tileBoard[x][y] == TILE_1 and tileBoard[x+1][y] == TILE_2) or (tileBoard[x][y] == TILE_2 and tileBoard[x+1][y] == TILE_1)) return;      
        else if ((tileBoard[x][y] == tileBoard[x+1][y]) and (tileBoard[x][y] > 2)) return;
      }
      if (x==3 and y<3){
        if ((tileBoard[x][y] == TILE_1 and tileBoard[x][y+1] == TILE_2) or (tileBoard[x][y] == TILE_2 and tileBoard[x][y+1] == TILE_1)) return;      
        else if ((tileBoard[x][y] == tileBoard[x][y+1]) and (tileBoard[x][y] > 2)) return;        
      }
      if (tileBoard[x][y] == TILE_BLANK) return;
    }
  }
  gameState = STATE_GAME_OVER;
}

#endif
