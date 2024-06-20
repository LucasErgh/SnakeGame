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
	int rows = 20, columns = 20, cellScale = 40;
	ControlInterface* model; 
	StateInfo* info;
	if (option == NormalGame) {
		model = new Player(rows, columns);
		info = new StateInfo(rows, columns, cellScale, model);

		MainGameWnd = new SnakeWindow(info);

		if (!MainGameWnd->Create(L"Snake", WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, cellScale * rows + cellScale, cellScale * rows + cellScale + 20)) {
			return 0;
		}
		ShowWindow(MainGameWnd->Window(), SW_SHOW);
		SetTimer(MainGameWnd->Window(), 1, 400, NULL);
	}
	else if (option == ComputerGame) {
		model = new ComputerPlayer(rows, columns);
		info = new StateInfo(rows, columns, cellScale, model);

		MainGameWnd = new SnakeWindow(info);

		if (!MainGameWnd->Create(L"Snake", WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, cellScale * rows + cellScale, cellScale * rows + cellScale + 20)) {
			return 0;
		}
		ShowWindow(MainGameWnd->Window(), SW_SHOW);
		SetTimer(MainGameWnd->Window(), 1, 400, NULL);
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