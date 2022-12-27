#include "pch.h"
#include <SFML/Graphics.hpp>

#define DLL extern "C" __declspec(dllexport)

#pragma once

constexpr int line_Width = 2;
constexpr int circle_Radius = 4;
constexpr int chess_Radius = 12;
constexpr int block_Size = 1080 * 2 / 3 / 19;
constexpr int window_Size[2] = { 1920 * 2 / 3, 1080 * 2 / 3 };

constexpr char black = 100;
constexpr char white = -100;
constexpr int map_Offset = (1080 * 2 / 3 - 37 * 19) / 2;

sf::Texture get_Map_Texture();
//sf::Texture get_Chess_Texture(sf::Color qi);
void draw();
void compute_Qi(char x, char y);//���㲢������
short check_Qi(char x, char y, short self_Color = 0);//������
void spread_Qi(char x, char y);//������
void spread_Qi(char x, char y, short qi, char color);//������

void compute_Belong(char depth);//�������

bool running = false;
bool update_Fream = true;
bool draw_Belong = false;

sf::RenderWindow* window = nullptr;
sf::RenderTexture fream;
sf::Texture background_Texture;
sf::Sprite background;

struct map_t
{
	short qi;//�� ��Ϊ��������Ҫ�����ֽ�
	short belong;//����
	bool checked;//���������
};

map_t map[19][19] = { {0,false} };
short holds[4] = { 0 };
short wins[2] = { 0 };
sf::Texture chess_Texture[2];
//sf::Sprite chess[19][19];
sf::CircleShape chess[4];


DLL void init(sf::RenderWindow* window)
{
	running = true;
	::window = window;
	fream.create(window_Size[0], window_Size[1]);
	background_Texture = get_Map_Texture();
	background.setTexture(background_Texture);
	for (char i = 0; i < 4; i++)
	{
		chess[i].setRadius(chess_Radius);
		chess[i].setOrigin((float)chess_Radius, (float)chess_Radius);
	}
	chess[0].setFillColor(sf::Color(0x000000FF));
	chess[1].setFillColor(sf::Color(0xFFFFFFFF));
	chess[2].setFillColor(sf::Color(0x00000033));
	chess[3].setFillColor(sf::Color(0xFFFFFF66));

	//for (char i = 0; i < 19; i++)
	//	for (char j = 0; j < 19; j++)
	//	{
	//		chess[i][j].setOrigin((float)(chess_Texture[0].getSize().x / 2), (float)(chess_Texture[0].getSize().y / 2));
	//		chess[i][j].setPosition((float)(block_Size * i + block_Size / 2 + map_Offset),
	//			(float)(block_Size * j + block_Size / 2 + map_Offset));
	//		chess[i][j].setTexture(chess_Texture[(j + i) % 2]);

	//		window->draw(chess[i][j]);
	//	}

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
	//printf_s("update\n");
}

DLL void click(sf::Event::MouseButtonEvent mouseEvent)
{
	sf::Vector2f position = window->mapPixelToCoords({ mouseEvent.x, mouseEvent.y });

	char subscript[2] = { -1, -1 };

	subscript[0] = ((int)position.x - map_Offset) / block_Size;
	subscript[1] = 18 - ((int)position.y - map_Offset) / block_Size;//��֪Ϊ��Ҫ�Լ���תy��

	if (subscript[0] < 0 || subscript[0] > 18)  subscript[0] = -1;
	if (subscript[1] < 0 || subscript[1] > 18)  subscript[1] = -1;

	if (subscript[0] != -1 && subscript[1] != -1)
	{
		switch (mouseEvent.button)
		{
		case sf::Mouse::Right:
			map[subscript[0]][subscript[1]].qi = -1;
			break;
		case sf::Mouse::Middle:
			map[subscript[0]][subscript[1]].qi = 0;
			break;
		default:
			map[subscript[0]][subscript[1]].qi = 1;
			break;
		}
		compute_Qi(subscript[0] - 1, subscript[1]);//����
		compute_Qi(subscript[0] + 1, subscript[1]);
		compute_Qi(subscript[0], subscript[1] - 1);
		compute_Qi(subscript[0], subscript[1] + 1);

		compute_Qi(subscript[0], subscript[1]);//����

		compute_Belong(3);
		update_Fream = true;
		printf_s("put %d %d ����%d\n", subscript[0], subscript[1], map[subscript[0]][subscript[1]].qi);
	}
	printf_s("click %d %d\n", (int)position.x, (int)position.y);
	printf_s("mouse %d %d\n", (int)mouseEvent.x, (int)mouseEvent.y);
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
	case sf::Keyboard::Space:
		draw_Belong = !draw_Belong;
		update_Fream = true;
		break;
	default:
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

//sf::Texture get_Chess_Texture(sf::Color qi) //�Ҳ���⣬ֱ������ԲҲ�а���ΪɶҪ�����ͼ�󵼳���
//{
//	sf::RenderTexture chess;
//	sf::CircleShape circle;
//
//	chess.create(chess_Radius * 2, chess_Radius * 2);
//	circle.setRadius(chess_Radius);
//	circle.setFillColor(qi);
//	chess.draw(circle);
//	return chess.getTexture();
//}

void draw()
{
	fream.draw(background);
	for (char i = 0; i < 19; i++) for (char j = 0; j < 19; j++)
	{
		if (map[i][j].qi > 0)
		{
			chess[0].setPosition((float)(i * block_Size + block_Size / 2) + map_Offset, (float)(j * block_Size + block_Size / 2 + map_Offset));
			fream.draw(chess[0]);
			continue;
		}
		if (map[i][j].qi < 0)
		{
			chess[1].setPosition((float)(i * block_Size + block_Size / 2) + map_Offset, (float)(j * block_Size + block_Size / 2 + map_Offset));
			fream.draw(chess[1]);
			continue;
		}

		if (!draw_Belong) continue;

		if (map[i][j].belong > 0)
		{
			chess[2].setPosition((float)(i * block_Size + block_Size / 2) + map_Offset, (float)(j * block_Size + block_Size / 2 + map_Offset));
			fream.draw(chess[2]);
			continue;
		}
		if (map[i][j].belong < 0)
		{
			chess[3].setPosition((float)(i * block_Size + block_Size / 2) + map_Offset, (float)(j * block_Size + block_Size / 2 + map_Offset));
			fream.draw(chess[3]);
			continue;
		}
	}

	printf_s("draw\n");
}

void compute_Qi(char x, char y)
{
	short qi = 0;

	if (x < 0 || x > 18) return;
	if (y < 0 || y > 18) return; //Խ��

	if (map[x][y].qi == 0) return;//��

	qi = check_Qi(x, y); //��ʹ��checked
	for (char i = 0; i < 19; i++) for (char j = 0; j < 19; j++)
		map[i][j].checked = false; //��ʼ��checked
	spread_Qi(x, y, qi, map[x][y].qi > 0 ? 1 : -1); //������ �п��ܴ���0��������Ҫcolor����
}

short check_Qi(char x, char y, short self_Color) //���̣߳����̰߳�ȫ
{
	short qi = 0;

	if (x < 0 || x > 18) return 0;
	if (y < 0 || y > 18) return 0; //Խ��
	
	if (self_Color == 0) //�Լ������ 
	{
		self_Color = map[x][y].qi > 0 ? 1 : -1; //�趨�Լ�����ɫ ���ƴ�С��ֹ������� ����0����� ����0�ᱻ��return��
		for (char i = 0; i < 19; i++) for (char j = 0; j < 19; j++)
			map[i][j].checked = false; //��ʼ��checked
	}

	if (map[x][y].checked) return 0; //�Ѿ����
	map[x][y].checked = true; //��Ǽ��

	if (map[x][y].qi == 0) return 1; //��

	if (map[x][y].qi * self_Color < 0) return 0; //��ͬ��ɫ��������0 ��˲������ ������0�����

	qi += abs(check_Qi(x - 1, y, self_Color));//��ͬ��ɫ������
	qi += abs(check_Qi(x + 1, y, self_Color));
	qi += abs(check_Qi(x, y - 1, self_Color));
	qi += abs(check_Qi(x, y + 1, self_Color));

	return qi * self_Color; //ת����ɫ
}

void spread_Qi(char x, char y) //���̣߳����̰߳�ȫ
{
	for (char i = 0; i < 19; i++) for (char j = 0; j < 19; j++)
		map[i][j].checked = false; //��ʼ��checked
	spread_Qi(x, y, map[x][y].qi, map[x][y].qi > 0 ? 1 : -1);
}

void spread_Qi(char x, char y, short qi, char color)
{
	if (x < 0 || x > 18) return;
	if (y < 0 || y > 18) return; //Խ��

	if (map[x][y].checked) return; //�Ѿ����
	map[x][y].checked = true; //��Ǽ��

	if (map[x][y].qi * color <= 0) return; //��ͬ��ɫ��գ�ֱ�ӷ��� Ӧ�ò��������

	map[x][y].qi = qi; //��ͬ��ɫ
	spread_Qi(x - 1, y, qi, color);//����
	spread_Qi(x + 1, y, qi, color);//����
	spread_Qi(x, y - 1, qi, color);//����
	spread_Qi(x, y + 1, qi, color);//����
}

void compute_Belong(char depth)
{
	short buffer[19][19] = { 0 };
	short sum = 0;
	for (char i = 0; i < 19; i++) for (char j = 0; j < 19; j++)
	{
		sum = 0;
		if (i != 00) sum += min(abs(map[i - 1][j].qi), 20) * 250 * (map[i - 1][j].qi > 0 ? 1 : -1);
		if (i != 18) sum += min(abs(map[i + 1][j].qi), 20) * 250 * (map[i + 1][j].qi > 0 ? 1 : -1);
		if (j != 00) sum += min(abs(map[i][j - 1].qi), 20) * 250 * (map[i][j - 1].qi > 0 ? 1 : -1);
		if (j != 18) sum += min(abs(map[i][j + 1].qi), 20) * 250 * (map[i][j + 1].qi > 0 ? 1 : -1);
		if (i != 00 && j != 00) sum += min(abs(map[i - 1][j - 1].qi), 20) * 125 * (map[i - 1][j - 1].qi > 0 ? 1 : -1);
		if (i != 00 && j != 18) sum += min(abs(map[i - 1][j + 1].qi), 20) * 125 * (map[i - 1][j + 1].qi > 0 ? 1 : -1);
		if (i != 18 && j != 00) sum += min(abs(map[i + 1][j - 1].qi), 20) * 125 * (map[i + 1][j - 1].qi > 0 ? 1 : -1);
		if (i != 18 && j != 18) sum += min(abs(map[i + 1][j + 1].qi), 20) * 125 * (map[i + 1][j + 1].qi > 0 ? 1 : -1);

		map[i][j].belong = buffer[i][j] = sum; //��ʼ������ 
	}

	for (char times = 0; times < depth; times++)
	{
		//��map��buffer
		for (char i = 0; i < 19; i++) for (char j = 0; j < 19; j++)
		{
			//����ÿһ��λ��
			buffer[i][j] = buffer[i][j] / 10 * 6; //3/5��
			if (i != 00 && map[i][j].qi == 0) buffer[i][j] += map[i - 1][j].belong / 10; //2/5��
			if (i != 18 && map[i][j].qi == 0) buffer[i][j] += map[i + 1][j].belong / 10;
			if (j != 00 && map[i][j].qi == 0) buffer[i][j] += map[i][j - 1].belong / 10;
			if (j != 18 && map[i][j].qi == 0) buffer[i][j] += map[i][j + 1].belong / 10;
		}

		//��buffer��map
		for (char i = 0; i < 19; i++) for (char j = 0; j < 19; j++)
		{
			//����ÿһ��λ��
			map[i][j].belong = map[i][j].belong / 10 * 6; //3/5��
			if (i != 00 && map[i][j].qi == 0) map[i][j].belong += buffer[i - 1][j] / 10; //2/5��
			if (i != 18 && map[i][j].qi == 0) map[i][j].belong += buffer[i + 1][j] / 10;
			if (j != 00 && map[i][j].qi == 0) map[i][j].belong += buffer[i][j - 1] / 10;
			if (j != 18 && map[i][j].qi == 0) map[i][j].belong += buffer[i][j + 1] / 10;
		}
	}

	for (char i = 0; i < 4; i++) holds[i] = 0;
	for (char i = 0; i < 19; i++) for (char j = 0; j < 19; j++)
	{
		if (map[i][j].qi > 0) { holds[0]++; continue; } //��
		if (map[i][j].qi < 0) { holds[1]++; continue; }

		if (map[i][j].belong > 0) { holds[0]++; holds[2]++; continue; } //Ŀ
		if (map[i][j].belong < 0) { holds[1]++; holds[3]++; continue; }
	}
	wins[0] = holds[0] - holds[1]; wins[1] = holds[2] - holds[3];
	printf_s("wins %d %d\n", wins[0], wins[1]);
}
