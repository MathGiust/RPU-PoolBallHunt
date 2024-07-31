/*********************************************************************


    Scoring Header File
    Version : 1.00


*********************************************************************/

#ifndef SCORING_H
#define SCORING_H

#include "MachineState.h"

#include "Arduino.h"

namespace Scoring{
void addToTensStack(byte amountToAdd);
void addToHundredStack(byte amountToAdd);
void addToThousandsStack(byte amountToAdd);
void addToTensOfThousandStack(byte amountToAdd);
void increaseCurrentPlayerScore(score_t valueToAdd, MachineState& machineState);

byte getToTensStack(byte amountToAdd);
byte getToHundredStack(byte amountToAdd);
byte getToThousandsStack(byte amountToAdd);
byte getToTensOfThousandStack(byte amountToAdd);

void scoring(MachineState& machineState);
void emptyAllStacks();
}


#endif
