#pragma once
#include <SFML/Graphics.hpp>
#define DLL extern "C" __declspec(dllexport)

DLL void init(sf::RenderWindow* window);
DLL void update();
DLL void click(sf::Event::MouseButtonEvent mouseEvent);
DLL void mouse(sf::Event::MouseMoveEvent mouseEvent);
DLL void keyborad(sf::Event::KeyEvent keyEvent);
DLL void ened();