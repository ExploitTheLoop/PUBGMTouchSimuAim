//
// Created by PCAditiaID29 on 6/25/2021.
//

#ifndef PUBGM_AIMBOT_TOUCH_TEST_MAIN_H
#define PUBGM_AIMBOT_TOUCH_TEST_MAIN_H

#include "../Includes/obfuscate.h"
#include "../Includes/Logger.h"

#define maxplayerCount 50
#define maxvehicleCount 20
#define maxitemsCount 100
#define maxgrenadeCount 10
#define maxlootBoxCount 25

struct AimbotTouch {
    bool Status;
};

enum Mode {
    InitMode = 1,
};

struct Request {
    int Mode;
    int ScreenWidth;
    int ScreenHeight;
    AimbotTouch Aimbot;
};

struct Response {
    bool Success;
};


#endif //PUBGM_AIMBOT_TOUCH_TEST_MAIN_H
