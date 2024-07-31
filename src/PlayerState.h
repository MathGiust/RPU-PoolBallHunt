/*********************************************************************


Player State Header File
Version : 1.00


*********************************************************************/

#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

#include "Arduino.h"

#define MAX_BONUS_MULTIPLIER 5
#define MAX_DISPLAY_BONUS 16

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

    // 8 Ball specific
    boolean kickerSpinnerLit;
    boolean superBonus;
    boolean eightBallQualified;
    boolean topRollOversCompleted;
    boolean plainBallsLit[7];
    boolean stripBallsLit[7];
    byte    bankShotProgress;

public:
    PlayerState();


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
    void setSuperBonus(boolean superBonus);

    // Getters :
    score_t getScore() const;
    boolean getSuperBonus() const;
    byte    getBonusMultiplier() const;
    byte    getBonus() const;

    //
    // Game specific
    //
    boolean checkSuperBonus() const;

    // Getters :
    boolean getEightBallQualified() const;
    boolean getTopRollOversCompleted() const;
    boolean getKickerSpinnerLit() const;
    boolean getPlainBallsLit(byte index);
    boolean getStripBallsLit(byte index);
    boolean getModePlayed(byte modeNumber) const;
    boolean getAnyModeQualified() const;
    boolean getModeQualified(byte modeNumber) const;
    byte    getBankShotProgress() const;

    // Setters :
    void setPlainBallsLit(byte index, boolean value);
    void setStripBallsLit(byte index, boolean value);
    void setEightBallQualified(boolean value);
    void setTopRollOversCompleted(boolean value);
    void setBankShotProgress(byte value);
    void setKickerSpinnerLit(boolean value);
    void setModePlayed(byte modeNumber, boolean value);
    void setModeQualified(byte modeNumber, boolean value);

    // Game Modes
    void checkModeQualified();

    // Pool balls
    void resetPoolBallsLit();
    void randomisePoolBallsLit();

    // Bank shot
    void bankShotAdvance();
    void bankShotReset();
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
