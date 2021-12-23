//
// Created by PCAditiaID29 on 5/30/2021.
//

#ifndef PUBGM_MOD_SHARKCHEAT_CANVASDRAWING_H
#define PUBGM_MOD_SHARKCHEAT_CANVASDRAWING_H

#include <thread>
void DrawingSkeleton(CanvasEngine esp, Vector2 vec1, Vector2 vec2, Color color);
bool isnull(Vector2 dat);

Color colorByDistance(int distance, float alpha){
    Color _colorByDistance;
    if (distance < 450)
        _colorByDistance = Color(0,255,0, alpha);
    if (distance < 200)
        _colorByDistance = Color(255,255,0, alpha);
    if (distance < 120)
        _colorByDistance = Color(255,133,51, alpha);
    if (distance < 50)
        _colorByDistance = Color(255,0,0, alpha);
    return _colorByDistance;
}

bool isContain(string str, string check) {
    size_t found = str.find(check);
    return (found != string::npos);
}

int randomer;

Color RGBBoy() {
    randomer += 1;
    if (randomer == 0) {
        return Color::Red();
    } else if (randomer == 1) {
        return Color::Red();
    } else if (randomer == 2) {
        return Color::Red();
    } else if (randomer == 3) {
        return Color::Red();
    } else if (randomer == 4) {
        return Color::Red();
    } else if (randomer == 5) {
        return Color::Red();
    } else if (randomer == 6) {
        return Color::Red();
    } else if (randomer == 7) {
        return Color::Red();
    } else if (randomer == 8) {
        return Color::Red();
    } else if (randomer == 9) {
        return Color::Yellow();
    } else if (randomer == 10) {
        return Color::Yellow();
    } else if (randomer == 11) {
        return Color::Yellow();
    } else if (randomer == 12) {
        return Color::Yellow();
    } else if (randomer == 13) {
        return Color::Yellow();
    } else if (randomer == 14) {
        return Color::Yellow();
    } else if (randomer == 15) {
        return Color::Yellow();
    } else if (randomer == 16) {
        return Color::Yellow();
    } else {
        randomer = 0;
        return Color::Yellow();
    }
}


int myTeamID = 101;
Color colorShootEnemy = Color::Red();
Color colorSeekbarAimbotRange = Color::Red();

void DrawEnemyCount(int PlayerCount, CanvasEngine rViewEngine){
    int colors;
    char eCountStr[10];
    if (PlayerCount != 0){
        colors = 2;
        if (PlayerCount > 5)
            colors = 2;
        if (PlayerCount > 10)
            colors = 3;
        if (PlayerCount > 15)
            colors = 4;
        sprintf(eCountStr, "%d", PlayerCount);
    } else {
        colors = 1;
        strcpy(eCountStr, "CLEAR");
    }
    rViewEngine.DrawEnemyCount(colors, Vector2(rViewEngine.getWidth()/2, 20));
    rViewEngine.DrawText2(Color::Black(), eCountStr, Vector2(rViewEngine.getWidth() / 2, 110), 25);
}

float GetDistance2(float Width, float Height, float x1, float y1)
{
    float xx1 = (Width / 2) - x1;
    float yy2 = (Height / 2) - y1;
    return sqrt(xx1 * xx1 + yy2 * yy2);
}

Request request;
Response response;

bool AimbotStatus = true;

extern "C"
void MainDraw(CanvasEngine rViewEngine) {
    clientSocket.send((void *) &request, sizeof(request));
    clientSocket.receive((void *) &response);
    request.Aimbot.Status = AimbotStatus;
}

bool isnull(Vector2 dat)
{
    if (dat.X != NULL)
        if (dat.Y != NULL)
            return true;
    return false;
}

#endif //PUBGM_MOD_SHARKCHEAT_CANVASDRAWING_H
