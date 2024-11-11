//
// Created by Mathias on 27/07/2024.
//

#ifndef SOUND_H
#define SOUND_H

#include "Arduino.h"
#include "PinballMachineBase.h"
#include "Time.h"

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
} // namespace SoundHelper

//
//  DASH 51 Sound indexes
//

#define DASH51_CHIME_1000 0
#define DASH51_CHIME_100 1
#define DASH51_CHIME_10 2
#define DASH51_CHIME_10000 3
#define DASH51_SNARE_NOISE 4
#define DASH51_LONG_NOISE_1 5
#define DASH51_HIGH_WHISTLE 6
#define DASH51_JINGLE_1 7
#define DASH51_LOW_PITCH_DROP_1 8
#define DASH51_LOW_PITCH_DROP_2 9
#define DASH51_LOW_PITCH_DROP_3 10
#define DASH51_JINGLE_1B 11
#define DASH51_FITH_MOD 12
#define DASH51_RS_RIFF_1 13
#define DASH51_RS_RIFF_2 14
#define DASH51_RISER_1 15
#define DASH51_RS_RIFF_3 16
#define DASH51_NOISE_BURST 17
#define DASH51_UNKNOWN_1 18
#define DASH51_SNARE_LONG 19
#define DASH51_SHORT_LOW_DRONE 20
#define DASH51_TRIPLE_DROP 21
#define DASH51_SHORT_HIGH_DRONE 22
#define DASH51_BACKGROUND_AUGMENT_1 23
#define DASH51_BACKGROUND_AUGMENT_2 24
#define DASH51_SYNC_NOISE_1 25
#define DASH51_SYNC_NOISE_2 26
#define DASH51_BACKGROUND_PLAY_HIGH 27
#define DASH51_WHISTLE_RISER 28
#define DASH51_BACKGROUND_STOP 29
#define DASH51_BACKGROUND_PLAY_LOW 30
#define DASH51_HIGH_PITCHED_MOD 31

#endif // SOUND_H
