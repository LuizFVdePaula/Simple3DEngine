#include "MathUtils.hpp"
#include "Camera.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <list>
#pragma once

struct Triangle4D {
    Vector4D vecPoint[3];
};

Triangle4D transform_triangle(const Matrix4D&, const Triangle4D&);
bool is_inside_fov(const Triangle4D&);

struct RasterTriangle {
    //sf::ConvexShape* pTri = nullptr;
    sf::Vertex* pTri = nullptr;
    float fDist = 0;
};

bool operator<(const RasterTriangle&, const RasterTriangle&);

class AbstractObject : public sf::Drawable, public sf::Transformable {
protected:
    std::vector<Triangle4D> vMesh;
    //std::vector<sf::ConvexShape> vTriangles;
    sf::VertexArray vTriangles;
    std::list<RasterTriangle> lTrianglesToRasterize;
    Camera* pCamera;

public:
    bool load_object(const std::string);
    void transform_mesh(const Matrix4D&);
    void update_triangles();
    //void update_triangles(const Matrix4D&);
    void draw(sf::RenderTarget&, sf::RenderStates) const;

};

class CustomObject : public AbstractObject {
public:
    CustomObject(const std::string, Camera*);
};