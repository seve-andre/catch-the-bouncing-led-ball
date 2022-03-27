#include "Arduino.h"

#define GAME_INTRO 1
#define PLAY_GAME 2
#define GAME_OVER 3

extern int gameStatus;

void restartGame();

void timeToPressButtonFinished();

void pressedButton();

int timeBeforePause(int timeBefore);

int timeBetweenLeds(int timerInterval);

void reduceTimesGame();

int timerPause(int timerPause);

void gameIntro();

void playGame();

void setUpGameIntro();

int findDifferentButtonPression();
