#include "../include/MathUtils.hpp"

Vector4D::Vector4D() {
    for (std::size_t i = 0; i < 4; i++) {
        element[i] = 0;
    }
}

Vector4D::Vector4D(std::vector<float> elements_) {
    for (std::size_t i = 0; i < 4; i++) {
        element[i] = elements_[i];
    }
}

float Vector4D::at(std::size_t pos) const {return element[pos];}

const float* Vector4D::begin() const {return element;}

const float* Vector4D::end() const {return element + 4;}

Vector4D Vector4D::operator+(const Vector4D& other) const {
    Vector4D result;
    for (std::size_t i = 0; i < 4; i++) {
        result.element[i] = element[i] + other.element[i];
    }
    return result;
}

Vector4D Vector4D::operator-(const Vector4D& other) const {
    Vector4D result;
    for (std::size_t i = 0; i < 4; i++) {
        result.element[i] = element[i] - other.element[i];
    }
    return result;
}

Vector4D Vector4D::operator*(const float& other) const {
    return Vector4D({element[0] * other, element[1] * other, element[2] * other, element[3] * other});
}

Vector4D Vector4D::operator/(const float& other) const {
    return Vector4D({element[0] / other, element[1] / other, element[2] / other, element[3] / other});
}

Vector4D crossprod(const Vector4D& A, const Vector4D& B) {
    return Vector4D({A.at(1) * B.at(2) - A.at(2) * B.at(1), A.at(2) * B.at(0) - A.at(0) * B.at(2), A.at(0) * B.at(1) - A.at(1) * B.at(0), 0});
}

float dotprod(const Vector4D& A, const Vector4D& B) {
    return A.at(0) * B.at(0) + A.at(1) * B.at(1) + A.at(2) * B.at(2);
}

float distance2(const Vector4D& A, const Vector4D& B) {
    return dotprod(A - B, A - B);
}

Matrix4D::Matrix4D() {
    for (std::size_t i = 0; i < 16; i++) {
        element[i] = 0;
    }
}

Matrix4D:: Matrix4D(std::vector<float> elements_) {
    for (std::size_t i = 0; i < 16; i++) {
        element[i] = elements_[i];
    }
}

float Matrix4D::at(std::size_t row, std::size_t col) const {return element[row + col * 4];}

const float* Matrix4D::begin() const {return element;}

const float* Matrix4D::end() const {return element + 16;}

Matrix4D Matrix4D::operator*(const Matrix4D &other) const {
    Matrix4D result;
    for (std::size_t row = 0; row < 4; ++row) {
        for (std::size_t col = 0; col < 4; ++col) {
            for (std::size_t k = 0; k < 4; ++k) {
                result.element[row + col * 4] += element[row + k * 4] * other.element[k + col * 4];
            }
        }
    }
    return result;
}

Vector4D Matrix4D::operator*(const Vector4D &other) const {
    std::vector<float> result({0, 0, 0, 0});
    for (std::size_t row = 0; row < 4; ++row) {
        for (std::size_t col = 0; col < 4; ++col) {
            result[row] += element[row + col * 4] * other.at(col);
        }
    }
    return Vector4D(result);
}

Matrix4D Matrix4D::operator+(const Matrix4D &other) const {
    Matrix4D result;
    for (std::size_t i = 0; i < 16; i++) {
        result.element[i] = element[i] + other.element[i];
    }
    return result;
}

Matrix4D Matrix4D::operator-(const Matrix4D &other) const {
    Matrix4D result;
    for (std::size_t i = 0; i < 16; i++) {
        result.element[i] = element[i] - other.element[i];
    }
    return result;
}
Matrix4D rotationMatrix4DX(const float theta) {
    return Matrix4D({
        1,  0,                  0,                  0,
        0,  std::cos(theta),    std::sin(theta),    0,
        0,  -std::sin(theta),   std::cos(theta),    0,
        0,  0,                  0,                  1
    });
}

Matrix4D rotationMatrix4DY(const float theta) {
    return Matrix4D({
        std::cos(theta),    0,  -std::sin(theta),   0,
        0,                  1,  0,                  0,
        std::sin(theta),    0,  std::cos(theta),    0,
        0,                  0,  0,                  1
    });
}

Matrix4D rotationMatrix4DZ(const float theta) {
    return Matrix4D({
        std::cos(theta),    std::sin(theta),    0,  0,
        -std::sin(theta),   std::cos(theta),    0,  0,
        0,                  0,                  1,  0,
        0,                  0,                  0,  1
    });
}

Matrix4D translationMatrix4D(const float x, const float y, const float z) {
    return Matrix4D({
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    });
}