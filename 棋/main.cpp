#include <SFML/Graphics.hpp>
#include "const.h"

int main(int argc,char * argv[]);
sf::Texture get_Map_Texture();

int main(int argc, char* argv[])
{
	sf::RenderWindow window;
	sf::View view;
	sf::Texture map_Texture;
	sf::Sprite map;

	map_Texture = get_Map_Texture();
	map.setTexture(map_Texture);

	window.create(sf::VideoMode(window_Size[0],window_Size[1]), L"Æå");
	view.setSize((float)window_Size[0], (float)window_Size[1]);
	view.setCenter((float)(window_Size[0] / 2), (float)(window_Size[1] / 2));
	window.setView(view);
	window.draw(map);
	window.display();
	
	while (window.isOpen())
	{
		static sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case ::sf::Event::Closed:
				window.close();
				break;
			}
		}

		sf::sleep(sf::milliseconds(100));
	}

	return 0;
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
