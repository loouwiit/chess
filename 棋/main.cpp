#include <iostream>
#include <Windows.h>
#include <SFML/Graphics.hpp>

constexpr int window_Size[2] = { 1920 * 2 / 3, 1080 * 2 / 3 };

int main(int argc,char * argv[]);
void initlize();
bool load_DLL(const char path[]);

void (*init)(sf::RenderWindow* window) = nullptr;
void (*update)(void) = nullptr;
void (*click)(sf::Event::MouseButtonEvent mouseEvent) = nullptr;
void (*mouse)(sf::Event::MouseMoveEvent mouseEvent) = nullptr;
void (*keyborad)(sf::Event::KeyEvent keyEvent) = nullptr;
void (*sleep)(void) = nullptr;
void (*ened)(void) = nullptr;

sf::RenderWindow window;
sf::View view;
sf::Font font;
HINSTANCE hDLL;

int main(int argc, char* argv[])
{
	initlize();

	while (window.isOpen())
	{
		static sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyPressed:
				if (keyborad != nullptr)keyborad(event.key);
				break;
			case sf::Event::MouseMoved:
				if (mouse != nullptr) mouse(event.mouseMove);
				break;
			case sf::Event::MouseButtonPressed:
				if (click != nullptr)click(event.mouseButton);
				break;
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		if (ened != nullptr)ened();
		if (sleep != nullptr) sleep();
		else sf::sleep(sf::milliseconds(100));
	}

	load_DLL(nullptr);

	return 0;
}

void initlize()
{
	window.create(sf::VideoMode(window_Size[0], window_Size[1]), L"∆Â");
	view.setSize((float)window_Size[0], (float)window_Size[1]);
	view.setCenter((float)(window_Size[0] / 2), (float)(window_Size[1] / 2));
	window.setView(view);

	sf::Text text;
	if (!font.loadFromFile("C:/windows/fonts/msyh.ttc")) font.loadFromFile("C:/windos/fonts/msyh.ttf");
	text.setFont(font);
	text.setPosition((float)(window_Size[0] / 2), (float)(window_Size[1] / 2));
	text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
	text.setString(L"º”‘ÿ÷–°≠°≠");
	window.draw(text);
	window.display();

	if (!load_DLL("./dll/weiqi.dll"))
	{
		text.setString(L"º”‘ÿ ß∞‹");
		window.draw(text);
		window.display();
		return;
	}

	init(&window);
}

bool load_DLL(const char path[])
{
	if (path == nullptr)
	{
		if (ened != nullptr) ened();
		FreeLibrary(hDLL);

		hDLL = nullptr;
		init = nullptr;
		update = nullptr;
		click = nullptr;
		mouse = nullptr;
		keyborad = nullptr;
		sleep = nullptr;
		ened = nullptr;
		return true;
	}

	hDLL = LoadLibraryA(path);
	if (hDLL == nullptr) { FreeLibrary(hDLL); return false; }

	init = (void (*)(sf::RenderWindow*)) GetProcAddress(hDLL, "init");
	if (init == nullptr) { FreeLibrary(hDLL); return false; }

	update = (void (*)(void)) GetProcAddress(hDLL, "update");
	if (update == nullptr) { FreeLibrary(hDLL); return false; }

	click = (void (*)(sf::Event::MouseButtonEvent)) GetProcAddress(hDLL, "click");
	if (click == nullptr) { FreeLibrary(hDLL); return false; }

	mouse = (void (*)(sf::Event::MouseMoveEvent)) GetProcAddress(hDLL, "mouse");
	if (mouse == nullptr) { FreeLibrary(hDLL); return false; }

	keyborad = (void (*)(sf::Event::KeyEvent)) GetProcAddress(hDLL, "keyborad");
	if (keyborad == nullptr) { FreeLibrary(hDLL); return false; }

	sleep = (void (*)(void)) GetProcAddress(hDLL, "sleep");
	if (sleep == nullptr) { FreeLibrary(hDLL); return false; }

	ened = (void (*)(void)) GetProcAddress(hDLL, "ened");
	if (ened == nullptr) { FreeLibrary(hDLL); return false; }

	return true;
}
