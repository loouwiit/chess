#pragma once
#include <SFML/Graphics.hpp>
#define DLL extern "C" __declspec(dllexport)

using function_Pointer = void(*)(char dll_Path[]);

function_Pointer end_Function = nullptr;

DLL void init(sf::RenderWindow* window, function_Pointer end_Function); //�������ʱ(ened�������)����end_Function ����Ϊchar* ���򽫼��ش��������dll ����ȱʡ������loader.dll
DLL void update();
DLL void click(sf::Event::MouseButtonEvent mouseEvent);
DLL void mouse(sf::Event::MouseMoveEvent mouseEvent);
DLL void keyborad(sf::Event::KeyEvent keyEvent);
DLL void sleep();
DLL void ened(bool call_End_Function);
