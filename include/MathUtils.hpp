#include <vector>
#include <math.h>
#include <iostream>
#pragma once

class Vector4D {
private:
    float element[4];

public:
    Vector4D();
    Vector4D(std::vector<float>);

    float at(std::size_t) const;
    const float* begin() const;
    const float* end() const;

    Vector4D operator+(const Vector4D&) const;
    Vector4D operator-(const Vector4D&) const;
    Vector4D operator*(const float&) const;
    Vector4D operator/(const float&) const;
};

Vector4D crossprod(const Vector4D&, const Vector4D&);
float dotprod(const Vector4D&, const Vector4D&);
float distance2(const Vector4D&, const Vector4D&);


class Matrix4D {
private:
    float element[16];

public:
    Matrix4D(std::vector<float>);
    Matrix4D();

    float at(std::size_t, std::size_t) const;
    const float* begin() const;
    const float* end() const;

    Matrix4D operator*(const Matrix4D&) const;
    Vector4D operator*(const Vector4D&) const;
    Matrix4D operator+(const Matrix4D&) const;
    Matrix4D operator-(const Matrix4D&) const;
};

Matrix4D rotationMatrix4DX(const float);
Matrix4D rotationMatrix4DY(const float);
Matrix4D rotationMatrix4DZ(const float);
Matrix4D translationMatrix4D(const float, const float, const float);