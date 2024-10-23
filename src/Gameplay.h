/*********************************************************************


    Gameplay Header File
    Version : 1.00


*********************************************************************/

#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "MachineState.h"
#include <Arduino.h>

namespace Gameplay {
int run(boolean currentStateChanged, MachineState& machineState);
void setGameMode(byte newGameMode);
}
#endif
