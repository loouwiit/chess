#include <iostream>
#include <Windows.h>
#include <SFML/Graphics.hpp>

#pragma region main函数定义
#ifdef _DEBUG
#define MAIN int main(int argc, char* argv[])
#else
#ifdef WIN32
#include <wtypes.h>
#define MAIN int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
#define MAIN int WinMain(int argc, char* argv[])
#endif
#endif
#pragma endregion

constexpr int window_Size[2] = { 1920 * 2 / 3, 1080 * 2 / 3 };
constexpr char loader_Path[] = ".\\dll\\loader.dll";

using function_Pointer = void(*)(char dll_Path[]);

MAIN;
void initlize();
bool load_DLL(const char path[]);
void change_DLL();
void return_Function(char path[]);

void (*init)(sf::RenderWindow* window, function_Pointer end_Function) = nullptr;
void (*update)(void) = nullptr;
void (*click)(sf::Event::MouseButtonEvent mouseEvent) = nullptr;
void (*mouse)(sf::Event::MouseMoveEvent mouseEvent) = nullptr;
void (*keyborad)(sf::Event::KeyEvent keyEvent) = nullptr;
void (*sleep)(void) = nullptr;
void (*ened)(bool call_End_Function) = nullptr;

sf::RenderWindow window;
sf::View view;
sf::Font font;
HINSTANCE hDLL;
char* next_DLL_Path = nullptr;

MAIN
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
				if (click != nullptr)
					click(event.mouseButton);
				break;
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		//if (is_Running != nullptr)
		//	if (!is_Running())
		//	{
		//		load_DLL(nullptr);
		//	}
		if (update != nullptr) update();
		if (sleep != nullptr) sleep();
		else sf::sleep(sf::milliseconds(100));
	}

	load_DLL(nullptr);

	return 0;
}

void initlize()
{
	window.create(sf::VideoMode(window_Size[0], window_Size[1]), L"棋");
	view.setSize((float)window_Size[0], (float)window_Size[1]);
	view.setCenter((float)(window_Size[0] / 2), (float)(window_Size[1] / 2));
	window.setView(view);

	sf::Image icon;
	icon.loadFromFile("./res/棋.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	sf::Text text;
	if (!font.loadFromFile("C:/windows/fonts/msyh.ttc")) font.loadFromFile("C:/windos/fonts/msyh.ttf");
	text.setFont(font);
	text.setPosition((float)(window_Size[0] / 2), (float)(window_Size[1] / 2));
	text.setString(L"加载中……");
	text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
	window.draw(text);
	window.display();

	if (!load_DLL(loader_Path))
	{
		text.setString(L"加载失败");
		window.draw(text);
		window.display();
		return;
	}
}

bool load_DLL(const char path[])
{
	if (path == nullptr)
	{
		if (ened != nullptr) ened(false);
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

	//printf_s("%s\n", path);
	hDLL = LoadLibraryA(path); //勿用'/'，要用'\\'
	if (hDLL == nullptr) { /*FreeLibrary(hDLL);*/ return false; } //C6387 hDLL == NULL 但是_IN_不应传入NULL

	init = (void (*)(sf::RenderWindow*, function_Pointer)) GetProcAddress(hDLL, "init");
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

	ened = (void (*)(bool)) GetProcAddress(hDLL, "ened");
	if (ened == nullptr) { FreeLibrary(hDLL); return false; }

	init(&window, return_Function);

	return true;
}

void change_DLL()
{
	load_DLL(nullptr);
	load_DLL(next_DLL_Path);
}

void return_Function(char dll_Path[])
{
	//printf_s("return_Function Called\n");
	int lenght = 0;
	if (dll_Path == nullptr)
	{
		lenght = sizeof(loader_Path);
		next_DLL_Path = new char[lenght];
		strcpy_s(next_DLL_Path, lenght, loader_Path);
	}
	else
	{
		lenght = (int)strlen(dll_Path)+1;
		next_DLL_Path = new char[lenght];
		strcpy_s(next_DLL_Path, lenght, dll_Path);
		delete[] dll_Path;
	}
	sleep = change_DLL;
}
