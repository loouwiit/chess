#include "pch.h"
#include <SFML/Graphics.hpp>

#define DLL extern "C" __declspec(dllexport)

#pragma once

constexpr int line_Width = 2;
constexpr int circle_Radius = 4;
constexpr int block_Size = 1080 * 2 / 3 / 19;
constexpr int window_Size[2] = { 1920 * 2 / 3, 1080 * 2 / 3 };

sf::Texture get_Map_Texture();

sf::RenderWindow* window = nullptr;
sf::Texture map_Texture;
sf::Sprite map;

DLL void init(sf::RenderWindow* window)
{
	::window = window;
	map_Texture = get_Map_Texture();
	map.setTexture(map_Texture);
	window->draw(map);
	window->display();
}

DLL void update()
{

}

DLL void click(sf::Event::MouseButtonEvent mouseEvent)
{

}

DLL void mouse(sf::Event::MouseMoveEvent mouseEvent)
{

}

DLL void keyborad(sf::Event::KeyEvent keyEvent)
{
	switch (keyEvent.code)
	{
	case sf::Keyboard::Escape:
		window->close();
		break;
	}
}

DLL void ened()
{

}

sf::Texture get_Map_Texture()
{
	constexpr int offset = (1080 * 2 / 3 - 37 * 19) / 2;

	sf::RenderTexture map;
	sf::RectangleShape line[2];
	sf::CircleShape circle;

	//map.create(block_Size*19, block_Size * 19);
	map.create(window_Size[1], window_Size[1]);
	map.clear(sf::Color(0xF0D36FFF));
	line[0].setTexture(nullptr);
	line[1].setTexture(nullptr);
	line[0].setSize({ (float)((block_Size * 18) + line_Width), (float)line_Width });
	line[1].setSize({ (float)line_Width, (float)((block_Size * 18) + line_Width) });
	line[0].setOrigin((float)(line_Width / 2), (float)(line_Width / 2));
	line[1].setOrigin((float)(line_Width / 2), (float)(line_Width / 2));
	line[0].setFillColor(sf::Color(0x000000FF));
	line[1].setFillColor(sf::Color(0x000000FF));
	circle.setTexture(nullptr);
	circle.setRadius(circle_Radius);
	circle.setOrigin(circle_Radius, circle_Radius);
	circle.setFillColor(sf::Color(0x000000FF));

	for (char i = 0; i < 19; i++)
	{
		line[0].setPosition((float)(block_Size / 2 + offset), (float)(i * block_Size + block_Size / 2 + offset));
		line[1].setPosition((float)(i * block_Size + block_Size / 2 + offset), (float)(block_Size / 2 + offset));
		map.draw(line[0]);
		map.draw(line[1]);
	}

	for (char i = 0; i < 9; i++)
	{
		circle.setPosition((float)((i / 3 * 6 + 3) * block_Size + block_Size / 2 + offset),
			(float)((i % 3 * 6 + 3) * block_Size + block_Size / 2) + offset);
		map.draw(circle);
	}

	return map.getTexture();
}
