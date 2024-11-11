/*********************************************************************


Player State Header File
Version : 1.00


*********************************************************************/

#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

#include "Arduino.h"

#define MAX_BONUS_MULTIPLIER 5
#define MAX_DISPLAY_BONUS 49

#define NUMBER_OF_MODES 3

typedef int32_t score_t;

class PlayerState {
private:
    // General attributes
    score_t score;
    byte    bonusMultiplier;
    byte    bonus;

    // Gameplay
    boolean modePlayed[NUMBER_OF_MODES];
    boolean modeQualified[NUMBER_OF_MODES];

public:
    PlayerState();

    // Harlem specific
    boolean gateLit;
    boolean leftSpinnerBonusAdvanceLit;
    boolean globeLetters[5];
    byte    superBonus;
    byte    leftSpinnerValue;
    byte    centerSpinnerAdv;

    // Player initialize
    void resetPlayer();
    void initNewBall();

    // General methods
    void decreaseBonus();
    void decreaseBonus(byte amountToSubtract);
    void decreaseBonusMultiplier();
    void increaseBonus(byte amountToAdd = 1);
    void increaseBonusMultiplier(byte newBonusX);
    void increaseScore(score_t amountToAdd);
    void overridePlayerScore(score_t value);

    // Setters :
    void setScore(score_t newScore);
    void setBonusValue(byte newBonus);
    void setSuperBonus(byte superBonus);
    void setModePlayed(byte modeNumber, boolean value);
    void setModeQualified(byte modeNumber, boolean value);

    // Getters :
    score_t getScore() const;
    boolean getSuperBonus() const;
    byte    getBonusMultiplier() const;
    byte    getBonus() const;

    // Game Modes
    void    checkModeQualified();
    boolean getModePlayed(byte modeNumber) const;
    boolean getAnyModeQualified() const;
    boolean getModeQualified(byte modeNumber) const;

    //
    // Game specific
    //

    boolean checkSuperBonus() const;
    boolean getGateLit() const;
    void    switchLitBonusAdvance();

    // Getters
    byte getLeftSpinnerValue() const;
    bool getLeftSpinnerBonusAdvanceLit() const;

    // Setters
    void setGateLit(boolean value);
};

/*
class PlayerState {
  private:

    byte          qualifiedScoreMultiplier_;

    unsigned long lastFlash_;
    unsigned long tempScore_;

    boolean       knockerHasKnocked_;
    byte          modeStatus_[4];

    // 8 Ball Private



  public:


    boolean       allModesCompleted();
    boolean       anyModeQualified();
    boolean       scoreIncreasing();

    byte          bonus();
    byte          bonusMultiplier();
    byte          qualifiedScoreMultiplier();

    unsigned long score();


    void          increaseQualifiedScoreMultiplier();


    void          resetPlayerState();
    void          resetTopRollovers();
    void          rotatePlayerLamps();
    void          setBonus(byte value);
    void          setBonusMultiplier(byte value);
    void          setScore(unsigned long value);
    void          unqualifyAllModes();
    void          updateBonusLamps();
    void          updateBonusMultiplierLamps();
    void          updatePlayerScore(boolean flashCurrent = false, boolean dashCurrent = false);
    void          updateScoreMultiplierLamps();
    void          updateTopRolloverLamps();

    // 8 Ball Public
    void          registerRackBalls(byte switchHit);
    void          registerTopRollover(byte switchHit);
    void          registerBankShot();
    void          registerSpinner();
    void          register8Ball();

    boolean       topRolloversCompleted();
};
*/

#endif
