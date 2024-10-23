//
// Created by Mathias on 27/07/2024.
//

#include "sound.h"

#include "AudioHandler.h"
#include "RPU.h"
#include "Time.h"

#include <RPU.h>

Time::time_t nextSoundEffectTime = 0;

//  Sound stack
#define SOUND_STACK_SIZE 64
#define SOUND_STACK_EMPTY 0x0000
volatile byte         soundStackFirst;
volatile byte         soundStackLast;
static unsigned short soundStack[SOUND_STACK_SIZE];

#define TIMED_SOUND_STACK_SIZE 33
struct TimedSoundEntry {
    byte           inUse;
    unsigned short soundNumber;
    bool           address_E;
    Time::time_t   pushTime;
};

// Return the space available in the sound stack
static int spaceLeftOnSoundStack() {
    if (soundStackFirst >= SOUND_STACK_SIZE || soundStackLast >= SOUND_STACK_SIZE) return 0;
    if (soundStackLast >= soundStackFirst) return ((SOUND_STACK_SIZE - 1) - (soundStackLast - soundStackFirst));
    return (soundStackFirst - soundStackLast) - 1;
}

static unsigned short pullFirstFromSoundStack() {
    // If first and last are equal, there's nothing on the stack
    if (soundStackFirst == soundStackLast) {
        return SOUND_STACK_EMPTY;
    }

    unsigned short retVal = soundStack[soundStackFirst];

    soundStackFirst += 1;
    if (soundStackFirst >= SOUND_STACK_SIZE) soundStackFirst = 0;

    return retVal;
}

// Initialize stack variables
static TimedSoundEntry timedSoundStack[TIMED_SOUND_STACK_SIZE];

void SoundHelper::pushToSoundStack(unsigned short soundIndex) {
    // If the solenoid stack last index is out of range, then it's an error - return
    if (spaceLeftOnSoundStack() == 0) return;
    if (soundIndex > 15) return;

    soundStack[soundStackLast] = soundIndex;

    soundStackLast += 1;
    if (soundStackLast == SOUND_STACK_SIZE) {
        // If the end index is off the end, then wrap
        soundStackLast = 0;
    }
    // If the stack is now full, return
    if (spaceLeftOnSoundStack() == 0) return;
}

// Push to timed sound --> plays 1 chimes at a give time
bool SoundHelper::pushToTimedSoundStack(const unsigned short soundIndex, bool address_E_Value, const Time::time_t whenToPlay) {
    for (int count = 0; count < TIMED_SOUND_STACK_SIZE; count++) {
        if (!timedSoundStack[count].inUse) {
            timedSoundStack[count].inUse = true;                // Is in use
            timedSoundStack[count].soundNumber = soundIndex;    // Sound index to play
            timedSoundStack[count].address_E = address_E_Value; // Enable/Disable address E
            timedSoundStack[count].pushTime = whenToPlay;       // When to play the sound
            return true;
        }
    }
    return false;
}

// Update the timed sound stack --> Executed at the end of the main loop
void SoundHelper::UpdateTimedSoundStack(unsigned long currentTime) {
    for (int count = 0; count < TIMED_SOUND_STACK_SIZE; count++) {
        if (timedSoundStack[count].inUse && timedSoundStack[count].pushTime < currentTime) {
            RPU_PlaySoundDash32(timedSoundStack[count].soundNumber, timedSoundStack[count].address_E);
            timedSoundStack[count].inUse = false;
        }
    }
}

void SoundHelper::playSoundEffect(byte soundEffectName) {
    Time::time_t currentTime = Time::getCurrentTime();
    byte         time_i = 0;
    uint16_t     baseTime = 30;
    uint8_t      subTime = 35;
    switch (soundEffectName) {
    default:
        // Silence
        pushToTimedSoundStack(15, false, currentTime);
        break;
    case SOUND_EFFECT_STARTUP:

        pushToTimedSoundStack(5, false, currentTime + baseTime * 0);
        pushToTimedSoundStack(9, false, currentTime + baseTime * 1);
        pushToTimedSoundStack(3, false, currentTime + baseTime * 2);
        pushToTimedSoundStack(7, false, currentTime + baseTime * 3);

        pushToTimedSoundStack(11, false, currentTime + baseTime * 4);
        pushToTimedSoundStack(5, false, currentTime + baseTime * 5);
        pushToTimedSoundStack(9, false, currentTime + baseTime * 6);
        pushToTimedSoundStack(13, false, currentTime + baseTime * 7);

        pushToTimedSoundStack(7, false, currentTime + baseTime * 8);
        pushToTimedSoundStack(11, false, currentTime + baseTime * 9);
        pushToTimedSoundStack(0, true, currentTime + baseTime * 10);
        pushToTimedSoundStack(9, false, currentTime + baseTime * 11);

        pushToTimedSoundStack(13, false, currentTime + baseTime * 12);
        pushToTimedSoundStack(2, true, currentTime + baseTime * 13);
        pushToTimedSoundStack(11, false, currentTime + baseTime * 14);
        pushToTimedSoundStack(0, true, currentTime + baseTime * 15);

        pushToTimedSoundStack(4, true, currentTime + baseTime * 16);
        pushToTimedSoundStack(13, false, currentTime + baseTime * 17);
        pushToTimedSoundStack(2, true, currentTime + baseTime * 18);
        pushToTimedSoundStack(6, true, currentTime + baseTime * 19);

        pushToTimedSoundStack(0, true, currentTime + baseTime * 20);
        pushToTimedSoundStack(4, true, currentTime + baseTime * 21);
        pushToTimedSoundStack(8, true, currentTime + baseTime * 22);
        pushToTimedSoundStack(2, true, currentTime + baseTime * 23);

        pushToTimedSoundStack(6, true, currentTime + baseTime * 24);
        pushToTimedSoundStack(10, true, currentTime + baseTime * 25);
        pushToTimedSoundStack(4, true, currentTime + baseTime * 26);
        pushToTimedSoundStack(8, true, currentTime + baseTime * 27);

        pushToTimedSoundStack(12, true, currentTime + baseTime * 28);
        pushToTimedSoundStack(6, true, currentTime + baseTime * 29);
        pushToTimedSoundStack(10, true, currentTime + baseTime * 30);
        pushToTimedSoundStack(14, true, currentTime + baseTime * 31);

        pushToTimedSoundStack(15, true, currentTime + baseTime * 32);

        // High notes

        break;

    case SOUND_EFFECT_GAME_START:
        pushToTimedSoundStack(0, true, currentTime);
        break;

    case SOUND_EFFECT_SPINNER_1:
        pushToTimedSoundStack(8, false, currentTime);
        pushToTimedSoundStack(14, false, currentTime + 20);
        pushToTimedSoundStack(4, true, currentTime + 40);
        pushToTimedSoundStack(15, true, currentTime + 60);
        break;

    case SOUND_EFFECT_SPINNER_2:
        pushToTimedSoundStack(12, false, currentTime);
        pushToTimedSoundStack(2, true, currentTime + 20);
        pushToTimedSoundStack(6, true, currentTime + 40);
        pushToTimedSoundStack(15, true, currentTime + 60);
        break;

    case SOUND_EFFECT_SPINNER_3:
        pushToTimedSoundStack(14, false, currentTime);
        pushToTimedSoundStack(2, true, currentTime + 20);
        pushToTimedSoundStack(5, true, currentTime + 40);
        pushToTimedSoundStack(2, true, currentTime + 60);
        pushToTimedSoundStack(15, true, currentTime + 80);
        break;

    case SOUND_EFFECT_SLING:
        for (uint8_t i = 0; i < 8; i++) {
            pushToTimedSoundStack(12 - i, true, currentTime + 15 * i);
        }
        pushToTimedSoundStack(15, true, currentTime + 15 * 9);
        break;
    }
}
