#pragma once

#include <cmath>
#include <windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include "BaseWindow.h"
#include "DataTypes.h"
#include "StateInfo.h"
#include "Player.h"
#include "ComputerPlayer.h"

#include <vector> 
#include <string>

class SnakeWindow : public BaseWindow<SnakeWindow>
{
    StateInfo* info;

    ID2D1Factory*           pFactory;
    ID2D1HwndRenderTarget*  pRenderTarget;
    ID2D1SolidColorBrush*   pBrush;
    D2D1_ELLIPSE            ellipse;
    D2D1_RECT_F             rectangle;

    HRESULT CreateGraphicsResources();
    void    DiscardGraphicsResources();
    void    OnPaint();
    void    Resize();

    void EnterGame();
    void ExitGame();
    void TimerUp(int num);

    void ShowButtons();
    void HideButtons();
    HWND Start;
    HWND OptionsBox;
    HWND button1;
    HWND button2;
    HWND button3;

public:
    SnakeWindow();
    void Update() { OnPaint(); }

    PCWSTR ClassName() const { return L"Snake"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

