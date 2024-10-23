/*********************************************************************


    Test/Audit/Adjust Header File
    Version : 1.00


*********************************************************************/

#ifndef SELF_TEST_H
#define SELF_TEST_H

#include <Arduino.h>

namespace SelfTestAndAudit {
int run(int currentState, boolean curStateChanged);
}


// TESTS :
#define MACHINE_STATE_TEST_LAMPS (-1)
#define MACHINE_STATE_TEST_DISPLAYS (-2)
#define MACHINE_STATE_TEST_SOLENOIDS (-3)
#define MACHINE_STATE_TEST_SWITCHES (-4)
#define MACHINE_STATE_TEST_SOUNDS (-5)

// AUDITS :
#define MACHINE_STATE_AUDIT_CREDITS (-6)
#define MACHINE_STATE_AUDIT_TOTAL_PLAYS (-7)
#define MACHINE_STATE_AUDIT_TOTAL_REPLAYS (-8)
#define MACHINE_STATE_AUDIT_HISCR_BEAT (-9)
#define MACHINE_STATE_AUDIT_CHUTE_1_COINS (-10)
#define MACHINE_STATE_AUDIT_CHUTE_2_COINS (-11)
#define MACHINE_STATE_AUDIT_CHUTE_3_COINS (-12)

// ADJUST :
#define MACHINE_STATE_ADJ_SCORE_LEVEL_1 (-13)
#define MACHINE_STATE_ADJ_SCORE_LEVEL_2 (-14)
#define MACHINE_STATE_ADJ_SCORE_LEVEL_3 (-15)
#define MACHINE_STATE_ADJ_HISCR (-16)
#define MACHINE_STATE_ADJ_FREEPLAY (-17)
#define MACHINE_STATE_ADJ_BALL_SAVE (-18)
#define MACHINE_STATE_ADJ_NUM_TILT_WARN (-19)

// GAME SPECIFIC ADJUST :
// TODO : Define game specific adjust



#define MACHINE_STATE_TEST_DONE -100
#endif
