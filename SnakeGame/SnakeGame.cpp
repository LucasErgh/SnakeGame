#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

#include "SnakeWindow.h"
#include "Board.h"

// Window Procedure declaration
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	int rows = 10, columns = 10;
	Board board(columns, rows);
	StateInfo info(rows, columns, 40, &board);

	SnakeWindow win(&info);

	if (!win.Create(L"Snake", WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, 440, 460)) {
		return 0;
	}

	ShowWindow(win.Window(), nCmdShow);

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	/*// Register the window class
	const wchar_t CLASS_NAME[] = L"Sample Window Class"; // Name the window class 

	WNDCLASS wc = { };	// Define the structure

	wc.lpfnWndProc = WindowProc;	// Set Window Procedure
	wc.hInstance = hInstance;		// Sets handle to the application instance
	wc.lpszClassName = CLASS_NAME;	// Identifies the window class

	RegisterClass(&wc);	// This function registers the window class with the operating system

	// Create Window Reference https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
	HWND hwnd = CreateWindowEx(
		0,								// Optional window styles
		CLASS_NAME,						// Window class
		L"Learn to Program Windows",	// Window text
		WS_OVERLAPPEDWINDOW,			// Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		
		NULL,		// Parent window
		NULL,		// Menu
		hInstance,	// Instance handle
		NULL		// Additional application data
	);
	
	if (hwnd == NULL) {
		return 0;
	}

	ShowWindow(hwnd, nCmdShow); 
	// remember nCmdShow is a flag that indicates if the window is minimized, maximized or shown normally


	MSG msg = { };

	while (GetMessage(&msg, NULL, 0, 0) > 0) // GetMessage returns 0 if PostQuitMessage was called
	{
		TranslateMessage(&msg);	// Translates message to keypress
		DispatchMessage(&msg);	// Calls corrosponding Window Procedure
	}*/

	return 0;
}

/*
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// All painting occurs here, between BeginPaint and EndPaint.

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
*/