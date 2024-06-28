#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <thread>
#include <future>
#include <chrono>
#include <cmath>

#include "SnakeWindow.h"
#include "Player.h"
#include "ControlInterface.h"
#include "StateInfo.h"
#include "LaunchWindow.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	SnakeWindow win;

	if (!win.Create(L"Snake", WS_OVERLAPPEDWINDOW)) {
		return 0;
	}

	ShowWindow(win.Window(), nCmdShow);

	MSG msg = {};

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}