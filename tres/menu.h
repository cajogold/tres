#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include "globals.h"

//Main menu states
//1. Main Menu
//2. High Score
//3. Restore Saved Game

//Main Menu
void stateMainMenu(){

  arduboy.drawBitmap(0, 0, menu, 128, 64, WHITE);
  //sprites.drawOverwrite(48,8,menuAnimation,0);
  arduboy.setTextColor(WHITE);
  arduboy.setTextBackground(BLACK);

  if (arduboy.justPressed(DOWN_BUTTON) and menuCarrot < 2){
    menuCarrot += 1;
    menuCarrotFrame += 1;
  }
  if (arduboy.justPressed(UP_BUTTON) and menuCarrot > 0){
    menuCarrot -= 1;
    menuCarrotFrame -= 1;
  }
  if (menuCarrot == 0){
    arduboy.setCursor(25,48);
    arduboy.print("PLAY NEW GAME");
    if (arduboy.justPressed(B_BUTTON)) gameState = STATE_GAME_NEW;
  }
  else if (menuCarrot == 1){
    arduboy.setCursor(32,48);
    arduboy.print("RESUME GAME");
    if (arduboy.justPressed(B_BUTTON)) gameState = STATE_GAME_RECALL;
  }
  else if (menuCarrot == 2){
    arduboy.setCursor(25,48);
    arduboy.print("SEE HIGHSCORE");
    if (arduboy.justPressed(B_BUTTON)) gameState = STATE_HIGHSCORE;
  }

  if (frame1 < 32){ 
    arduboy.drawBitmap(0, 0, menu, 128, 64, WHITE);     
    sprites.drawOverwrite(48,8,menuAnimation,frame1); 
    sprites.drawOverwrite(116,47,mcarrot,menuCarrotFrame);
    delay(30);
    frame1 += 1;
    if (frame1 == 32) frame1 = 0;
  }

}
//High score menu
void stateHighscore(){
  //draw the title text on the screen
  arduboy.drawBitmap(0, 0, highScore, 128, 64, WHITE);
  EEPROM.get(EEPROM_STORAGE_SPACE_START + BASE_OFFSET + HIGH_TILE_OFFSET, highTile);
  sprites.drawSelfMasked(36,12,tiles,highTile);
  //Get high score from eeprom
  unsigned long highScore = 0;
  EEPROM.get(EEPROM_STORAGE_SPACE_START + BASE_OFFSET + HIGHSCORE_OFFSET, highScore);
  arduboy.setTextSize(2);
  arduboy.setCursor(20,41);
  arduboy.print(highScore);
  arduboy.setTextSize(1);
  if (arduboy.justPressed(A_BUTTON|B_BUTTON)) gameState = STATE_MAIN_MENU;
}

//restore the saved game from the eeprom and resume game play
void stateGameRecall(){
  nextTile = EEPROM.read(EEPROM_STORAGE_SPACE_START + BASE_OFFSET+NEXT_TILE_OFFSET);
  stackCount = EEPROM.read(EEPROM_STORAGE_SPACE_START + BASE_OFFSET+STACK_COUNT_OFFSET);
  for (byte i = 0; i < 12; i++) shuffledStack[i] = EEPROM.read(EEPROM_STORAGE_SPACE_START + BASE_OFFSET+STACK_OFFSET + i);
  byte offsetInc = 0;
  for (byte x = 0; x < 4; x++){
    for (byte y = 0; y < 4; y++){
      tileBoard[x][y] = EEPROM.read(EEPROM_STORAGE_SPACE_START + BASE_OFFSET+TILE_BOARD_OFFSET + offsetInc);
      offsetInc++;
    }
  }
  gameState = STATE_GAME_PLAY;
}
#endif
