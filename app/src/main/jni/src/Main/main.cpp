//
// Created by PCAditiaID29 on 6/27/2021.
//

#include "main.h"
#include <jni.h>
#include "Structs.h"
#include "CanvasEngine.h"
#include "CanvasDrawing.h"

CanvasEngine canvasEngine;

extern "C"
JNIEXPORT jint JNICALL
Java_com_sharkcheat_pubgmaimbottouchtest_MyService_getInitBase(JNIEnv *env, jobject clazz,
                                                               jint Width, jint Height) {
    if (!clientSocket.Create()){
        Log.ERROR("[Client] Socket can't create");
        return -1;
    }
    if (!clientSocket.Bind()){
        Log.ERROR("[Client] Socket can't bind");
        return -1;
    }
    if (!clientSocket.Listen()){
        Log.ERROR("[Client] Socket can't listen");
        return -1;
    }
    if (clientSocket.Accept()){
        Log.INFO("[Client] Socket accepted");
        Request request{};
        request.Mode = InitMode;
        request.ScreenWidth = Width;
        request.ScreenHeight = Height;
        request.Aimbot.Status = AimbotStatus;
        clientSocket.send((void*) &request, sizeof(request));
    }
    return 1;
}extern "C"
JNIEXPORT void JNICALL
Java_com_sharkcheat_pubgmaimbottouchtest_MyService_closeSocket(JNIEnv *env, jobject thiz) {
    clientSocket.Close();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_sharkcheat_pubgmaimbottouchtest_MyService_getInitCanvasDrawing(JNIEnv *env, jclass clazz,
                                                                        jobject canvas_drawing_view,
                                                                        jobject canvas) {
    canvasEngine = CanvasEngine(env, canvas_drawing_view, canvas);
    if (canvasEngine.isValid()){
        Width = canvasEngine.getWidth();
        Height = canvasEngine.getHeight();
        MainDraw(canvasEngine);
    }
}