/*********************************************************************


    Gameplay Header File
    Version : 1.00


*********************************************************************/

#include "MachineState.h"

#include <Arduino.h>

#ifndef GAMEPLAY_H
#define GAMEPLAY_H

namespace Gameplay {
int run(boolean currentStateChanged, MachineState& machineState);
void setGameMode(byte newGameMode);
}

#endif
