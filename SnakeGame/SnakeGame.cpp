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

int GetSpeed(int);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	LaunchWindow win;

	if (!win.Create(L"Launcher", WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, 120, 160)) {
		return 0;
	}

	ShowWindow(win.Window(), nCmdShow);

	MSG msg = {};

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	/*SnakeWindow win(&info);

	if (!win.Create(L"Snake", WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, 440, 460)) {
		return 0;
	}

	ShowWindow(win.Window(), nCmdShow);

	MSG msg = { };
	
	SetTimer(win.Window(), 1, GetSpeed(model->Score()) , NULL);
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (msg.message == WM_TIMER) {
			SetTimer(win.Window(), 1, GetSpeed(model->Score()), NULL);
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (!model->isAlive()) {
			std::wstring score = L"Game Over! Your score is: " + std::to_wstring(model->Score());
			MessageBox(NULL, (LPCWSTR)score.c_str(), (LPCWSTR)L"Game Over", MB_OK);
			SendMessage(win.Window(), WM_DESTROY, NULL, NULL);
		}
	}*/

	

	return 0;
}

int GetSpeed(int size) {
	const int start = 400;
	const int max = 80;
	const double decayFactor = 0.95;
	int speed;

	if (size < 4) return start;
	else 
		speed = static_cast<int>(start * std::pow(decayFactor, size-3));

	if (speed < max) return max;

	return speed;
}