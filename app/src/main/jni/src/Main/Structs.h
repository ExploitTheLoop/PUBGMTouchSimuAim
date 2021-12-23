//
// Created by PCAditiaID29 on 2/10/2021.
//

#ifndef RCHEATS_STRUCTS_H
#define RCHEATS_STRUCTS_H

using namespace std;

float Width, Height;

class Color {
public:
    float r;
    float g;
    float b;
    float a;

    Color() {
        this->r = 0;
        this->g = 0;
        this->b = 0;
        this->a = 0;
    }

    Color(float r, float g, float b, float a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    Color(float r, float g, float b) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = 255;
    }

    static Color Red(){
        return Color(255,0,0);
    }
    static Color Red2(){
        return Color(255,26,26);
    }
    static Color Red3(){
        return Color(255,77,77);
    }

    static Color Orange(){
        return Color(255,102,0);
    }

    static Color Orange2(){
        return Color(255,133,51);
    }

    static Color Orange3(){
        return Color(255,133,51);
    }

    static Color Yellow(){
        return Color(255,255,0);
    }

    static Color Yellow2(){
        return Color(255,204,0);
    }

    static Color Yellow3(){
        return Color(255,255,51);
    }

    static Color White(){
        return Color(255,255,255);
    }

    static Color Green(){
        return Color(0,255,0);
    }
    static Color Green2(){
        return Color(26,255,26);
    }
    static Color Green3(){
        return Color(21,255,21);
    }
    static Color Blue(){
        return Color(0,0,255);
    }

    static Color BlueSky1(){
        return Color(0,102,255);
    }

    static Color BlueSky2(){
        return Color(51,133,255);
    }
    static Color Pink(){
        return Color(255,0,102);
    }
    static Color Pink2(){
        return Color(255,51,153);
    }
    static Color BlueDongker(){
        return Color(0,0,102);
    }
    static Color Grey(){
        return Color(102,102,153);
    }
    static Color Silver(){
        return Color(230,230,230);
    }
    static Color Silver2(){
        return Color(204,204,204);
    }
    static Color SilverDark(){
        return Color(64,64,64);
    }
    static Color Black(){
        return Color(0,0,0);
    }
    static Color Dark2(){
        return Color(0,3,12);
    }
};

class Vector2
{
public:
    float X;
    float Y;

    Vector2() {
        this->X = 0.0f;
        this->Y = 0.0f;
    }

    Vector2(float X, float Y) {
        this->X = X;
        this->Y = Y;
    }

};

class Vector3
{
public:
    float X;
    float Y;
    float Z;

    Vector3() {
        this->X = 0.0f;
        this->Y = 0.0f;
        this->Z = 0.0f;
    }

    Vector3(float X, float Y, float Z) {
        this->X = X;
        this->Y = Y;
        this->Z = Z;
    }

};

struct Vector4
{
    float X;
    float Y;
    float Z;
    float W;
};

struct FTransform
{
    Vector4 Rotation;
    Vector3 Translation;
    float ddd;
    Vector3 Scale3D;
};


struct D3DMatrix {
    float _11, _12, _13, _14;
    float _21, _22, _23, _24;
    float _31, _32, _33, _34;
    float _41, _42, _43, _44;
};

D3DMatrix ToMatrixWithScale(Vector3 translation, Vector3 scale, Vector4 rot) {
    D3DMatrix m;
    m._41 = translation.X;
    m._42 = translation.Y;
    m._43 = translation.Z;

    float x2 = rot.X + rot.X;
    float y2 = rot.Y + rot.Y;
    float z2 = rot.Z + rot.Z;

    float xx2 = rot.X * x2;
    float yy2 = rot.Y * y2;
    float zz2 = rot.Z * z2;
    m._11 = (1.0f - (yy2 + zz2)) * scale.X;
    m._22 = (1.0f - (xx2 + zz2)) * scale.Y;
    m._33 = (1.0f - (xx2 + yy2)) * scale.Z;

    float yz2 = rot.Y * z2;
    float wx2 = rot.W * x2;
    m._32 = (yz2 - wx2) * scale.Z;
    m._23 = (yz2 + wx2) * scale.Y;

    float xy2 = rot.X * y2;
    float wz2 = rot.W * z2;
    m._21 = (xy2 - wz2) * scale.Y;
    m._12 = (xy2 + wz2) * scale.X;

    float xz2 = rot.X * z2;
    float wy2 = rot.W * y2;
    m._31 = (xz2 + wy2) * scale.Z;
    m._13 = (xz2 - wy2) * scale.X;

    m._14 = 0.0f;
    m._24 = 0.0f;
    m._34 = 0.0f;
    m._44 = 1.0f;

    return m;
}

D3DMatrix MatrixMultiplication(D3DMatrix pM1, D3DMatrix pM2) {
    D3DMatrix pOut = {
            pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41,
            pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42,
            pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43,
            pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44,
            pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41,
            pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42,
            pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43,
            pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44,
            pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41,
            pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42,
            pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43,
            pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44,
            pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41,
            pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42,
            pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43,
            pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44
    };

    return pOut;
}
#endif //RCHEATS_STRUCTS_H
