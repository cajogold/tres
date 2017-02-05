/*
  Title: Tres
  Author: cajogold (Casey Gold)
  Date:  FEB/2017
  
  What is it: Clone/Port/Rip-off of the game Threes! for mobile phones.
  
  Credit to Team A.R.G. for a good bit of the code structure.  Specificaly the state machine and eeprom.
*/

#include "globals.h"
#include "menu.h"
#include "gameplay.h"
#include "tileboard.h"

typedef void (*FunctionPointer) ();
const FunctionPointer PROGMEM  mainGameLoop[] = {
  stateMainMenu,
  stateHighscore,
  stateNewGame,
  stateGamePlay,
  stateGamePause,
  stateGameOver,
  stateGameSave,
  stateGameRecall,
};

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(30);
  arduboy.initRandomSeed();
  eepromInit();
}

void loop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();
  ((FunctionPointer) pgm_read_word (&mainGameLoop[gameState]))();
  arduboy.display();
}
