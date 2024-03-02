#ifndef __WINDOW_HEADER__
#define __WINDOW_HEADER__

// #include <iostream>
#include <iomanip>
#include <sstream>
#include "C:\raylib\raylib\src\raylib.h"
#include <string>
#include <cmath>
#include "csv.hpp"

const float SCREENWIDTH = 400;
const float SCREENHEIGHT = 580;
const int FPS = 60;
const float PAY = 10.5;
const std::string TITLE = "WorkMathíe";
const Color WhiteChocolate = {238, 231, 211};
const Font font = GetFontDefault();

const Color BACKGROUND = WhiteChocolate;
const Color SYSTEMTEXT = VIOLET;
const Color USERTEXT = BROWN;
const Color BOXLINEDEFAULT = BEIGE;
const Color BOXLINEMOUSE = PINK;
const Color BOXCOL = LIGHTGRAY;

std::string hoursStr(std::string inputStartHH, std::string inputStartMM, std::string inputEndHH, std::string inputEndMM);
std::string moneyStr(float duration);
int strToInt(std::string str);
float convertToDecimal(int hours, int minutes);
float calculateDuration(int startHH, int startMM, int endHH, int endMM);
void DrawTextCentered(const char *text, Vector2 position, int fontSize, Color color);
void Init();

#endif