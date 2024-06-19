#include "LaunchWindow.h"

void LaunchWindow::OnPaint() {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(m_hwnd, &ps);

	// All painting occurs here, between BeginPaint and EndPaint.

	FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

	EndPaint(m_hwnd, &ps);
}

int LaunchWindow::LaunchSnake() {
	int rows = 10, columns = 10;
	ControlInterface* model = new Player(10, 10);
	StateInfo* info = new StateInfo(rows, columns, 40, model);
	MainGameWnd = new SnakeWindow(info);

	
	if (!MainGameWnd->Create(L"Snake", WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, 440, 460)) {
		return 0;
	}
	ShowWindow(MainGameWnd->Window(), SW_SHOW);
	SetTimer(MainGameWnd->Window(), 1, 400, NULL);
	
}

LRESULT LaunchWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		StartButton = CreateWindow(
				L"BUTTON",  // Predefined class; Unicode assumed 
				L"OK",      // Button text 
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
				10,         // x position 
				10,         // y position 
				100,        // Button width
				100,        // Button height
				m_hwnd,     // Parent window
				NULL,       // No menu.
				(HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE),
				NULL);      // Pointer not needed.
		break;
	case WM_COMMAND:
		LaunchSnake();
		break;
	case WM_PAINT:
		OnPaint();
		break;
	default:
		return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
		break;
	}
}