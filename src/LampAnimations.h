/*********************************************************************


LampAnimation Source File
    Version : 0.10


*********************************************************************/

#ifndef LAMP_ANIMATIONS_H
#define LAMP_ANIMATIONS_H

#include "Lamps.h"
#include "Time.h"

#include <Arduino.h>

#define NUMBER_OF_LAMP_ANIMATIONS 4

// Animation name
#define ANIM_BONUS_X_SWEEP 0
#define ANIM_SAUCER_GLOBE 1
#define ANIM_CENTER_GLOBE 2
#define ANIM_UNUSED_4 3

namespace AnimationHelper {

//   Functions
void startLampAnimation(uint8_t animationNumber, Time::time_t animationDuration, Time::time_t startTime);
void stopAnimation(uint8_t animationNumber);
void stopAllAnimations();
void updateAnimations(Time::time_t currentTime);
}



//
//  Attract animations
//

/*
typedef struct {
    byte lampNum;
    byte lampState;
    byte lampDim;
} AnimationLampChange;


#define LAMP(lamp, state, dim) { lamp, state, dim }

#define STEP(name, num, ...) const AnimationLampChange step_##name##_etape_##num[] = { __VA_ARGS__ }

#define ANIM(name, ...)

STEP(animation3, 1,
     LAMP(LAMP_SUPER_BONUS, 1, 1),
     LAMP(LAMP_BIG_1, 1, 0)
);

ANIM(animation3,
     STEP(
          LAMP(LAMP_SUPER_BONUS, 1, 1),
          LAMP(LAMP_BIG_1, 1, 0)
     ),
     STEP(
          LAMP(LAMP_SUPER_BONUS, 1, 1),
          LAMP(LAMP_BIG_1, 1, 0)
     )
)

*/

#endif
