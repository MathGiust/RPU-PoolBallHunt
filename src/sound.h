//
// Created by Mathias on 27/07/2024.
//

#include "Arduino.h"
#include "PinballMachineBase.h"

#ifndef SOUND_H
#define SOUND_H

// Define Sound effects Number
#define SOUND_EFFECT_STARTUP 0
#define SOUND_EFFECT_GAME_START 1
#define SOUND_EFFECT_10000_CHIME 2
#define SOUND_EFFECT_10_CHIME 3
#define SOUND_EFFECT_100_CHIME 4
#define SOUND_EFFECT_1000_CHIME 5
#define SOUND_EFFECT_MODE_START 6
#define SOUND_EFFECT_INCREASE_SPINNER 7

namespace ChimesHelper {
void PlayChimesSoundEffect(byte soundEffectNum, boolean priority);
}

#endif // SOUND_H
