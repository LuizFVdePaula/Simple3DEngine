#include "AbstractObject.hpp"

int main() {
    const float width = 1000;
    const float height = 800;
    const float fov = 90 * 3.141592 / 180;
    const float zfar = 1000;
    const float znear = 0.1;
    
    sf::RenderWindow window(sf::VideoMode(width, height), "my window!", sf::Style::Default);

    Camera cam(width, height, fov, zfar, znear);

    CustomObject mountains("../obj/mountains.obj", &cam);
    mountains.transform_mesh(translationMatrix4D(0, 10, 0) * rotationMatrix4DX(3.141592));
    mountains.update_triangles();

    while (window.isOpen()) {
        window.clear();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::A: {
                        cam.MoveX(-1); break;
                    }
                    case sf::Keyboard::D: {
                        cam.MoveX(1); break;
                    }
                    case sf::Keyboard::W: {
                        cam.MoveZ(1); break;
                    }
                    case sf::Keyboard::S: {
                        cam.MoveZ(-1); break;
                    }
                    case sf::Keyboard::Up: {
                        cam.MoveY(1); break;
                    }
                    case sf::Keyboard::Down: {
                        cam.MoveY(-1); break;
                    }
                    case sf::Keyboard::Q: {
                        cam.RotateY(-0.1); break;
                    }
                    case sf::Keyboard::E: {
                        cam.RotateY(0.1); break;
                    }
                    case sf::Keyboard::Left: {
                        cam.RotateX(-0.1); break;
                    }
                    case sf::Keyboard::Right: {
                        cam.RotateX(0.1); break;
                    }
                    default: {
                        break;
                    }
                }
                mountains.update_triangles();
            }
        }
        window.draw(mountains);
        window.display();
    }
}