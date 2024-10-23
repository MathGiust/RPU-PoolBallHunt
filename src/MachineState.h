/*********************************************************************


    Machine State Header File
    Version : 1.20


*********************************************************************/

#ifndef MACHINE_STATE_H
#define MACHINE_STATE_H

#include "PlayerState.h"
#include <Arduino.h>

// Machine State definition :
#define MACHINE_STATE_DEBUG 0
#define MACHINE_STATE_ATTRACT 1
#define MACHINE_STATE_INIT_GAMEPLAY 2
#define MACHINE_STATE_INIT_NEW_BALL 3
#define MACHINE_STATE_NORMAL_GAMEPLAY 4
#define MACHINE_STATE_COUNTDOWN_BONUS 99
#define MACHINE_STATE_BALL_OVER 100
#define MACHINE_STATE_RESTART_GAME 101
#define MACHINE_STATE_MATCH_MODE 110

// Game Constants
#define BALLS_PER_GAME 3
#define BALL_SAVE_NUMBER_OF_SECONDS 5
#define COINS_PER_CREDIT 2
#define DEFAULT_HIGH_SCORE 10000
#define MAXIMUM_NUMBER_OF_BALLS_IN_PLAY 5
#define MAXIMUM_NUMBER_OF_COINS_PER_CREDIT 4
#define MAXIMUM_NUMBER_OF_CREDITS 99
#define MAXIMUM_NUMBER_OF_TILT_WARNINGS 2
#define MULTIBALL_SAVE_NUMBER_OF_SECONDS 15
#define SOLENOID_BUFFER_TIME 250
#define TILT_WARNING_DEBOUNCE_TIME 1000


struct MachineState {

private:
    PlayerState players[4];
    int         machineStateId;
    boolean     machineStateChanged;
    boolean     freeplay;
    boolean     currentScoreFlashing;

    byte    credits;
    byte    currentBallInPlay;
    byte    numberOfCoinsPerCredit;
    byte    numberOfPlayers;
    byte    currentPlayerNumber;
    byte    numberOfTiltWarnings;
    byte    numberOfBallsInPlay;
    boolean playfieldValidated;
    boolean samePlayerShootsAgain;

    score_t highScore;

    static byte mostRecentSwitchHit;

public:
    MachineState();

    PlayerState* currentPlayer{};

    boolean playerCanBeAdded() const;

    void increaseNumberOfPlayers();
    void increaseCurrentPlayerScore(score_t amountToAdd);
    void resetAllPlayers();
    void resetCurrentBallInPlay();
    void exitGame();
    void setMachineState(int machineState);
    void manageCoinDrop(byte switchHit);

    byte increaseCurrentPlayerNumber();
    byte manageCreditButton(byte State);
    byte restartGame();
    byte initGamePlay();
    byte initNewBall(boolean currentStateChanged);

    // Setters
    void setHighScore(score_t score);
    void setScore(score_t newScore, byte playerNumber);
    void setPlayfieldValidated(boolean isPlayFieldValidated);
    void setCurrentScoreFlashing(boolean value);
    void setNumberOfCreditsFromEEPROM();

    void setKickerSpinner(boolean value);


    // Getters
    score_t getHighScore() const;
    score_t getPlayerScore(byte player);
    byte    getCurrentPlayerNumber() const;
    byte    getNumberOfPlayers() const;
    byte    getCredits() const;
    int     getMachineState() const;
    boolean getSamePlayerShootsAgain() const;
    boolean getMachineStateChanged() const;
    boolean getPlayfieldValidated() const;
    boolean getCurrentScoreFlashing() const;


    static void setMostRecentSwitchHit(byte switchHit);
    static byte getMostRecentSwitchHit();


    // 8 Ball specific
    boolean isPlayerEven() const;
};

#endif
