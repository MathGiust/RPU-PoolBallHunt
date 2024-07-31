/*********************************************************************


    Bonus Count Source File
    Version : 1.00


*********************************************************************/

#include "BonusCountDown.h"

#include "Display.h"
#include "Lamps.h"
#include "PinballMachineBase.h"
#include "RPU.h"
#include "Scoring.h"
#include "Time.h"

static byte countDownBonusValue = 0;

byte BonusCountDown::run(boolean curStateChanged, MachineState& machineState) {
    PlayerState* currentPlayer = machineState.currentPlayer;
    Time::time_t timeToWaitBeforeCount = 175;


    if (curStateChanged) {
        if (DEBUG_MESSAGES) Serial.write("Entering Bonus Countdown :\n\r");
        countDownBonusValue = currentPlayer->getBonus();
    }

    // Check if bonus count down is finished
    if (countDownBonusValue == 0) {
        if (currentPlayer->getBonusMultiplier() == 1) return MACHINE_STATE_BALL_OVER;
        else {
            currentPlayer->decreaseBonusMultiplier();
            countDownBonusValue = currentPlayer->getBonus();
            LampsHelper::showBonusLamps(countDownBonusValue);
        }
    } else {
        // TODO implement bonus countdown :
        byte chimeToFire = SOL_CHIME_10000;
        switch (currentPlayer->getBonusMultiplier()) {
        case 1:
            chimeToFire = SOL_CHIME_10000;
            timeToWaitBeforeCount = 175;
            break;
        case 2:
            chimeToFire = SOL_CHIME_10;
            timeToWaitBeforeCount = 150;
            break;
        case 3:
            chimeToFire = SOL_CHIME_100;
            timeToWaitBeforeCount = 115;
            break;
        case 4:
        case 5:
            chimeToFire = SOL_CHIME_1000;
            timeToWaitBeforeCount = 100;
            break;
        }

        // wait before counting down
        if (Time::getCurrentTime() - Time::getLastBonusPointsTime() > timeToWaitBeforeCount) {
            Time::updateLastBonusPointsTime();
            currentPlayer->increaseScore(1000);
            countDownBonusValue -= 1;

            RPU_PushToTimedSolenoidStack(chimeToFire, SOL_CHIME_STRENGTH, Time::getCurrentTime());
            LampsHelper::showBonusLamps(countDownBonusValue);
        }
    }

    DisplayHelper::showPlayerScore(machineState.getCurrentPlayerNumber(), currentPlayer->getScore());

    return MACHINE_STATE_COUNTDOWN_BONUS;
}

//
//  Private :
//
