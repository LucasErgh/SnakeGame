#include "LaunchWindow.h"

#define NormalGame 101
#define ComputerGame 102

void LaunchWindow::OnPaint() {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(m_hwnd, &ps);

	// All painting occurs here, between BeginPaint and EndPaint.

	FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

	EndPaint(m_hwnd, &ps);
}

int LaunchWindow::LaunchSnake(int option) {
	int rows = 10, columns = 10;
	ControlInterface* model = new Player(10, 10);
	StateInfo* info = new StateInfo(rows, columns, 40, model);
	if (option == NormalGame) {
		MainGameWnd = new SnakeWindow(info);

		if (!MainGameWnd->Create(L"Snake", WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, 440, 460)) {
			return 0;
		}
		ShowWindow(MainGameWnd->Window(), SW_SHOW);
		SetTimer(MainGameWnd->Window(), 1, 400, NULL);
	}
	else if (option == ComputerGame) {
		//todo impliment computer game

	}
}


void LaunchWindow::CreateButtons() {
	SinglePlayerButton = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"Normal Snake",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		10,         // x position 
		10,         // y position 
		100,        // Button width
		100,        // Button height
		m_hwnd,     // Parent window
		(HMENU)NormalGame,       // Indicates which button was pressed
		(HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.

	ComputerButton = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"Computer",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		120,         // x position 
		10,         // y position 
		100,        // Button width
		100,        // Button height
		m_hwnd,     // Parent window
		(HMENU)ComputerGame,       // Indicates which button was pressed
		(HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.
}

LRESULT LaunchWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CREATE:
		CreateButtons();
		break;

	case WM_COMMAND:
	{
		int wmID = LOWORD(wParam);
		LaunchSnake(wmID);
		break;
	}

	case WM_PAINT:
		OnPaint();
		break;

	default:
		return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
		break;
	}
}