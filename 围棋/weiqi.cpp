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

constexpr int line_Width = 2;
constexpr int circle_Radius = 4;
constexpr int chess_Radius = 12;
constexpr int UI_Chess_Radius = 20;
constexpr int block_Size = 1080 * 2 / 3 / 19;
constexpr int window_Size[2] = { 1920 * 2 / 3, 1080 * 2 / 3 };

constexpr int map_Offset = (1080 * 2 / 3 - 37 * 19) / 2;

constexpr int text_Position[2][4] = { 860,550,980 - text_Position[0][0],580-text_Position[0][1],
	1050,550,1170 - text_Position[1][0],580 - text_Position[1][1] };

namespace checked
{
	constexpr char check_true = 1 << 0;
	constexpr char check_false = ~check_true;

	constexpr char spread_true = 1 << 1;
	constexpr char spread_false = ~spread_true;

	constexpr char spread_Zero_true = 1 << 2;
	constexpr char spread_Zero_false = ~spread_Zero_true;
}

struct map_t
{
	short qi = 0;//�� ��Ϊ��������Ҫ�����ֽ�
	short belong = 0;//����
	char checked = 0;//���������
};

sf::Texture get_Map_Texture();
sf::Texture	get_UI_Texture();
//sf::Texture get_Chess_Texture(sf::Color qi);
void draw();
void compute_Qi(char x, char y);//���㲢������
short check_Qi(char x, char y, short self_Color = 0);//������
void spread_Qi(char x, char y);//������
void spread_Qi(char x, char y, short qi, char color, char checked_true = checked::spread_true, bool except_Self = false);//������

void compute_Belong(char depth);//�������

bool update_Fream = true;
bool draw_Details = false;
bool next_Color = true;

sf::RenderWindow* window = nullptr;
sf::RenderTexture fream;

sf::Font font;

sf::Texture background_Texture;
sf::Sprite background;
sf::Texture UI_Texture;
sf::Sprite UI_backgreoud;

map_t map[19][19] = { {0,false} };
short holds[4] = { 0 };
short wins[2] = { 0 };
sf::Texture chess_Texture[2];
//sf::Sprite chess[19][19];
sf::CircleShape chess[6];
sf::Text UI_Text[3];
sf::Text number_Text[10];
sf::IntRect UI_Rect[2];

function_Pointer end_Function = nullptr;

DLL void init(sf::RenderWindow* window, function_Pointer end_Function)
{
	//running = true;
	::window = window;
	::end_Function = end_Function;
	fream.create(window_Size[0], window_Size[1]);

	if (!font.loadFromFile("C:/windows/fonts/msyh.ttc")) font.loadFromFile("C:/windos/fonts/msyh.ttf");

	background_Texture = get_Map_Texture();
	background.setTexture(background_Texture);
	UI_Texture = get_UI_Texture();
	UI_backgreoud.setTexture(UI_Texture);

	UI_backgreoud.setPosition((float)window_Size[1], 0.f);

	for (char i = 0; i < 3; i++)
	{
		UI_Text[i].setFont(font);
		UI_Text[i].setFillColor(sf::Color(0x000000FF));
		UI_Text[i].setScale(1, -1); //��֪Ϊ����Ҫ��Y��Գ�һ�� ������view��ԭ��
	}

	for (char i = 0; i < 2; i++)
	{
		UI_Text[i].setString(L"0(0)");
		UI_Text[i].setOrigin(UI_Text[i].getGlobalBounds().width / 2, UI_Text[i].getGlobalBounds().height / 2);
		UI_Text[i].setPosition((float)(186 + 186 * (i % 2) + window_Size[1]), (float)(200 + 180 * 2 - 80));//����ת�����鷳 ��Ҫƫ��window_Size[1]
	}

	UI_Text[2].setString(L"�ڰ���ͬ");
	UI_Text[2].setOrigin(UI_Text[2].getGlobalBounds().width / 2, UI_Text[2].getGlobalBounds().height / 2);
	UI_Text[2].setPosition(1000, 360);

	char buffer[3] = "";

	for (char i = 0; i < 10; i++)
	{
		sprintf_s(buffer, sizeof(buffer), "%d", i + 1);
		number_Text[i].setFont(font);
		number_Text[i].setFillColor(sf::Color(0x999999FF));
		number_Text[i].setScale(0.5, -0.5);
		number_Text[i].setString(buffer);
		number_Text[i].setOrigin(number_Text[i].getGlobalBounds().width, number_Text[i].getGlobalBounds().height); //�����Ϊʲô������
	}
	number_Text[9].setString("+");

	for (char i = 0; i < 2; i++)
	{
		UI_Rect[i].left = text_Position[i][0];
		UI_Rect[i].top = text_Position[i][1];
		UI_Rect[i].width = text_Position[i][2];
		UI_Rect[i].height = text_Position[i][3];
	}

	for (char i = 0; i < 4; i++)
	{
		chess[i].setRadius(chess_Radius);
		chess[i].setOrigin((float)chess_Radius, (float)chess_Radius);
	}
	for (char i = 4; i < 6; i++)
	{
		chess[i].setRadius(UI_Chess_Radius);
		chess[i].setOrigin((float)UI_Chess_Radius, (float)UI_Chess_Radius);
		chess[i].setRadius(UI_Chess_Radius);
		chess[i].setOrigin((float)UI_Chess_Radius, (float)UI_Chess_Radius);
	}

	chess[0].setFillColor(sf::Color(0x000000FF));
	chess[1].setFillColor(sf::Color(0xFFFFFFFF));
	chess[2].setFillColor(sf::Color(0x00000033));
	chess[3].setFillColor(sf::Color(0xFFFFFF66));
	chess[4].setFillColor(sf::Color(0x000000FF));
	chess[5].setFillColor(sf::Color(0xFFFFFFFF));
	//chess[6].setFillColor(sf::Color(0x000000FF));
	//chess[7].setFillColor(sf::Color(0xFFFFFFFF));

	chess[4].setPosition(800,160);
	chess[5].setPosition(800,160);
	//chess[6].setPosition(800,360);
	//chess[7].setPosition(800,360);

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

	//printf_s("init\n");
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

	//printf_s("click %d %d\n", (int)position.x, (int)position.y);
	//printf_s("mouse %d %d\n", (int)mouseEvent.x, (int)mouseEvent.y);

	char subscript[2] = { -1, -1 };

	subscript[0] = ((int)position.x - map_Offset) / block_Size;
	subscript[1] = 18 - ((int)position.y - map_Offset) / block_Size;//��֪Ϊ��Ҫ�Լ���תy��

	if (subscript[0] < 0 || subscript[0] > 18)  subscript[0] = -1;
	if (subscript[1] < 0 || subscript[1] > 18)  subscript[1] = -1;

	if (subscript[0] != -1 && subscript[1] != -1)
	{
		if (mouseEvent.button == sf::Mouse::Right)
		{
			map[subscript[0]][subscript[1]].qi = -map[subscript[0]][subscript[1]].qi;

			compute_Qi(subscript[0] - 1, subscript[1]);//����
			compute_Qi(subscript[0] + 1, subscript[1]);
			compute_Qi(subscript[0], subscript[1] - 1);
			compute_Qi(subscript[0], subscript[1] + 1);

			compute_Qi(subscript[0], subscript[1]);//����

			compute_Belong(3);
			update_Fream = true;
			//printf_s("change %d %d ����%d\n", subscript[0], subscript[1], map[subscript[0]][subscript[1]].qi);
		}

		if (map[subscript[0]][subscript[1]].qi == 0) //�յ�
		{
			map[subscript[0]][subscript[1]].qi = next_Color ? 1 : -1;
			compute_Qi(subscript[0] - 1, subscript[1]);//����
			compute_Qi(subscript[0] + 1, subscript[1]);
			compute_Qi(subscript[0], subscript[1] - 1);
			compute_Qi(subscript[0], subscript[1] + 1);

			compute_Qi(subscript[0], subscript[1]);//����

			if (map[subscript[0]][subscript[1]].qi != 0) //����
			{
				next_Color = !next_Color; //��������
				compute_Belong(3);
				update_Fream = true;
				//printf_s("put %d %d = %d\n", subscript[0], subscript[1], map[subscript[0]][subscript[1]].qi);
			}
			else
			{
				for (char i = 0; i < 19; i++)for (char j = 0; j < 19; j++)
					map[i][j].checked &= checked::spread_false;//�Ƴ���־
				spread_Qi(subscript[0], subscript[1], next_Color ? 1 : -1, 0, checked::spread_true, true);//���û�ȥ �����Լ�
			}
		}
	}

	//����������

	if (UI_Rect[0].contains((sf::Vector2i)position))
	{
		//�ؿ�
		for (char i = 0; i < 19; i++) for (char j = 0; j < 19; j++)
		{
			map[i][j].qi = 0;
			map[i][j].belong = 0;
		}
		next_Color = true;
		for (char i = 0; i < 2; i++) wins[i] = 0;

		for (char i = 0; i < 4; i++) holds[i] = 0;
		update_Fream = true;
		//printf_s("restart\n");
	}

	if (UI_Rect[1].contains((sf::Vector2i)position))
	{
		//��ʾ����
		draw_Details = !draw_Details;
		update_Fream = true;
		//printf_s("show detail\n");
	}
}

DLL void mouse(sf::Event::MouseMoveEvent mouseEvent)
{

}

DLL void keyborad(sf::Event::KeyEvent keyEvent)
{
	switch (keyEvent.code)
	{
	case sf::Keyboard::Escape:
		ened(true);
		break;
	case sf::Keyboard::Space:
		draw_Details = !draw_Details;
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

DLL void ened(bool call_End_Function)
{
	if (call_End_Function) end_Function(NULL);
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

sf::Texture get_UI_Texture()
{
	sf::RenderTexture background;
	sf::CircleShape chess[2];
	sf::Text text[3];

	background.create(window_Size[0] - window_Size[1], window_Size[1]);
	background.clear(sf::Color(0xF0D36FFF));

	chess[0].setFillColor(sf::Color(0x000000FF));
	chess[1].setFillColor(sf::Color(0xFFFFFFFF));
	for (char i = 0; i < 2; i++)
	{
		chess[i].setRadius(UI_Chess_Radius);
		chess[i].setOrigin(UI_Chess_Radius, UI_Chess_Radius);
		chess[i].setPosition((float)(186 + 186 * (i % 2)), (float)(200 + 180 * (i / 2)));
		background.draw(chess[i]);
	}

	text[0].setString(L"������Ϸ");
	text[1].setString(L"��ʾ����");
	text[2].setString(L"Χ��");

	for (char i = 0; i < 3; i++)
	{
		text[i].setFont(font);
		text[i].setFillColor(sf::Color(0x000000FF));
		text[i].setOrigin(text[i].getGlobalBounds().width / 2, text[i].getGlobalBounds().height / 2);
	}

	//text[0].setPosition((float)(186 - 100), (float)(200 + 180));

	for (char i = 0; i < 2; i++)
		text[i].setPosition((float)(200 + 186 * (i % 2)), (float)(200 + 180 * 2));

	text[2].setPosition(280, 60);

	for (char i = 0; i < 3; i++)
		background.draw(text[i]);

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
	fream.draw(UI_backgreoud);

	for (char i = 0; i < 19; i++) for (char j = 0; j < 19; j++) //��Ⱦ����
	{
		if (map[i][j].qi > 0)
		{
			chess[0].setPosition((float)(i * block_Size + block_Size / 2) + map_Offset, (float)(j * block_Size + block_Size / 2 + map_Offset));
			fream.draw(chess[0]);
			if (draw_Details)
			{
				number_Text[min(map[i][j].qi, 10) - 1].setPosition((float)(i * block_Size + block_Size / 2) + map_Offset, (float)(j * block_Size + block_Size / 2 + map_Offset));
				fream.draw(number_Text[min(map[i][j].qi, 10) - 1]);
			}
			continue;
		}
		if (map[i][j].qi < 0)
		{
			chess[1].setPosition((float)(i * block_Size + block_Size / 2) + map_Offset, (float)(j * block_Size + block_Size / 2 + map_Offset));
			fream.draw(chess[1]);
			if (draw_Details)
			{
				number_Text[min(-map[i][j].qi, 10) - 1].setPosition((float)(i * block_Size + block_Size / 2) + map_Offset, (float)(j * block_Size + block_Size / 2 + map_Offset));
				fream.draw(number_Text[min(-map[i][j].qi, 10) - 1]);
			}
			continue;
		}

		if (!draw_Details) continue;

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

	//��ȾUI
	{
		
		//��Ⱦ����
		{
			static char buffer[9] = "";

			for (int i = 0; i < 2; i++)
			{
				sprintf_s(buffer, sizeof(buffer), "%d(%d)", holds[i], holds[i + 2]);
				UI_Text[i].setString(buffer);
				UI_Text[i].setOrigin(UI_Text[i].getGlobalBounds().width / 2, UI_Text[i].getGlobalBounds().height / 2); //��һ�ξ���һ��
				fream.draw(UI_Text[i]);
			}
		}

		//��Ⱦʤ��
		{
			wchar_t buffer[20] = L"�ڰ��൱";
			if (wins[0] * wins[1] == 0)
			{
				//����
				if (wins[0] != wins[1])
				{
					//һ����
					if (wins[0] > 0) swprintf_s(buffer, sizeof(buffer) / sizeof(wchar_t), L"�����Ȱ�%d��", wins[0]);
					else if (wins[0] < 0) swprintf_s(buffer, sizeof(buffer) / sizeof(wchar_t), L"�����Ⱥ�%d��", -wins[0]);
					else if (wins[1] > 0) swprintf_s(buffer, sizeof(buffer) / sizeof(wchar_t), L"�����Ȱ�%dĿ", wins[1]);
					else if (wins[1] < 0) swprintf_s(buffer, sizeof(buffer) / sizeof(wchar_t), L"�����Ⱥ�%dĿ", -wins[1]);
				}
				//������
			}
			else if (wins[0] * wins[1] > 0)
			{
				//ͬ��
				if (wins[0] > 0) swprintf_s(buffer, sizeof(buffer)/sizeof(wchar_t), L"�����Ȱ�%d��%dĿ", wins[0], wins[1]);
				else swprintf_s(buffer, sizeof(buffer) / sizeof(wchar_t), L"�����Ⱥ�%d��%dĿ", -wins[0], -wins[1]);
			}
			else
			{
				//���
				if (wins[0] > 0) swprintf_s(buffer, sizeof(buffer) / sizeof(wchar_t), L"�����Ȱ�%d�ӣ������Ⱥ�%dĿ", wins[0], -wins[1]);
				else swprintf_s(buffer, sizeof(buffer) / sizeof(wchar_t), L"�����Ⱥ�%d�ӣ������Ȱ�%dĿ", -wins[0], wins[1]);
			}
			UI_Text[2].setString(buffer);
			UI_Text[2].setOrigin(UI_Text[2].getGlobalBounds().width / 2, UI_Text[2].getGlobalBounds().height / 2);
			fream.draw(UI_Text[2]);
		}

		//fream.draw(chess[wins[0] > wins[1] ? 6 : 7]);
		
		//��Ⱦ��һ��
		fream.draw(chess[next_Color ? 4 : 5]);
	}

	//printf_s("draw\n");
}

void compute_Qi(char x, char y)
{
	short qi = 0;
	
	if (x < 0 || x > 18) return;
	if (y < 0 || y > 18) return; //Խ��

	if (map[x][y].qi == 0) return;//��

	qi = check_Qi(x, y); //��ʹ��checked checked�ѷ��룬���ص���
	if (map[x][y].qi == qi) return; //��ֵ��ͬ��ֱ�ӷ���
	for (char i = 0; i < 19; i++) for (char j = 0; j < 19; j++)
		map[i][j].checked &= checked::spread_false; //��ʼ��checked
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
			map[i][j].checked &= checked::check_false; //��ʼ��checked
	}

	if (map[x][y].checked & checked::check_true) return 0; //�Ѿ����
	map[x][y].checked |= checked::check_true; //��Ǽ��

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
		map[i][j].checked &= checked::spread_false; //��ʼ��checked
	spread_Qi(x, y, map[x][y].qi, map[x][y].qi > 0 ? 1 : -1);
}

void spread_Qi(char x, char y, short qi, char color, char checked_true, bool except_Self)
{
	//printf_s("[debug]spread at %d %d param qi = %d color = %d checked_true = %d\n", x, y, qi, color, checked_true);

	if (x < 0 || x > 18) return;
	if (y < 0 || y > 18) return; //Խ��

	if (map[x][y].checked & checked_true) return; //�Ѿ����
	map[x][y].checked |= checked_true; //��Ǽ��

	if (color == 0) //����ģʽ
	{
		if (map[x][y].qi != 0) //����
		{
			if (checked_true == checked::spread_true) map[x][y].checked &= checked::spread_false; //���ܵ��¶�θ��£������Ҫ����

			short new_qi = check_Qi(x, y);
			if (map[x][y].qi == new_qi) return; //ǰ���޲ֱ�ӷ���

			for (char i = 0; i < 19; i++) for (char j = 0; j < 19; j++)
				map[i][j].checked &= checked::spread_Zero_false; //�����Ӵ�����־
			//printf_s("reput %d %d = %d\n", x, y, new_qi);
			spread_Qi(x, y, new_qi, map[x][y].qi > 0 ? 1 : -1, checked::spread_Zero_true); //�����𣬽��д���
			return;
		}

		if (!except_Self) map[x][y].qi = qi; //��һ��������

		spread_Qi(x - 1, y, qi, color, checked_true);//����
		spread_Qi(x + 1, y, qi, color, checked_true);//����
		spread_Qi(x, y - 1, qi, color, checked_true);//����
		spread_Qi(x, y + 1, qi, color, checked_true);//����

		return;
	}
	
	if (map[x][y].qi == 0) return; //���ҷǷ���ģʽ��ֱ�ӷ���

	if (map[x][y].qi * color < 0) //��ͬ��ɫ
	{
		if (qi != 0) return; //û��������ֱ�ӷ���

		if (checked_true == checked::spread_true) map[x][y].checked &= checked::spread_false; //���ܵ��¶�θ��£������Ҫ����

		short new_qi = check_Qi(x, y);
		if (map[x][y].qi == new_qi) return; //ǰ���޲ֱ�ӷ���

		for (char i = 0; i < 19; i++) for (char j = 0; j < 19; j++)
			map[i][j].checked &= checked::spread_Zero_false; //�����Ӵ�����־
		//printf_s("zero %d %d = %d\n", x, y, new_qi);
		spread_Qi(x, y, new_qi, map[x][y].qi > 0 ? 1 : -1, checked::spread_Zero_true); //�����𣬽��д���
		return;
	}

	map[x][y].qi = qi; //��ͬ��ɫ
	//printf_s("spread %d %d = %d\n", x, y, qi);
	spread_Qi(x - 1, y, qi, color, checked_true);//����
	spread_Qi(x + 1, y, qi, color, checked_true);//����
	spread_Qi(x, y - 1, qi, color, checked_true);//����
	spread_Qi(x, y + 1, qi, color, checked_true);//����
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
			if (i != 00 && map[i - 1][j].qi == 0) buffer[i][j] += map[i - 1][j].belong / 10; //2/5��
			if (i != 18 && map[i + 1][j].qi == 0) buffer[i][j] += map[i + 1][j].belong / 10;
			if (j != 00 && map[i][j - 1].qi == 0) buffer[i][j] += map[i][j - 1].belong / 10;
			if (j != 18 && map[i][j + 1].qi == 0) buffer[i][j] += map[i][j + 1].belong / 10;
		}

		//��buffer��map
		for (char i = 0; i < 19; i++) for (char j = 0; j < 19; j++)
		{
			//����ÿһ��λ��
			map[i][j].belong = map[i][j].belong / 10 * 6; //3/5��
			if (i != 00 && map[i - 1][j].qi == 0) map[i][j].belong += buffer[i - 1][j] / 10; //2/5��
			if (i != 18 && map[i + 1][j].qi == 0) map[i][j].belong += buffer[i + 1][j] / 10;
			if (j != 00 && map[i][j - 1].qi == 0) map[i][j].belong += buffer[i][j - 1] / 10;
			if (j != 18 && map[i][j + 1].qi == 0) map[i][j].belong += buffer[i][j + 1] / 10;
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
	//printf_s("wins %d %d\n", wins[0], wins[1]);
}
