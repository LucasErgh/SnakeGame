#pragma once

#include <windows.h> 

#include "BaseWindow.h"
#include "SnakeWindow.h"
#include "ControlInterface.h"
#include "Player.h"

class LaunchWindow : public BaseWindow<LaunchWindow>
{
public:
	void OnPaint();
	int LaunchSnake();

	SnakeWindow *MainGameWnd;
	HWND CompetetorWnd;
	HWND StartButton;


	LaunchWindow() : MainGameWnd(NULL), CompetetorWnd(NULL), StartButton(NULL) { };

	PCWSTR ClassName() const { return L"Snake"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);


};

