/*********************************************************************


    Time Header File
    Version : 1.00


*********************************************************************/

#ifndef TIME_H
#define TIME_H

namespace Time {

typedef unsigned long time_t;

time_t getCurrentTime();
time_t getMostRecentSwitchHitTime();
time_t getLastTiltWarningTime();
time_t getBallEnteredTroughTime();
time_t getLastOutholeKickTime();
time_t getLastScoringTime();
time_t getLastBonusPointsTime();
time_t getLastTestCycleTime();
time_t getLastModeTimerUpdateTime();
time_t getLastSubModeTimerUpdateTime();
time_t getLastLampAnimUpdateTime();
time_t getLastAttractScoreUpdateTime();

void updateCurrentTime();
void updateMostRecentSwitchHitTime();
void updateLastTiltWarningTime();
void updateBallEnteredTroughTime();
void updateLastOutholeKickTime();
void updateLastScoringTime();
void updateLastBonusPointsTime();
void updateLastTestCycleTime();
void updateLastModeTimerUpdateTime();
void updateLastSubModeTimerUpdateTime();
void updateLastLampAnimUpdateTime();
void updateLastAttractScoreUpdateTime();

void resetBallEnteredTroughTime();
void resetLastOutholeKickTime();

} // namespace Time

#endif
