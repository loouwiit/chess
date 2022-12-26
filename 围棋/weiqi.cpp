#include "pch.h"
#include <SFML/Graphics.hpp>

#define DLL extern "C" __declspec(dllexport)

#pragma once

constexpr int line_Width = 2;
constexpr int circle_Radius = 4;
constexpr int chess_Radius = 12;
constexpr int block_Size = 1080 * 2 / 3 / 19;
constexpr int window_Size[2] = { 1920 * 2 / 3, 1080 * 2 / 3 };

constexpr char black = 50;
constexpr char white = -50;
constexpr int map_Offset = (1080 * 2 / 3 - 37 * 19) / 2;

sf::Texture get_Map_Texture();
sf::Texture get_Chess_Texture(sf::Color color);
void draw();

bool running = false;
bool update_Fream = true;

sf::RenderWindow* window = nullptr;
sf::RenderTexture fream;
sf::Texture background_Texture;
sf::Sprite background;

signed char map[19][19] = { 0 };
sf::Texture chess_Texture[2];
//sf::Sprite chess[19][19];
sf::Sprite chess[2];


DLL void init(sf::RenderWindow* window)
{
	running = true;
	::window = window;
	fream.create(window_Size[0], window_Size[1]);
	background_Texture = get_Map_Texture();
	background.setTexture(background_Texture);
	chess_Texture[0] = get_Chess_Texture(sf::Color(0x000000FF));
	chess_Texture[1] = get_Chess_Texture(sf::Color(0xFFFFFFFF));

	//for (char i = 0; i < 19; i++)
	//	for (char j = 0; j < 19; j++)
	//	{
	//		chess[i][j].setOrigin((float)(chess_Texture[0].getSize().x / 2), (float)(chess_Texture[0].getSize().y / 2));
	//		chess[i][j].setPosition((float)(block_Size * i + block_Size / 2 + map_Offset),
	//			(float)(block_Size * j + block_Size / 2 + map_Offset));
	//		chess[i][j].setTexture(chess_Texture[(j + i) % 2]);

	//		window->draw(chess[i][j]);
	//	}

	chess[0].setOrigin((float)(chess_Texture[0].getSize().x / 2), (float)(chess_Texture[0].getSize().y / 2));
	chess[1].setOrigin((float)(chess_Texture[0].getSize().x / 2), (float)(chess_Texture[0].getSize().y / 2));
	chess[0].setTexture(chess_Texture[0]);
	chess[1].setTexture(chess_Texture[1]);

	update_Fream = true;

	printf_s("init\n");
}

DLL void update()
{
	static sf::Sprite sprite_Fream;
	if (update_Fream)
	{
		draw();
		sprite_Fream.setTexture(fream.getTexture());
		update_Fream = false;
	}
	window->clear();
	window->draw(sprite_Fream);
	window->display();
	printf_s("update\n");
}

DLL void click(sf::Event::MouseButtonEvent mouseEvent)
{
	sf::Vector2f position = window->mapPixelToCoords({ mouseEvent.x, mouseEvent.y });

	map[0][0] = 50;
	update_Fream = true;

	printf_s("click %d %d\n", (int)position.x, (int)position.y);
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

DLL void sleep()
{
	sf::sleep(sf::milliseconds(100));
}

DLL bool is_Running()
{
	return running;
}

DLL void ened()
{
	running = false;
}

sf::Texture get_Map_Texture()
{
	sf::RenderTexture background;
	sf::RectangleShape line[2];
	sf::CircleShape circle;

	//background.create(block_Size*19, block_Size * 19);
	background.create(window_Size[1], window_Size[1]);
	background.clear(sf::Color(0xF0D36FFF));
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
		line[0].setPosition((float)(block_Size / 2 + map_Offset), (float)(i * block_Size + block_Size / 2 + map_Offset));
		line[1].setPosition((float)(i * block_Size + block_Size / 2 + map_Offset), (float)(block_Size / 2 + map_Offset));
		background.draw(line[0]);
		background.draw(line[1]);
	}

	for (char i = 0; i < 9; i++)
	{
		circle.setPosition((float)((i / 3 * 6 + 3) * block_Size + block_Size / 2 + map_Offset),
			(float)((i % 3 * 6 + 3) * block_Size + block_Size / 2) + map_Offset);
		background.draw(circle);
	}

	return background.getTexture();
}

sf::Texture get_Chess_Texture(sf::Color color)
{
	sf::RenderTexture chess;
	sf::CircleShape circle;

	chess.create(chess_Radius * 2, chess_Radius * 2);
	circle.setRadius(chess_Radius);
	circle.setFillColor(color);
	chess.draw(circle);
	return chess.getTexture();
}

void draw()
{
	fream.draw(background);
	for (char i = 0; i < 19; i++) for (char j = 0; j < 19; j++)
	{
		if (map[i][j] == black)
		{
			chess[0].setPosition((float)(i * block_Size + block_Size / 2) + map_Offset, (float)(j * block_Size + block_Size / 2 + map_Offset));
			fream.draw(chess[0]);
			continue;
		}
		if (map[i][j] == white)
		{
			chess[1].setPosition((float)(i * block_Size + block_Size / 2) + map_Offset, (float)(j * block_Size + block_Size / 2 + map_Offset));
			fream.draw(chess[1]);
			continue;
		}
	}

	printf_s("draw\n");
}
