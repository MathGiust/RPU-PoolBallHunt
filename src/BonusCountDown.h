/*********************************************************************


    Bonus Count Header File
    Version : 1.00


*********************************************************************/

#ifndef BONUS_H
#define BONUS_H

#include "MachineState.h"
#include <Arduino.h>

namespace BonusCountDown {
byte run(boolean curStateChanged, MachineState& machineState);
}

#endif
