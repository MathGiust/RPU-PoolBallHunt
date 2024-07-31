#define DEBUG_MESSAGES 1
#define EXECUTION_MESSAGES 1

#define GAME_MAJOR_VERSION 01
#define GAME_MINOR_VERSION 00

// Define number of Switches
#define NUM_OF_SWITCHES 26

// Defines for SWITCHES
#define SW_10_PTS 0
#define SW_TOP_LEFT_TARGET 1
#define SW_BANK_SHOT 2
#define SW_CREDIT_BUTTON 5
#define SW_TILT 6
#define SW_OUTHOLE 7
#define SW_COIN_3 9
#define SW_COIN_1 8
#define SW_COIN_2 10
#define SW_SLAM 15
#define SW_SPINNER 16
#define SW_8_BALL 24
#define SW_7_15_BALL 25
#define SW_6_14_BALL 26
#define SW_5_13_BALL 27
#define SW_4_12_BALL 28
#define SW_3_11_BALL 29
#define SW_2_10_BALL 30
#define SW_1_9_BALL 31
#define SW_RIGHT_OUTLANE 32
#define SW_LEFT_OUTLANE 33
#define SW_RIGHT_SLING 35
#define SW_LEFT_SLING 36
#define SW_BOTTOM_BUMPER 37
#define SW_RIGHT_BUMPER 38
#define SW_LEFT_BUMPER 39
// End of define for SWITCHES

// Defines for SOLENOIDS

#define SOL_CHIME_10 1       // 10 Points chime
#define SOL_CHIME_100 2      // 100 Points chime
#define SOL_CHIME_1000 3     // 1000 Points chime
#define SOL_CHIME_10000 4    // 10000 Points chime
#define SOL_KNOCKER 5        // Knocker
#define SOL_OUTHOLE 6        // Outhole
#define SOL_KICKER 7         // Kickback (Breakshot)
#define SOL_LEFT_BUMPER 8    // Left Bumper
#define SOL_RIGHT_BUMPER 9   // Right Bumper
#define SOL_BOTTOM_BUMPER 10 // Bottom Bumper
#define SOL_LEFT_SLING 11    // Left Sling
#define SOL_RIGHT_SLING 13   // Right Sling

#define NUMBER_OF_SOL 12     // Total number of solenoids
// End of defines for SOLENOIDS

// Define Solenoid Strength
#define SOL_KNOCKER_STRENGTH 7
#define SOL_OUTHOLE_KICKER_STRENGTH 3
#define SOL_SLING_STRENGTH 4
#define SOL_BUMPER_STRENGTH 3
#define SOL_CHIME_STRENGTH 2

// SWITCHES_WITH_TRIGGERS are for switches that will automatically
// activate a solenoid (like in the case of a chime that rings on a rollover)
// but SWITCHES_WITH_TRIGGERS are fully debounced before being activated
#define NUM_SWITCHES_WITH_TRIGGERS 5

// PRIORITY_SWITCHES_WITH_TRIGGERS are switches that trigger immediately
// (like for pop bumpers or slings) - they are not debounced completely
#define NUM_PRIORITY_SWITCHES_WITH_TRIGGERS 5

#define MACHINE_STATE_TEST_LIGHTS (-1)

//
// EEPROM indices location :
//

// Indices of EEPROM save locations
#define EEPROM_BALL_SAVE_BYTE 100
#define EEPROM_FREE_PLAY_BYTE 101
#define EEPROM_SOUND_SELECTOR_BYTE 102
#define EEPROM_SKILL_SHOT_BYTE 103
#define EEPROM_TILT_WARNING_BYTE 104
#define EEPROM_AWARD_OVERRIDE_BYTE 105
#define EEPROM_BALLS_OVERRIDE_BYTE 106
#define EEPROM_TOURNAMENT_SCORING_BYTE 107
#define EEPROM_SFX_VOLUME_BYTE 108
#define EEPROM_MUSIC_VOLUME_BYTE 109
#define EEPROM_SCROLLING_SCORES_BYTE 110
#define EEPROM_CALLOUTS_VOLUME_BYTE 111
#define EEPROM_CRB_HOLD_TIME 118
#define EEPROM_EXTRA_BALL_SCORE_UL 140
#define EEPROM_SPECIAL_SCORE_UL 144
