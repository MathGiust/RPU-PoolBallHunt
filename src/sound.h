//
// Created by Mathias on 27/07/2024.
//

#ifndef SOUND_H
#define SOUND_H

#include "PinballMachineBase.h"
#include "Time.h"
#include "Arduino.h"

// Define Sound effects Number
#define SOUND_EFFECT_STARTUP 0
#define SOUND_EFFECT_GAME_START 1
#define SOUND_EFFECT_SPINNER_1 2
#define SOUND_EFFECT_SPINNER_2 3
#define SOUND_EFFECT_SPINNER_3 4
#define SOUND_EFFECT_SLING 5
#define SOUND_EFFECT_BUMPER 6
#define SOUND_EFFECT_INCREASE_SPINNER 7

namespace SoundHelper {
void playSoundEffect(byte soundEffectName);

// Sound stack functions
void pushToSoundStack(unsigned short soundIndex);
bool pushToTimedSoundStack(unsigned short soundIndex, bool address_E_Value, Time::time_t whenToPlay);
void UpdateTimedSoundStack(unsigned long currentTime);
}

#define SOUND_EFFECT_BACKGROUND_PLAY 26
#define SOUND_EFFECT_BACKGROUND_STOP 27
#define SOUND_EFFECT_BACKGROUND_AUGMENT 25

#endif // SOUND_H
