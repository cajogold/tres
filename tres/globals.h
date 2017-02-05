#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <Arduboy2.h>
#include "bitmaps.h"

//Holds the global variables

//Game States
#define STATE_MAIN_MENU     0
#define STATE_HIGHSCORE     1
#define STATE_GAME_NEW      2
#define STATE_GAME_PLAY     3
#define STATE_GAME_PAUSE    4
#define STATE_GAME_OVER     5
#define STATE_GAME_SAVE     6
#define STATE_GAME_RECALL   7

//Define tiles to associate with bitmap frames
//not really used in the code past TILE_3
#define TILE_BLANK          0
#define TILE_1              1
#define TILE_2              2
#define TILE_3              3
//#define TILE_6              4
//#define TILE_12             5
//#define TILE_24             6  
//#define TILE_48             7
//#define TILE_96             8
//#define TILE_192            9
//#define TILE_384            10
//#define TILE_768            11
//#define TILE_1536           12
//#define TILE_3072           13
//#define TILE_6144           14

//EEPROM offsets
#define BASE_OFFSET         512
#define NEXT_TILE_OFFSET    1
#define STACK_COUNT_OFFSET  2
#define STACK_OFFSET        3
#define TILE_BOARD_OFFSET   15
#define HIGHSCORE_OFFSET    31
#define HIGH_TILE_OFFSET    31 + sizeof(unsigned long)

Arduboy2 arduboy;
Sprites sprites;

//initialize a default stack before shuffle
byte stackSize = 12;
byte defaultStack[12] = {
  TILE_1, TILE_1, TILE_1, TILE_1, 
  TILE_2, TILE_2, TILE_2, TILE_2, 
  TILE_3, TILE_3, TILE_3, TILE_3
  };

//tile stack variables
byte shuffledStack[12] = {};
byte nextTile = 0;
byte stackCount = 0;
byte highTile = 0;
byte bonusTileIndex = 0;
byte bonusTile[11] = {3,4,5,6,7,8,9,10,11};

byte frame1 = 0;

//Carrot positions for pause and main menus
byte pauseCarrot = 0;
byte menuCarrot = 0;
byte menuCarrotFrame = 0;

//global state variable
byte gameState = STATE_MAIN_MENU;

//Scoring.  Same as the origninal threes game
unsigned long gameScore = 0;
unsigned long tileScore[15] = {0,0,0,3,9,27,81,243,729,2187,6561,19683,59049,177147,531441};

//Blank tiles loaded into the board.  This holds the current tile set.
byte tileBoard [4][4] = {
  {TILE_BLANK,TILE_BLANK,TILE_BLANK,TILE_BLANK},
  {TILE_BLANK,TILE_BLANK,TILE_BLANK,TILE_BLANK},
  {TILE_BLANK,TILE_BLANK,TILE_BLANK,TILE_BLANK},
  {TILE_BLANK,TILE_BLANK,TILE_BLANK,TILE_BLANK}
};

// Stores which tiles need to be animated when combined
bool tileAnimate[4][4] = {
  {false,false,false,false},
  {false,false,false,false},
  {false,false,false,false},
  {false,false,false,false}
};

//board dimensions and locations
byte tileBoardOriginX = 0;
byte tileBoardOriginY = 0;
byte tileX [4] = {tileBoardOriginX, tileBoardOriginX+16, tileBoardOriginX+32, tileBoardOriginX+48};
byte tileY [4] = {tileBoardOriginY, tileBoardOriginY+16, tileBoardOriginY+32, tileBoardOriginY+48};

bool newTileBottom[4] = {false, false, false, false};
bool newTileTop[4] = {false, false, false, false};
bool newTileRight[4] = {false, false, false, false};
bool newTileLeft[4] = {false, false, false, false};

//Initialize the eeprom space set aside for this game.  Checks for this game and if it has been run before on this hardware
void eepromInit(){
  if (EEPROM.read(EEPROM_STORAGE_SPACE_START + BASE_OFFSET) != 79)
  {
    EEPROM.put(EEPROM_STORAGE_SPACE_START + BASE_OFFSET, (byte)79);
    EEPROM.write(EEPROM_STORAGE_SPACE_START + BASE_OFFSET + NEXT_TILE_OFFSET, (byte)1);
    EEPROM.write(EEPROM_STORAGE_SPACE_START + BASE_OFFSET + STACK_COUNT_OFFSET, (byte)0);
    EEPROM.put(EEPROM_STORAGE_SPACE_START + BASE_OFFSET + HIGHSCORE_OFFSET, (unsigned long)0);   
    EEPROM.write(EEPROM_STORAGE_SPACE_START + BASE_OFFSET + HIGH_TILE_OFFSET, (byte)0); 
    for (byte i = 0; i < 12; i++) EEPROM.write(EEPROM_STORAGE_SPACE_START + BASE_OFFSET + STACK_OFFSET + i, (byte)0);
    byte offsetInc = 0;
    for (byte x = 0; x < 4; x++){
      for (byte y = 0; y < 4; y++){
        EEPROM.write(EEPROM_STORAGE_SPACE_START + BASE_OFFSET + TILE_BOARD_OFFSET + offsetInc, (byte)0);
        offsetInc++;
      }
    }
  }
}

#endif
