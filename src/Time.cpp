/*********************************************************************


    Time Source File
    Version : 1.10


*********************************************************************/

#include "Time.h"

#include <Arduino.h>

static Time::time_t currentTime = 0;
static Time::time_t mostRecentSwitchHitTime = 0;
static Time::time_t lastTiltWarningTime = 0;
static Time::time_t ballEnteredTroughTime = 0;
static Time::time_t lastOutholeKickTime = 0;
static Time::time_t lastScoringTime = 0;
static Time::time_t lastBonusPointsTime = 0;
static Time::time_t lastTestCycleTime = 0;
static Time::time_t lastModeTimerUpdateTime = 0;
static Time::time_t lastSubModeTimerUpdateTime = 0;
static Time::time_t lastLampAnimUpdateTime = 0;
static Time::time_t lastAttractScoreUpdateTime = 0;
static Time::time_t lastSoundPlayedTime = 0;

//
//  Get
//

Time::time_t Time::getCurrentTime() {
    return currentTime;
}
Time::time_t Time::getMostRecentSwitchHitTime() {
    return mostRecentSwitchHitTime;
}
Time::time_t Time::getLastTiltWarningTime() {
    return mostRecentSwitchHitTime;
}
Time::time_t Time::getBallEnteredTroughTime() {
    return ballEnteredTroughTime;
}
Time::time_t Time::getLastOutholeKickTime() {
    return lastOutholeKickTime;
}
Time::time_t Time::getLastScoringTime() {
    return lastScoringTime;
}
Time::time_t Time::getLastBonusPointsTime() {
    return lastBonusPointsTime;
}
Time::time_t  Time::getLastTestCycleTime() {
    return lastTestCycleTime;
}
Time::time_t Time::getLastModeTimerUpdateTime() {
    return lastModeTimerUpdateTime;
}
Time::time_t Time::getLastSubModeTimerUpdateTime() {
    return lastSubModeTimerUpdateTime;
}
Time::time_t Time::getLastLampAnimUpdateTime() {
    return lastLampAnimUpdateTime;
}
Time::time_t Time::getLastAttractScoreUpdateTime() {
    return lastAttractScoreUpdateTime;
}
Time::time_t Time::getLastSoundPlayedTime() {
    return lastSoundPlayedTime;
}


//
//  Update
//

void Time::updateCurrentTime() {
    currentTime = millis();
}
void Time::updateMostRecentSwitchHitTime() {
    mostRecentSwitchHitTime = currentTime;
}
void Time::updateLastTiltWarningTime() {
    lastTiltWarningTime = currentTime;
}
void Time::updateBallEnteredTroughTime() {
    ballEnteredTroughTime = currentTime;
}
void Time::updateLastOutholeKickTime() {
    lastOutholeKickTime = currentTime;
}
void Time::updateLastScoringTime() {
    lastScoringTime = currentTime;
}
void Time::updateLastBonusPointsTime() {
    lastBonusPointsTime = currentTime;
}
void Time::updateLastTestCycleTime() {
    lastTestCycleTime = currentTime;
}
void Time::updateLastModeTimerUpdateTime() {
    lastModeTimerUpdateTime = currentTime;
}
void Time::updateLastSubModeTimerUpdateTime() {
    lastSubModeTimerUpdateTime = currentTime;
}
void Time::updateLastLampAnimUpdateTime() {
    lastLampAnimUpdateTime = currentTime;
}
void Time::updateLastAttractScoreUpdateTime() {
    lastAttractScoreUpdateTime = currentTime;
}
void Time::updateLastSoundPlayedTime() {
    lastSoundPlayedTime = currentTime;
}


//
//  Reset
//

void Time::resetBallEnteredTroughTime() {
    ballEnteredTroughTime = 0;
}
void Time::resetLastOutholeKickTime() {
    lastOutholeKickTime = 0;
}