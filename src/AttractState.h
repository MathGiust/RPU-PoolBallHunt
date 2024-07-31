/*********************************************************************


    Attract Header File
    Version : 1.00


*********************************************************************/

#ifndef ATTRACT_STATE_H
#define ATTRACT_STATE_H

#include "MachineState.h"
#include "Arduino.h"

#define FEATURE_SHOW_TIMEOUT 60000
#define TAUNT_TIMEOUT 15000

namespace Attract {
int run(boolean currentStateChanged, MachineState& machineState);
}

#endif
