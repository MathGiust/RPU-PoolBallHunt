/*********************************************************************


    Game Modes Header File
    Version : 1.00


*********************************************************************/

#include "MachineState.h"

#include <Arduino.h>

#ifndef GAMEMODES_H
#define GAMEMODES_H

// Game modes ID
#define GAME_MODE_SKILL_SHOT 0
#define GAME_MODE_UNSTRUCTURED_PLAY 254
#define GAME_MODE_SPINNER 0
#define GAME_MODE_BUMPER 1
#define GAME_MODE_COMBO 2
#define GAME_MODE_INITIALIZE 255

class BaseMode {
private:
    int     modeTimer;
    score_t modeValue;

public:
    BaseMode();
    int             getModeTimer() const;
    virtual void    manageNewMode();
    virtual void    manageSwitchHit(byte switchHit, MachineState& machineState);
    virtual byte            manageModeTimer(byte currentGameMode);
    void            increaseModeTimer(int valueToAdd);
    void            decreaseModeTimer(int valueToAdd);
    void            setModeTimer(int value);
    virtual score_t getModeValue();
};

//
// Spinner mode :
//

class SpinnerMode : public BaseMode {
private:
    int currentSpinnerValue;

public:
    SpinnerMode();

    // Override
    void    manageNewMode() override;
    score_t getModeValue() override;
    void    manageSwitchHit(byte switchHit, MachineState& machineState) override;

    // Mode specific
    void increaseSpinnerValue();
    void resetSpinnerValue();
};

class PopMode : public BaseMode {
private:
    int     currentPopValue;
    score_t hurryUpValue;

public:
    PopMode();

    // Override
    void    manageNewMode() override;
    score_t getModeValue() override;
    void    manageSwitchHit(byte switchHit, MachineState& machineState) override;
    byte            manageModeTimer(byte currentGameMode) override;

    // Mode specific
    void    increasePopValue();
    void    increaseHurryUpValue();
    void    resetPopValue();
    score_t getPopValue() const;
};

class ComboMode {
private:
    int     modeTimer;
    int     comboTimer;
    score_t comboValue;

public:
    void increaseComboValue();
    void resetComboValue();

    // Getters :
    int getModeTimer();
};

#endif
