#pragma once
#include "pch.h"
#include <SFML/Graphics.hpp>
#define DLL extern "C" __declspec(dllexport)

using function_Pointer = void(*)(char dll_Path[]);

DLL void init(sf::RenderWindow* window, function_Pointer end_Function);
DLL void update();
DLL void click(sf::Event::MouseButtonEvent mouseEvent);
DLL void mouse(sf::Event::MouseMoveEvent mouseEvent);
DLL void keyborad(sf::Event::KeyEvent keyEvent);
DLL void sleep();
DLL void ened(bool call_End_Function);

constexpr int window_Size[2] = { 1920 * 2 / 3, 1080 * 2 / 3 };

sf::RenderWindow* window = nullptr;
sf::RenderTexture fream;
sf::Font font;

function_Pointer end_Function = nullptr;

char* dll_Path = nullptr;
bool update_Fream = true;

DLL void init(sf::RenderWindow* window, function_Pointer end_Function)
{
	::window = window;
	::end_Function = end_Function;
	
	fream.create(window_Size[0], window_Size[1]);

	sf::Text text;

	if (!font.loadFromFile("C:/windows/fonts/msyh.ttc")) font.loadFromFile("C:/windos/fonts/msyh.ttf");

	text.setFont(font);
	text.setFillColor(sf::Color(0xFFFFFFFF));
	text.setPosition((float)(window_Size[0] / 2), (float)(window_Size[1] / 2));
	text.setString(L"dll loader\nµã»÷ÆÁÄ»¼ÓÔØÎ§Æå");
	text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
	text.setScale(1, -1);
	fream.draw(text);
}

DLL void update()
{
	static sf::Sprite sprite_Fream;
	if (update_Fream)
	{
		//draw();
		sprite_Fream.setTexture(fream.getTexture());
		update_Fream = false;
	}
	window->clear();
	window->draw(sprite_Fream);
	window->display();
}

DLL void click(sf::Event::MouseButtonEvent mouseEvent)
{
	ened(true);
}

DLL void mouse(sf::Event::MouseMoveEvent mouseEvent) {}

DLL void keyborad(sf::Event::KeyEvent keyEvent)
{
	switch (keyEvent.code)
	{
	case sf::Keyboard::Escape:
		window->close();
		break;
	}
}

DLL void sleep()
{
	sf::sleep(sf::milliseconds(100));
}

DLL void ened(bool call_End_Function)
{
	if (!call_End_Function) return;
	dll_Path = new char[100];
	strcpy_s(dll_Path, 100, ".\\dll\\weiqi.dll");
	end_Function(dll_Path);
}
