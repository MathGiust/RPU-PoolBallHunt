/*********************************************************************


    Lamps Header File
    Version : 1.00


*********************************************************************/

#include "Arduino.h"
#include "Time.h"

#ifndef LAMPS_H
#define LAMPS_H

#define FLASH_PERIOD 100
#define NUMBER_OF_LAMPS 59

// Define Lamp Matrix
#define LAMP_TERMINATOR 255 // Collection Terminator
#define LAMP_UNUSED 254

// Byte 0
#define LAMP_SMALL_1 0 // Small 1
#define LAMP_SMALL_2 1 // Small 2
#define LAMP_SMALL_3 2 // Small 3
#define LAMP_SMALL_4 3 // Small 4

#define LAMP_SMALL_5 4 // Small 5
#define LAMP_SMALL_6 5 // Small 6
#define LAMP_SMALL_7 6 // Small 7
#define LAMP_SMALL_8 7 // Small 8

// Byte 1
#define LAMP_SMALL_9 8   // Small 9
#define LAMP_SMALL_10 9  // Small 10
#define LAMP_SMALL_11 10 // Small 11
#define LAMP_SMALL_12 11 // Small 12

#define LAMP_SMALL_13 12   // Small 13
#define LAMP_SMALL_14 13   // Small 14
#define LAMP_SMALL_15 14   // Small 15
#define LAMP_LEFT_ARROW 15 // Left Arrow

// Byte 2
#define LAMP_BANK_SHOT_300 16  // Bank Shot 300 1
#define LAMP_BANK_SHOT_600 17  // Bank Shot 600 2
#define LAMP_BANK_SHOT_900 18  // Bank Shot 900 3
#define LAMP_BANK_SHOT_1200 19 // Bank Shot 1200 4

#define LAMP_BANK_SHOT_1500 20  // Bank Shot 1500 5
#define LAMP_BANK_SHOT_5000 21  // Bank Shot 5000 6
#define LAMP_OUTLANE_SPECIAL 22 // OutLAMPne Special
#define LAMP_SHOOT_AGAIN 23     // Same PLAYER Shoots Again - Playfield

// Byte 3
#define LAMP_BIG_1 24 // Big 1
#define LAMP_BIG_2 25 // Big 2
#define LAMP_BIG_3 26 // Big 3
#define LAMP_BIG_4 27 // Big 4

#define LAMP_BIG_5 28 // Big 5
#define LAMP_BIG_6 29 // Big 6
#define LAMP_BIG_7 30 // Big 7
#define LAMP_BIG_8 31 // Big 8

// Byte 4
#define LAMP_BIG_9 32  // Big 9
#define LAMP_BIG_10 33 // Big 10
#define LAMP_BIG_11 34 // Big 11
#define LAMP_BIG_12 35 // Big 12

#define LAMP_BIG_13 36      // Big 13
#define LAMP_BIG_14 37      // Big 14
#define LAMP_BIG_15 38      // Big 15
#define LAMP_RIGHT_ARROW 39 // Right Arrow

// Byte 5
#define LAMP_SAME_PLAYER_SHOOTS_AGAIN 40 // Same Player Shoots Again - Back Box
#define LAMP_MATCH 41                    // Match
#define LAMP_SPINNER 42                  // Spinner and Kickback Arrow
#define LAMP_CREDIT_INDICATOR 43         // Credit Indicator

#define LAMP_SUPER_BONUS 44 // Super Bonus
#define LAMP_5X_BONUS 45    // 5X Bonus
#define LAMP_3X_BONUS 46    // 3X Bonus
#define LAMP_2X_BONUS 47    // 2X Bonus

// Byte 6
#define LAMP_BALL_IN_PLAY 48 // Ball in play - not working
#define LAMP_HIGH_SCORE 49   // High Score
#define LAMP_GAME_OVER 50    // Game Over
#define LAMP_TILT 51         // Tilt

#define LAMP_1_PLAYER 52 // 1 PLAYER
#define LAMP_2_PLAYER 53 // 2 PLAYER
#define LAMP_3_PLAYER 54 // 3 PLAYER
#define LAMP_4_PLAYER 55 // 4 PLAYER

// Byte 7
#define PLAYER_1_UP 56 // PLAYER 1 Up
#define PLAYER_2_UP 57 // PLAYER 2 Up
#define PLAYER_3_UP 58 // PLAYER 3 Up
#define PLAYER_4_UP 59 // PLAYER 4 Up
// End of Lamp Matrix

//
// Lamps Collections
//

#define LAMP_COLL_BONUS_ALL 0
#define LAMP_COLL_BIG_BALLS 1
#define LAMP_COLL_SMALL_BALLS 2
#define LAMP_COLL_EVEN_PLAYER_BIG_BALLS 3
#define LAMP_COLL_ODD_PLAYER_BIG_BALLS 4
#define LAMP_COLL_EVEN_PLAYER_SMALL_BALLS 5
#define LAMP_COLL_ODD_PLAYER_SMALL_BALLS 6
#define LAMP_COLL_UPPER_ALLEY_BALLS 7
#define LAMP_COLL_BANK_SHOT_ALL 8
#define LAMP_COLL_BANK_SHOT_ALLEY 9
#define LAMP_COLL_BANK_SHOT_ARROWS 10
#define LAMP_COLL_BONUS_X 11
#define LAMP_COLL_BONUS_DESCENDING 12
#define LAMP_COLL_BOTTOM_HEAD 13

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
