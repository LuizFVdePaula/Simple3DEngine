#include "../include/AbstractObject.hpp"

Triangle4D transform_triangle(const Matrix4D& T, const Triangle4D& triangle) {
    return Triangle4D{T * triangle.vecPoint[0], T * triangle.vecPoint[1], T * triangle.vecPoint[2]};
}

bool is_inside_fov(const Triangle4D& triangle) {
    const float tx = 1.25; // hardcoded!!! -> std::tan(fFOV / 2) / fAspectRatio
    const float ty = 1; // hardcoded!!! -> std::tan(fFOV / 2)
    if (!(triangle.vecPoint[0].at(2) > 1 && triangle.vecPoint[1].at(2) > 1 && triangle.vecPoint[2].at(2) > 1)) {
        return false;
    }
    if (tx * triangle.vecPoint[0].at(2) < std::abs(triangle.vecPoint[0].at(0)) && tx * triangle.vecPoint[1].at(2) < std::abs(triangle.vecPoint[1].at(0)) && tx * triangle.vecPoint[2].at(2) < std::abs(triangle.vecPoint[2].at(0))) {
        return false;
    }
    if (ty * triangle.vecPoint[0].at(2) < std::abs(triangle.vecPoint[0].at(1)) && ty * triangle.vecPoint[1].at(2) < std::abs(triangle.vecPoint[1].at(1)) && ty * triangle.vecPoint[2].at(2) < std::abs(triangle.vecPoint[2].at(1))) {
        return false;
    }
    return true;
}

bool operator<(const RasterTriangle& A, const RasterTriangle& B) {
    return A.fDist > B.fDist;
}

bool AbstractObject::load_object(const std::string filename) {
    std::ifstream f(filename);
    if (!f.is_open()) {
        return false;
    }
    std::vector<Vector4D> verts;
    std::string line;
    while (std::getline(f, line)) {
        std::istringstream s(line);
        char junk;

        if (line[0] == 'v') {
            float x, y, z;
            s >> junk >> x >> y >> z;
            verts.push_back(Vector4D({x, y, z, 1}));
        }
        if (line[0] == 'f') {
            int f[3];
            s >> junk >> f[0] >> f[1] >> f[2];
            vMesh.push_back({verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1]});
        }
    }
    f.close();
    return true;
}

void AbstractObject::transform_mesh(const Matrix4D& T) {
    for (Triangle4D& triangle : vMesh) {
        triangle = transform_triangle(T, triangle);
    }
}

void AbstractObject::update_triangles() {
    lTrianglesToRasterize.clear();

    const Vector4D vecLightSource = pCamera->View() *  Vector4D({0.707, -0.707, 0, 0});
    
    //for (std::size_t i = 0; i < vTriangles.size(); i++) {
    for (std::size_t i = 0; i < vMesh.size(); i++) {
        Triangle4D triTransformedTriangle = transform_triangle(pCamera->View(), vMesh[i]);

        Triangle4D triProjTriangle = transform_triangle(pCamera->matProjectionMatrix, triTransformedTriangle);

        Vector4D l1 = triTransformedTriangle.vecPoint[1] - triTransformedTriangle.vecPoint[0];
        Vector4D l2 = triTransformedTriangle.vecPoint[2] - triTransformedTriangle.vecPoint[0];
        Vector4D vecNormal = crossprod(l1, l2);
        vecNormal = vecNormal / std::sqrt(dotprod(vecNormal, vecNormal));
        Vector4D vecPointing = triTransformedTriangle.vecPoint[0];
        vecPointing = vecPointing / std::sqrt(dotprod(vecPointing, vecPointing));

        if ((dotprod(vecNormal, vecPointing) < 0.0) && is_inside_fov(triTransformedTriangle)) {
            for (std::size_t j = 0; j < 3; j++) {
                if (triProjTriangle.vecPoint[j].at(3) != 0) {
                    triProjTriangle.vecPoint[j] = Vector4D({triProjTriangle.vecPoint[j].at(0) / triProjTriangle.vecPoint[j].at(3), triProjTriangle.vecPoint[j].at(1) / triProjTriangle.vecPoint[j].at(3), triProjTriangle.vecPoint[j].at(2) / triProjTriangle.vecPoint[j].at(3), 1});
                }
                //vTriangles[i].setPoint(j, sf::Vector2f((triProjTriangle.vecPoint[j].at(0) + 1) * pCamera->fScreenWidth / 2, (triProjTriangle.vecPoint[j].at(1) + 1) * pCamera->fScreenHeight / 2));
                vTriangles[3 * i + j].position = sf::Vector2f((triProjTriangle.vecPoint[j].at(0) + 1) * pCamera->fScreenWidth / 2, (triProjTriangle.vecPoint[j].at(1) + 1) * pCamera->fScreenHeight / 2);
            }
            float fAlpha = (dotprod(vecNormal, vecLightSource) + 1) / 2;
            //vTriangles[i].setFillColor(sf::Color(255 * fAlpha, 255 * fAlpha, 255 * fAlpha, 255));
            vTriangles[3 * i].color = sf::Color(255 * fAlpha, 255 * fAlpha, 255 * fAlpha, 255);
            vTriangles[3 * i + 1].color = sf::Color(255 * fAlpha, 255 * fAlpha, 255 * fAlpha, 255);
            vTriangles[3 * i + 2].color = sf::Color(255 * fAlpha, 255 * fAlpha, 255 * fAlpha, 255);

            float fCMZ = triTransformedTriangle.vecPoint[0].at(2) + triTransformedTriangle.vecPoint[1].at(2) + triTransformedTriangle.vecPoint[2].at(2);
            //lTrianglesToRasterize.emplace_back(RasterTriangle{&vTriangles[i], fCMZ});
            lTrianglesToRasterize.emplace_back(RasterTriangle{&vTriangles[3 * i], fCMZ});
        }
    }
    lTrianglesToRasterize.sort();
}
/*
void AbstractObject::update_triangles(const Matrix4D& T) {
    lTrianglesToRasterize.clear();

    const Vector4D vecLightSource = pCamera->View() * Vector4D({0.707, -0.707, 0, 0});
    
    for (std::size_t i = 0; i < vTriangles.size(); i++) {
        Triangle4D triTransformedTriangle = transform_triangle(pCamera->View() * T, vMesh[i]);

        Triangle4D triProjTriangle = transform_triangle(pCamera->matProjectionMatrix, triTransformedTriangle);

        Vector4D l1 = triTransformedTriangle.vecPoint[1] - triTransformedTriangle.vecPoint[0];
        Vector4D l2 = triTransformedTriangle.vecPoint[2] - triTransformedTriangle.vecPoint[0];
        Vector4D vecNormal = crossprod(l1, l2);
        vecNormal = vecNormal / std::sqrt(dotprod(vecNormal, vecNormal));
        Vector4D vecPointing = triTransformedTriangle.vecPoint[0];
        vecPointing = vecPointing / std::sqrt(dotprod(vecPointing, vecPointing));

        if ((dotprod(vecNormal, vecPointing) < 0.0) && is_inside_fov(triTransformedTriangle)) {
            for (std::size_t j = 0; j < 3; j++) {
                if (triProjTriangle.vecPoint[j].at(3) != 0) {
                    triProjTriangle.vecPoint[j] = Vector4D({triProjTriangle.vecPoint[j].at(0) / triProjTriangle.vecPoint[j].at(3), triProjTriangle.vecPoint[j].at(1) / triProjTriangle.vecPoint[j].at(3), triProjTriangle.vecPoint[j].at(2) / triProjTriangle.vecPoint[j].at(3), 1});
                }
                vTriangles[i].setPoint(j, sf::Vector2f((triProjTriangle.vecPoint[j].at(0) + 1) * pCamera->fScreenWidth / 2, (triProjTriangle.vecPoint[j].at(1) + 1) * pCamera->fScreenHeight / 2));
            }
            float fAlpha = (dotprod(vecNormal, vecLightSource) + 1) / 2;
            vTriangles[i].setFillColor(sf::Color(255 * fAlpha, 255 * fAlpha, 255 * fAlpha, 255));
            float fCMZ = triTransformedTriangle.vecPoint[0].at(2) + triTransformedTriangle.vecPoint[1].at(2) + triTransformedTriangle.vecPoint[2].at(2);
            lTrianglesToRasterize.emplace_back(RasterTriangle{&vTriangles[i], fCMZ});
        }
    }
    lTrianglesToRasterize.sort();
}
*/
void AbstractObject::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    //for (const RasterTriangle& Tri : lTrianglesToRasterize) {
    //    target.draw(*Tri.pTri);
    //}
    for (const RasterTriangle& Tri : lTrianglesToRasterize) {
        target.draw(Tri.pTri, 3, sf::Triangles);
    }
}

CustomObject::CustomObject(const std::string filename, Camera* camera) {
    pCamera = camera;
    load_object(filename);

    std::sort(vMesh.begin(), vMesh.end(), [](const Triangle4D& tri1, const Triangle4D& tri2) {
        return tri1.vecPoint[0].at(2) + tri1.vecPoint[1].at(2) + tri1.vecPoint[2].at(2) > tri2.vecPoint[0].at(2) + tri2.vecPoint[1].at(2) + tri2.vecPoint[2].at(2);
    });

    vTriangles.resize(vMesh.size());

    //for (sf::ConvexShape& tri : vTriangles) {
    //    tri.setPointCount(3);
    //    tri.setFillColor(sf::Color::White);
    //    //tri.setOutlineThickness(0.5);
    //    //tri.setOutlineColor(sf::Color::Red);
    //}
    vTriangles.setPrimitiveType(sf::Triangles);
    vTriangles.resize(3 * vMesh.size());
    for (std::size_t i = 0; i < vTriangles.getVertexCount(); i++) {
        vTriangles[i].color = sf::Color::White;        
    }
}