/*********************************************************************


    Lamps Header File
    Version : 1.00


*********************************************************************/

#ifndef LAMPS_H
#define LAMPS_H

#include "Time.h"

#include <Arduino.h>

#define FLASH_PERIOD 100
#define NUMBER_OF_LAMPS 59

// Define Lamp Matrix
#define LAMP_TERMINATOR 255 // Collection Terminator

// Byte 0
#define LAMP_BONUS_1K 0 // Bonus 1K
#define LAMP_BONUS_2K 1 // Bonus 2K
#define LAMP_BONUS_3K 2 // Bonus 3K
#define LAMP_BONUS_4K 3 // Bonus 4K
#define LAMP_BONUS_5K 4 // Bonus 5K

#define LAMP_BONUS_6K 4 // Bonus 6K
#define LAMP_BONUS_7K 5 // Bonus 7K
#define LAMP_BONUS_8K 6 // Bonus 8K
#define LAMP_BONUS_9K 7 // Bonus 9K

// Byte 1
#define LAMP_NU 8           // Small 9
#define LAMP_DUNK_SHOT_5K 9       // Small 10
#define LAMP_RIGHT_SPINNER_ADV 10 // Small 11
#define LAMP_LEFT_SPINNER_ADV 11  // Small 12

#define LAMP_SPINNERS_1 12 // Small 13
#define LAMP_SPINNERS_2 13 // Small 14
#define LAMP_SPINNERS_3 14 // Small 15
#define LAMP_SPINNERS_4 15 // Left Arrow

// Byte 2
#define LAMP_SPINNERS_5 16        // Bank Shot 300 1
#define LAMP_DUNK_SHOT_GATE 17    // Bank Shot 600 2
#define LAMP_GATE_OPEN_WL 18      // Bank Shot 900 3
#define LAMP_TARGET_SAUCER_25K 19 // Bank Shot 1200 4

#define LAMP_LEFT_SPINNER_100 20  // Bank Shot 1500 5
#define LAMP_LEFT_SPINNER_200 21  // Bank Shot 5000 6
#define LAMP_LEFT_SPINNER_1000 22 // OutLAMPne Special
#define LAMP_LEFT_SPINNER_2000 23 // Same PLAYER Shoots Again - Playfield

// Byte 3
#define LAMP_SAUCER_G 24 // Big 1
#define LAMP_SAUCER_L 25 // Big 2
#define LAMP_SAUCER_O 26 // Big 3
#define LAMP_SAUCER_B 27 // Big 4

#define LAMP_SAUCER_E 28              // Big 5
#define LAMP_DUNK_SHOT_SPECIAL 29     // Big 6
#define LAMP_LEFT_OUTLANE_25K 30      // Big 7
#define LAMP_TARGET_SAUCER_SPECIAL 31 // Big 8

// Byte 4
#define LAMP_SIDE_TARGET_5 32 // Big 9
#define LAMP_SIDE_TARGET_4 33 // Big 10
#define LAMP_SIDE_TARGET_3 34 // Big 11
#define LAMP_SIDE_TARGET_2 35 // Big 12

#define LAMP_SIDE_TARGET_1 36 // Big 13
#define LAMP_BOTTOM_BUMPER 37 // Big 14
#define LAMP_LR_BUMPERS 38    // Big 15
#define LAMP_DUNK_SHOT_EB 39  // Right Arrow

// Byte 5
#define LAMP_CENTER_E 40                         //
#define LAMP_MATCH 41                    // Match
#define LAMP_SAME_PLAYER_SHOOTS_AGAIN 42 // SPSA playfield and backbox
#define LAMP_CREDIT_INDICATOR 43         // Credit Indicator

#define LAMP_2X_BONUS 44      // Super Bonus
#define LAMP_3X_BONUS 45 // 5X Bonus
#define LAMP_5X_BONUS 46      // 3X Bonus
#define LAMP_GLOBE_SPECIAL 47      // 2X Bonus

// Byte 6
#define LAMP_BALL_IN_PLAY 48 // Ball in play - not working
#define LAMP_HIGH_SCORE 49   // High Score
#define LAMP_GAME_OVER 50    // Game Over
#define LAMP_TILT 51         // Tilt

#define LAMP_BONUS_10K 52 // 1 PLAYER
#define LAMP_BONUS_20K 53 // 2 PLAYER
#define LAMP_BONUS_30K 54 // 3 PLAYER
#define LAMP_BONUS_40K 55 // 4 PLAYER

// Byte 7
#define LAMP_CENTER_G 56 // PLAYER 1 Up
#define LAMP_CENTER_L 57 // PLAYER 2 Up
#define LAMP_CENTER_O 58 // PLAYER 3 Up
#define LAMP_CENTER_B 59 // PLAYER 4 Up
// End of Lamp Matrix

//
// Lamps Collections
//

#define LAMP_COLL_BONUS_LAMPS 1
#define LAMP_COLL_CENTER_GLOBE 2
#define LAMP_COLL_SAUCER_GLOBE 3
#define LAMP_COLL_SIDE_TARGET 4
#define LAMP_COLL_SPINNER_VALUES 5
#define LAMP_COLL_CENTER_SPINNER 6
#define LAMP_COLL_DUNK_SHOT 7

//
//  Lamp Animations
//

#define NUM_LAMP_ANIMATIONS 4
#define LAMP_ANIMATION_STEPS 24
#define NUM_LAMP_ANIMATION_BYTES 8

#define ANIM_ANIMATION1 0
#define ANIM_ANIMATION2 1
#define ANIM_ANIMATION3 2
#define ANIM_ANIMATION4 3

namespace LampsHelper {
void hideAllLamps();
void hideAllPlayfieldLamps();
void hideLamp(byte lamp);
void hideLampFromCollection(byte lampCollection, byte indexToShow);
void showLamp(byte lamp, byte dim = 0, byte flashPeriod = 0);
void hideLamps(byte lampCollection);
void showLamps(byte lampCollection, byte flashPeriod = 0);
void showLampFromCollection(byte lampCollection, byte indexToShow, byte dim = 0, byte flashPeriod = 0);
void showOneLampFromCollection(byte lampCollection, byte indexToShow, byte flashPeriod = 0);
void showAllLamps(int flashPeriod = 0);
void showBonusLamps(byte bonusValue);
void showBonusMultiplierLamps(byte value, byte flashPeriod = 0);

// Lamps animations
void showLampAnimation(const byte animationNum[][8], byte frames, Time::time_t divisor, byte subOffset, boolean dim, boolean reverse = false, byte keepLampOn = 99, boolean auxLamps = false);
void sweepLampCollection(byte lampColNum, byte stepTime, byte offset);
void fillLampCollection(byte lampColNum, byte stepTime);
void glowLamp(byte lamp, int stepTime);

void setGameAnimationStatus(byte animationNum, boolean status);
void runGameAnimations();

} // namespace LampsHelper
#endif
