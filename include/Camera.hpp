#include "MathUtils.hpp"
#pragma once

class Camera {
private:
    Vector4D vecLookDir = Vector4D({0, 0, 1, 0});
    Vector4D vecEye = Vector4D({0, 0, 0, 0});
    Vector4D vecUp = Vector4D({0, -1, 0, 0});
    Matrix4D matView;

    Matrix4D LookAtMatrix4D() const {
        Vector4D new_foward = vecLookDir;
        new_foward = new_foward / std::sqrt(dotprod(new_foward, new_foward));

        Vector4D new_right = crossprod(new_foward, vecUp);
        new_right = new_right / std::sqrt(dotprod(new_right, new_right));

        Vector4D new_up = crossprod(new_foward, new_right);
        
        return Matrix4D({
            new_right.at(0),                new_up.at(0),               new_foward.at(0),                0,
            new_right.at(1),                new_up.at(1),               new_foward.at(1),                0,
            new_right.at(2),                new_up.at(2),               new_foward.at(2),                0,
            -dotprod(vecEye, new_right),    -dotprod(vecEye, new_up),   -dotprod(vecEye, new_foward),    1
        });
    }

    float fPitch = 0;
    float fYaw = 0;

public:
    const float fScreenWidth;
    const float fScreenHeight;
    const float fFOV;
    const float fZfar;
    const float fZnear;
    const float fAspectRatio;
    const float fF;
    const float fQ;
    const Matrix4D matProjectionMatrix;

    Camera(const float ScreenWidth, const float ScreenHeight, const float FOV, const float Zfar, const float Znear) :
    fScreenWidth(ScreenWidth), fScreenHeight(ScreenHeight), fFOV(FOV), fZfar(Zfar), fZnear(Znear), fAspectRatio(ScreenHeight / ScreenWidth), fF(1 / std::tan(FOV / 2)), fQ(Zfar / (Zfar - Znear)),
    matProjectionMatrix({
        fAspectRatio * fF,  0,  0,              0,
        0,                  fF, 0,              0,
        0,                  0,  fQ,             1,
        0,                  0,  -fZnear * fQ,   0
    }) {
        matView = LookAtMatrix4D();
    }

    void MoveX(const float dx) {
        vecEye = vecEye + (crossprod(vecLookDir, vecUp) * dx);
        matView = LookAtMatrix4D();
        return;
    }

    void MoveY(const float dy) {
        vecEye = vecEye + (vecUp * dy);
        matView = LookAtMatrix4D();
        return;
    }

    void MoveZ(const float dz) {
        vecEye = vecEye + (vecLookDir * dz);
        matView = LookAtMatrix4D();
        return;
    }

    void RotateX(const float pitch) {
        if (std::abs(fPitch + pitch) > 1.4) {
            return;
        }
        fPitch += pitch;
        vecLookDir = rotationMatrix4DY(fYaw) * rotationMatrix4DX(fPitch) * Vector4D({0, 0, 1, 0});
        vecUp = rotationMatrix4DY(fYaw) * rotationMatrix4DX(fPitch) * Vector4D({0, -1, 0, 0});
        matView = LookAtMatrix4D();
        return;
    }

    void RotateY(const float yaw) {
        fYaw += yaw;
        vecLookDir = rotationMatrix4DY(fYaw) * rotationMatrix4DX(fPitch) * Vector4D({0, 0, 1, 0});
        vecUp = rotationMatrix4DY(fYaw) * rotationMatrix4DX(fPitch) * Vector4D({0, -1, 0, 0});
        matView = LookAtMatrix4D();
        return;
    }

    Vector4D Eye() const {
        return vecEye;
    }

    Vector4D LookDir() const {
        return vecLookDir;
    }

    Vector4D Target() const {
        return vecLookDir + vecEye;
    }

    Matrix4D View() const {
        return matView;
    }
};