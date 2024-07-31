//
// Created by Mathias on 27/07/2024.
//

#include "sound.h"

#include "RPU.h"
#include "Time.h"

Time::time_t nextSoundEffectTime = 0;

const static int soundEffectDurations[]{
        150, 900, 100, 100, 100, 100, 500, 500
};

void ChimesHelper::PlayChimesSoundEffect(byte soundEffectNum, boolean priority) {
    Time::time_t startTime;

    if ((Time::getCurrentTime() > nextSoundEffectTime)) {
        startTime = Time::getCurrentTime();
    } else {
        if (DEBUG_MESSAGES) Serial.write("Sound Effect skipped\n\r");
        if(!priority) return;
        startTime = nextSoundEffectTime + 250;
    }
    nextSoundEffectTime = startTime + soundEffectDurations[soundEffectNum];

    switch (soundEffectNum) {
    case SOUND_EFFECT_STARTUP: // SFX 0 -- DUR 150
        RPU_PushToTimedSolenoidStack(SOL_CHIME_10000, SOL_CHIME_STRENGTH, startTime + 0, true);
        RPU_PushToTimedSolenoidStack(SOL_CHIME_1000, SOL_CHIME_STRENGTH, startTime + 150, true);
        break;
    case SOUND_EFFECT_GAME_START: // SFX 1 -- DUR 900
        RPU_PushToTimedSolenoidStack(SOL_CHIME_10, SOL_CHIME_STRENGTH, startTime + 0, true);
        RPU_PushToTimedSolenoidStack(SOL_CHIME_100, SOL_CHIME_STRENGTH, startTime + 150, true);
        RPU_PushToTimedSolenoidStack(SOL_CHIME_1000, SOL_CHIME_STRENGTH, startTime + 300, true);
        RPU_PushToTimedSolenoidStack(SOL_CHIME_100, SOL_CHIME_STRENGTH, startTime + 450, true);
        RPU_PushToTimedSolenoidStack(SOL_CHIME_1000, SOL_CHIME_STRENGTH, startTime + 750, true);
        RPU_PushToTimedSolenoidStack(SOL_CHIME_10, SOL_CHIME_STRENGTH, startTime + 900, true);
        break;
    case SOUND_EFFECT_10000_CHIME: // SFX 2 -- DUR 100
        RPU_PushToTimedSolenoidStack(SOL_CHIME_10000, SOL_CHIME_STRENGTH, startTime + 0, true);
        break;
    case SOUND_EFFECT_10_CHIME: // SFX 3 -- DUR 100
        RPU_PushToTimedSolenoidStack(SOL_CHIME_10, SOL_CHIME_STRENGTH, startTime + 0, true);
        break;
    case SOUND_EFFECT_100_CHIME: // SFX 4 -- DUR 100
        RPU_PushToTimedSolenoidStack(SOL_CHIME_100, SOL_CHIME_STRENGTH, startTime + 0, true);
        break;
    case SOUND_EFFECT_1000_CHIME: // SFX 5 -- DUR 100
        RPU_PushToTimedSolenoidStack(SOL_CHIME_1000, SOL_CHIME_STRENGTH, startTime + 0, true);
        break;
    case SOUND_EFFECT_MODE_START: // SFX 6 -- DUR 500
        RPU_PushToTimedSolenoidStack(SOL_CHIME_100, SOL_CHIME_STRENGTH, startTime + 0, true);
        RPU_PushToTimedSolenoidStack(SOL_CHIME_1000, SOL_CHIME_STRENGTH, startTime + 100, true);
        RPU_PushToTimedSolenoidStack(SOL_CHIME_100, SOL_CHIME_STRENGTH, startTime + 200, true);
        RPU_PushToTimedSolenoidStack(SOL_CHIME_10, SOL_CHIME_STRENGTH, startTime + 300, true);
        RPU_PushToTimedSolenoidStack(SOL_CHIME_100, SOL_CHIME_STRENGTH, startTime + 400, true);
        RPU_PushToTimedSolenoidStack(SOL_CHIME_1000, SOL_CHIME_STRENGTH, startTime + 500, true);
        break;
    case SOUND_EFFECT_INCREASE_SPINNER: // SFX 7 -- DUR 200
        RPU_PushToTimedSolenoidStack(SOL_CHIME_1000, SOL_CHIME_STRENGTH, startTime + 0, true);
        RPU_PushToTimedSolenoidStack(SOL_CHIME_100, SOL_CHIME_STRENGTH, startTime + 100, true);
        RPU_PushToTimedSolenoidStack(SOL_CHIME_10, SOL_CHIME_STRENGTH, startTime + 200, true);
    default:
        break;
    }
}
