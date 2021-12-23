//
// Created by PCAditiaID29 on 2/10/2021.
//

#ifndef RCHEATS_CANVASENGINE_H
#define RCHEATS_CANVASENGINE_H

class CanvasEngine {
private:
    JNIEnv *_env;
    jobject _view;
    jobject _canvas;

public:
    CanvasEngine() {
        _env = nullptr;
        _view = nullptr;
        _canvas = nullptr;
    }

    CanvasEngine(JNIEnv *env, jobject view, jobject canvas) {
        this->_env = env;
        this->_view = view;
        this->_canvas = canvas;
    }

    bool isValid() const {
        return (_env != nullptr && _view != nullptr && _canvas != nullptr);
    }

    int getWidth() const {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_view);
            jmethodID width = _env->GetMethodID(canvasView, "getWidth", "()I");
            return _env->CallIntMethod(_view, width);
        }
        return 0;
    }

    int getHeight() const {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_view);
            jmethodID width = _env->GetMethodID(canvasView, "getHeight", "()I");
            return _env->CallIntMethod(_view, width);
        }
        return 0;
    }
    void DrawText(Color color, const char *nick, Vector2 pos, float size) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_view);
            jmethodID drawtext = _env->GetMethodID(canvasView, "DrawText",
                                                   "(Landroid/graphics/Canvas;IIIILjava/lang/String;FFF)V");
            _env->CallVoidMethod(_view, drawtext, _canvas, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b,
                                 _env->NewStringUTF(nick), pos.X, pos.Y, size);
        }
    }
    void DrawText2(Color color, const char *nick, Vector2 pos, float size) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_view);
            jmethodID drawText2 = _env->GetMethodID(canvasView, "DrawText2",
                                                    "(Landroid/graphics/Canvas;IIIILjava/lang/String;FFF)V");
            _env->CallVoidMethod(_view, drawText2, _canvas, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b,
                                 _env->NewStringUTF(nick), pos.X, pos.Y, size);
        }
    }
    void DrawLine(Color color, float thickness, Vector2 start, Vector2 end) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_view);
            jmethodID drawline = _env->GetMethodID(canvasView, "DrawLine",
                                                   "(Landroid/graphics/Canvas;IIIIFFFFF)V");
            _env->CallVoidMethod(_view, drawline, _canvas, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b,
                                 thickness,
                                 start.X, start.Y, end.X, end.Y);
        }
    }
    void DrawEnemyCount(int colors, Vector2 d) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_view);
            jmethodID drawtext = _env->GetMethodID(canvasView, "DrawEnemyCount",
                                                   "(Landroid/graphics/Canvas;IFF)V");
            _env->CallVoidMethod(_view, drawtext, _canvas, colors, d.X, d.Y);
        }
    }
    void DrawCircle(Color color, Vector2 c, float radius) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_view);
            jmethodID drawCircle = _env->GetMethodID(canvasView, "DrawCircle",
                                                     "(Landroid/graphics/Canvas;IIIIFFF)V");
            _env->CallVoidMethod(_view, drawCircle, _canvas, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b, c.X, c.Y, radius);
        }
    }
    void DrawCircleStroke(Color color, Vector2 c, float radius) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_view);
            jmethodID drawCircle = _env->GetMethodID(canvasView, "DrawCircleStroke",
                                                     "(Landroid/graphics/Canvas;IIIIFFF)V");
            _env->CallVoidMethod(_view, drawCircle, _canvas, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b, c.X, c.Y, radius);
        }
    }
    void DrawCircle2(Color color, Vector2 c, float radius) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_view);
            jmethodID drawCircle = _env->GetMethodID(canvasView, "DrawCircle2",
                                                     "(Landroid/graphics/Canvas;IIIIFFF)V");
            _env->CallVoidMethod(_view, drawCircle, _canvas, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b, c.X, c.Y, radius);
        }
    }
    void DrawRect(Color color, float strokeSize, Vector2 start, Vector2 end) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_view);
            jmethodID drawRect = _env->GetMethodID(canvasView, "DrawRect",
                                                   "(Landroid/graphics/Canvas;IIIIFFFFF)V");
            _env->CallVoidMethod(_view, drawRect, _canvas, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b, strokeSize, start.X, start.Y, end.X, end.Y);
        }
    }
    void DrawEnemyWeapon(Color color, int wid, int ammo, Vector2 pos, float size) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_view);
            jmethodID drawtext = _env->GetMethodID(canvasView, "DrawEnemyWeapon",
                                                   "(Landroid/graphics/Canvas;IIIIIIFFF)V");
            _env->CallVoidMethod(_view, drawtext, _canvas, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b,
                                 wid,ammo, pos.X, pos.Y, size);
        }
    }
    void DrawVehicles(const char *txt, float distance, Vector2 pos, float size) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_view);
            jmethodID drawtext = _env->GetMethodID(canvasView, "DrawVehicles",
                                                   "(Landroid/graphics/Canvas;Ljava/lang/String;IFFF)V");
            jstring s=_env->NewStringUTF(txt);
            _env->CallVoidMethod(_view, drawtext, _canvas,
                                 s , (int) distance, pos.X, pos.Y, size);
            _env->DeleteLocalRef(s);
        }
    }
    void DrawItem(const char *txt, float distance, Vector2 pos, float size) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_view);
            jmethodID drawtext = _env->GetMethodID(canvasView, "DrawItem",
                                                   "(Landroid/graphics/Canvas;Ljava/lang/String;IFFF)V");
            jstring s=_env->NewStringUTF(txt);
            _env->CallVoidMethod(_view, drawtext, _canvas,
                                 s , (int) distance, pos.X, pos.Y, size);
            _env->DeleteLocalRef(s);
        }
    }
    void DrawWeapon(const char *txt, float distance, Vector2 pos, float size) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_view);
            jmethodID drawWeapon = _env->GetMethodID(canvasView, "DrawWeapon",
                                                     "(Landroid/graphics/Canvas;Ljava/lang/String;IFFF)V");
            jstring s=_env->NewStringUTF(txt);
            _env->CallVoidMethod(_view, drawWeapon, _canvas,
                                 s , (int) distance, pos.X, pos.Y, size);
            _env->DeleteLocalRef(s);
        }
    }

    void DrawFilled(Color color, float strokeSize, Vector2 posX, Vector2 postY) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_view);
            jmethodID drawWeapon = _env->GetMethodID(canvasView, "DrawFilled",
                                                     "(Landroid/graphics/Canvas;IIIIFFFFF)V");
            _env->CallVoidMethod(_view, drawWeapon, _canvas, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b, strokeSize, posX.X, posX.Y, postY.X, postY.Y);
        }
    }


    void DrawHealth(Color color, Vector2 barHealthX, Vector2 barHealthY, Vector2 barStrokeX, Vector2 barStrokeY) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_view);
            jmethodID drawWeapon = _env->GetMethodID(canvasView, "DrawHealth",
                                                     "(Landroid/graphics/Canvas;IIIIFFFFFFFF)V");
            _env->CallVoidMethod(_view, drawWeapon, _canvas, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b, barHealthX.X, barHealthX.Y, barHealthY.X, barHealthY.Y, barStrokeX.X, barStrokeX.Y, barStrokeY.X, barStrokeY.Y);
        }
    }

};
#endif //RCHEATS_CANVASENGINE_H
