#pragma once

#include <windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include "BaseWindow.h"
#include "CellList.h"
#include "StateInfo.h"

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

public:
    SnakeWindow(StateInfo* info) : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL), ellipse(D2D1::Ellipse(D2D1::Point2F(), 0, 0)), info(info), rectangle(D2D1::RectF()) { };
    void Update() { OnPaint(); }

    PCWSTR ClassName() const { return L"Snake"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

