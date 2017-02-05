#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <Arduino.h>
#include "globals.h"
#include "tileboard.h"

//Holds the main game states
//1. New Game
//2. Playing the game
//3. Pause Game
//4. Save Game
//5. Game Over

//Initial game state -- set everything up for a new game
void stateNewGame(){
  arduboy.clear();
  nextTile = 0;
  stackCount = 0;
  shuffleStack();
  gameScore = 0;
  drawInitBoard();
  highTile = 0;
  bonusTileIndex = 0;
  pauseCarrot = 0;
  gameState = STATE_GAME_PLAY;
}

// Main Game Play State
void stateGamePlay(){
  
  drawNextTile();

  if (arduboy.justPressed(UP_BUTTON)){
    shiftUp();
    animateTile();
    addTileBottom();
  }
  if (arduboy.justPressed(DOWN_BUTTON)){
    shiftDown();
    animateTile();
    addTileTop();
  }
  if (arduboy.justPressed(LEFT_BUTTON)){
    shiftLeft();
    animateTile();
    addTileRight();
  }
  if (arduboy.justPressed(RIGHT_BUTTON)){
    shiftRight();
    animateTile();
    addTileLeft();

  }
  if (arduboy.justPressed(B_BUTTON)) gameState = STATE_GAME_PAUSE;
  drawBoard();
  scoreGame();
  isGameOver();
}

// Game Pause Screen.  Draw a rounded rectangle on the screen with some options, Resume, Save, Exit...
void stateGamePause(){
  drawBoard();
  drawNextTile();
  arduboy.drawRoundRect(36,15,54,31,3,WHITE);
  arduboy.fillRoundRect(37,16,52,29,3,BLACK);
  arduboy.setTextColor(WHITE);
  arduboy.setTextBackground(BLACK);
  arduboy.setCursor(48,18);
  arduboy.print("RESUME");
  arduboy.setCursor(48,27);
  arduboy.print("SAVE");
  arduboy.setCursor(48,36);
  arduboy.print("EXIT");
  if (arduboy.justPressed(DOWN_BUTTON) and pauseCarrot < 2){
    pauseCarrot += 1;
  }
  if (arduboy.justPressed(UP_BUTTON) and pauseCarrot > 0){
    pauseCarrot -= 1;
  }
  if (pauseCarrot == 0){
    arduboy.setCursor(40,18);
    arduboy.print(">");
    if (arduboy.justPressed(B_BUTTON)) gameState = STATE_GAME_PLAY;
  }
  else if (pauseCarrot == 1){
    arduboy.setCursor(40,27);
    arduboy.print(">");
    if (arduboy.justPressed(B_BUTTON)) gameState = STATE_GAME_SAVE;
  }
  else if (pauseCarrot == 2){
    arduboy.setCursor(40,36);
    arduboy.print(">");
    if (arduboy.justPressed(B_BUTTON)) gameState = STATE_MAIN_MENU;
  }
  
}

//Save the game to the eeprom and go back to the main menu
void stateGameSave(){
  EEPROM.write(EEPROM_STORAGE_SPACE_START + BASE_OFFSET + NEXT_TILE_OFFSET, nextTile);
  EEPROM.write(EEPROM_STORAGE_SPACE_START + BASE_OFFSET + STACK_COUNT_OFFSET, stackCount);
  for (byte i = 0; i < 12; i++) EEPROM.write(EEPROM_STORAGE_SPACE_START + BASE_OFFSET+STACK_OFFSET + i, shuffledStack[i]);
  byte offsetInc = 0;
  for (byte x = 0; x < 4; x++){
    for (byte y = 0; y < 4; y++){
      EEPROM.write(EEPROM_STORAGE_SPACE_START + BASE_OFFSET + TILE_BOARD_OFFSET + offsetInc, tileBoard[x][y]);
      offsetInc++;
    }
  }
  gameState = STATE_MAIN_MENU;
}

// Game Over Screen.  Write game over on the screen and show the final board and score.
// save the high score if the new high score was achieved.
void stateGameOver(){
  drawBoard();
  arduboy.fillRect(65,0,58,38,WHITE);
  arduboy.setTextColor(BLACK);
  arduboy.setTextBackground(WHITE);
  arduboy.setCursor(73,3);
  arduboy.setTextSize(2);
  arduboy.print("GAME");
  arduboy.setCursor(73,20);
  arduboy.print("OVER");
  arduboy.setTextSize(1);
  //obtain the highest tile on the board
  for (byte x = 0; x < 4; x++){
    for (byte y = 0; y < 4; y++){  
      if (tileBoard[x][y] > highTile){ //has to be a 24 or higher
        highTile = tileBoard[x][y];                                    
      }
    }
  }
  // read the last high score recorded and see if the game score is higher than the last
  unsigned long tempScore = 0;
  EEPROM.get(EEPROM_STORAGE_SPACE_START + BASE_OFFSET + HIGHSCORE_OFFSET, tempScore);
  if (gameScore > tempScore){
    EEPROM.put(EEPROM_STORAGE_SPACE_START + BASE_OFFSET+HIGHSCORE_OFFSET, gameScore);
    EEPROM.put(EEPROM_STORAGE_SPACE_START + BASE_OFFSET+HIGH_TILE_OFFSET, highTile);
  }

  if (arduboy.justPressed(A_BUTTON|B_BUTTON)) gameState = STATE_MAIN_MENU;
  
}

#endif
