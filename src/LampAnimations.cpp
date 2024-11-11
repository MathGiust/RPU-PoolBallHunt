/*********************************************************************


Lamp Animations Header File
    Version : 0.10


*********************************************************************/

#include "LampAnimations.h"

#define DEBUG_MESSAGES 1

typedef struct {
    bool         animationstatus;
    Time::time_t playDuration;
    Time::time_t startTime;
} LampAnimations;

LampAnimations lampAnimations[NUM_LAMP_ANIMATIONS] = {
        {false, 0, 0},
        {false, 0, 0},
        {false, 0, 0},
        {false, 0, 0}
};

void AnimationHelper::startLampAnimation(const uint8_t animationNumber, Time::time_t animationDuration, Time::time_t startTime) {
    if (DEBUG_MESSAGES) Serial.println("Animation started");
    lampAnimations[animationNumber].animationstatus = true;
    lampAnimations[animationNumber].playDuration = animationDuration;
    lampAnimations[animationNumber].startTime = startTime;
}

void AnimationHelper::stopAnimation(const uint8_t animationNumber) {
    if (DEBUG_MESSAGES) Serial.println("Animation stopped");
    lampAnimations[animationNumber].animationstatus = false;
    lampAnimations[animationNumber].playDuration = 0;
    lampAnimations[animationNumber].startTime = 0;
}

void AnimationHelper::stopAllAnimations() {
    for (uint8_t currentAnimation = 0; currentAnimation < NUM_LAMP_ANIMATIONS; currentAnimation++)
        stopAnimation(currentAnimation);
}

void AnimationHelper::updateAnimations(const Time::time_t currentTime) {
    for (uint8_t currentAnimation = 0; currentAnimation < NUM_LAMP_ANIMATIONS; currentAnimation++) {
        if (lampAnimations[currentAnimation].startTime + lampAnimations[currentAnimation].playDuration < currentTime) {
            stopAnimation(currentAnimation);
        }
    }

    if (lampAnimations[ANIM_BONUS_X_SWEEP].animationstatus) {
    }
    // End of animation 0

    if (lampAnimations[ANIM_SAUCER_GLOBE].animationstatus) {
        LampsHelper::sweepLampCollection(LAMP_COLL_SAUCER_GLOBE, 100, 1);
    }
    // End of animation 1

    if (lampAnimations[ANIM_CENTER_GLOBE].animationstatus) {
        LampsHelper::sweepLampCollection(LAMP_COLL_CENTER_GLOBE, 100, 1);
    }
    // End of animation 2

    if (lampAnimations[ANIM_UNUSED_4].animationstatus) {
    }
    // End of animation 3
}
