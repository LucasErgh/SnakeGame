#include "SnakeWindow.h"

#define IDC_START 100
#define IDC_OPTIONS 101
#define IDC_RADIO1 102
#define IDC_RADIO2 103
#define IDC_RADIO3 104
 
SnakeWindow::SnakeWindow() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL), Start(NULL), button1(NULL), button2(NULL), button3(NULL)
{

}

HRESULT SnakeWindow::CreateGraphicsResources() {
	HRESULT hr = S_OK;

	if (pRenderTarget == NULL) {
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		hr = pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&pRenderTarget);

		if (SUCCEEDED(hr))
		{
			const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
			hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
		}
	}
	return hr;
}

void SnakeWindow::DiscardGraphicsResources() {
	SafeRelease(&pRenderTarget);
	SafeRelease(&pBrush);
}

void SnakeWindow::OnPaint() {
	HRESULT hr = CreateGraphicsResources();
	if (SUCCEEDED(hr)) {
		PAINTSTRUCT ps;
		BeginPaint(m_hwnd, &ps);

		pRenderTarget->BeginDraw();

		pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		if (info && (info->gamemode == IDC_RADIO1 || info->gamemode == IDC_RADIO2) && info->model1 && info->model1->isAlive()) {
			// Draw Grid
			int offset = 10;
			int scale = info->scale;
			int rows = info->rows;
			int columns = info->columns;
			float cellBoarder = .8;

			pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			for (int i = 0; i <= rows; ++i) {
				pRenderTarget->DrawLine(D2D1::Point2F(offset, i * scale + offset), D2D1::Point2F(columns * scale + offset, i * scale + offset), pBrush, 1);
			}
			for (int i = 0; i <= columns; ++i) {
				pRenderTarget->DrawLine(D2D1::Point2F(i * scale + offset, offset), D2D1::Point2F(i * scale + offset, rows * scale + offset), pBrush, 1);
			}

			// Draw Snake Cells

			std::vector <cell> list = info->model1->GetCells();
			cell* last = NULL;
			for (cell &cur : list) {
				int left = offset + cur.first.first * scale - scale * cellBoarder, 
					top = offset + cur.first.second * scale - scale * cellBoarder,
					right = offset + cur.first.first * scale - scale * (1 - cellBoarder),
					bottem = offset + cur.first.second * scale - scale * (1 - cellBoarder);
				switch (cur.second) {
				case 0:
					pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
					ellipse = D2D1::Ellipse(D2D1::Point2F(offset + cur.first.first * scale - scale / 2, offset + cur.first.second * scale - scale / 2), scale * .4, scale * .4);
					pRenderTarget->FillEllipse(ellipse, pBrush);
					break;
				case 1:
					last = &cur;
					pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
					rectangle = D2D1::RectF(left, top, right, bottem);
					pRenderTarget->FillRectangle(rectangle, pBrush);
					break;
				case 2:
					pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::GreenYellow));
					if (last != NULL && cur.first.first == last->first.first) { // This means they are in the same column
						if (cur.first.second > last->first.second) { // this means the last was above the cur
							top -= scale * ((1 - cellBoarder) * 2);
						}
						else { 
							bottem += scale * ((1 - cellBoarder) * 2);
						}
					}
					else if (last != NULL){ // Otherwise they are in the same row
						if (cur.first.first < last->first.first) { // this means last is right of cur
							right += scale * ((1 - cellBoarder) * 2);
						}
						else {
							left -= scale * ((1 - cellBoarder) * 2);
						}

					}
					last = &cur;
					rectangle = D2D1::RectF(left, top, right, bottem);
					pRenderTarget->FillRectangle(rectangle, pBrush);
					break;
				}
				
			}
		}

		hr = pRenderTarget->EndDraw();
		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
			DiscardGraphicsResources();
		}
		EndPaint(m_hwnd, &ps);
	}
}

int GetTime(int size) {
	const int start = 100;
	const int max = 80;
	const double decayFactor = 0.95;
	int speed;

	if (size < 4) return start;
	else
		speed = static_cast<int>(start * std::pow(decayFactor, size - 3));

	if (speed < max) return max;

	return speed;
}

void SnakeWindow::Resize() {
	if (pRenderTarget != NULL) {
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
		
		pRenderTarget->Resize(size);
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

void SnakeWindow::EnterGame() {
	if(!info)
		info = new StateInfo(12, 12, 40, NULL);
	
	if (SendMessage(GetDlgItem(m_hwnd, IDC_RADIO1), BM_GETCHECK, 0, 0) == BST_CHECKED)
		info->gamemode = IDC_RADIO1;
	if (SendMessage(GetDlgItem(m_hwnd, IDC_RADIO2), BM_GETCHECK, 0, 0) == BST_CHECKED)
		info->gamemode = IDC_RADIO2;
	if (SendMessage(GetDlgItem(m_hwnd, IDC_RADIO3), BM_GETCHECK, 0, 0) == BST_CHECKED)
		info->gamemode = IDC_RADIO3;
	
	HideButtons();
	
	switch (info->gamemode)
	{
	case IDC_RADIO1: // This is Classic
		info->model1 = new Player(info->columns, info->rows);
		SetTimer(this->Window(), (int)IDC_RADIO1, 500, NULL);
		break;
	case IDC_RADIO2: // This is Computer Solo
		info->model1 = new ComputerPlayer(info->columns, info->rows);
		SetTimer(this->Window(), (int)IDC_RADIO1, 500, NULL);
		break;
	case IDC_RADIO3: // This is Player v Computer
		info->model1 = new Player(info->columns, info->rows);
		info->model2 = new ComputerPlayer(info->columns, info->rows);
		SetTimer(this->Window(), (int)IDC_RADIO1, 500, NULL);
		SetTimer(this->Window(), (int)IDC_RADIO2, 500, NULL);
		break;
	default:
		break;
	}
}

void SnakeWindow::ExitGame() {
	ControlInterface* model;
	if (info->gamemode != IDC_RADIO3) {
		model = info->model1;
	}
	else throw(1);

	std::wstring score = L"Game Over! Your score is: " + std::to_wstring(model->Score());
	MessageBox(m_hwnd, (LPCWSTR)score.c_str(), (LPCWSTR)L"Game Over", MB_OK);

	if (info->model1) {
		info->model1->endGame();
		delete info->model1;
	}
	if (info->model2) {
		info->model2->endGame();
		delete info->model2;
	}
	
	ShowButtons();
}

void SnakeWindow::ShowButtons() {
	// Create Start Button
	if (Start) {
		ShowWindow(Start, SW_SHOW);
	}
	else {
		Start = CreateWindow(
			L"BUTTON", // Predefined class
			L"Start", // Button text
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
			10, 10, // x and y position
			200, 200, // width and height
			m_hwnd, // Parent Window
			(HMENU)IDC_START, // HMENU code sent to message handler
			(HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE),
			NULL);
	}
	// Create Group Box
	if (OptionsBox) {
		ShowWindow(OptionsBox, SW_SHOW);
	}
	else {
		OptionsBox = CreateWindow(
			L"BUTTON",
			L"Gamemode",
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			220, 10,
			220, 120,
			m_hwnd,
			(HMENU)IDC_OPTIONS,
			(HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE),
			NULL);
		if (!OptionsBox) {
			throw(1);
		}
	}
	// Create Radio Buttons
	if (button1) {
		ShowWindow(button1, SW_SHOW);
	}
	else {
		button1 = CreateWindow(L"BUTTON", L"Classic", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 230, 40, 200, 20,
			m_hwnd, (HMENU)IDC_RADIO1, GetModuleHandle(NULL), NULL);

		SendMessage(button1, BM_SETCHECK, BST_CHECKED, 0);
	}

	if (button2) {
		ShowWindow(button2, SW_SHOW);
	}
	else {
		button2 = CreateWindow(L"BUTTON", L"Computer", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 230, 70, 200, 20,
			m_hwnd, (HMENU)IDC_RADIO2, GetModuleHandle(NULL), NULL);
	}

	if (button3) {
		ShowWindow(button3, SW_SHOW);
	}
	else {
		button3 = CreateWindow(L"BUTTON", L"Player vs Computer", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 230, 100, 200, 20,
			m_hwnd, (HMENU)IDC_RADIO3, GetModuleHandle(NULL), NULL);
	}

	if (!button1 || !button2 || !button3) {
		throw(1);
	}
}

void SnakeWindow::HideButtons() {
	ShowWindow(Start, SW_HIDE);
	ShowWindow(OptionsBox, SW_HIDE);
	ShowWindow(button1, SW_HIDE);
	ShowWindow(button2, SW_HIDE);
	ShowWindow(button3, SW_HIDE);
}

void SnakeWindow::TimerUp(int num) {
	ControlInterface* model;

	if (num == IDC_RADIO1) model = info->model1;
	else if (num == IDC_RADIO1) model = info->model2;
	else throw(1);

	if (model->isAlive()) {
		SetTimer(m_hwnd, num, GetTime(model->Score()), NULL);
		model->DoTurn();
	}
	else {
		KillTimer(m_hwnd, num);
		ExitGame();
	}
	OnPaint();
}

LRESULT SnakeWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) 
	{
	case WM_CREATE:
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory))) {
			return -1; // this will fail CreateWindowEx
		}
		ShowButtons();
		return 0;

	case WM_DESTROY:
		DiscardGraphicsResources();
		SafeRelease(&pFactory);
		delete info;
		
		return 0;

	case WM_PAINT:
		OnPaint();
		return 0;

	case WM_TIMER:
		TimerUp((int)wParam);
		return 0;

	case WM_KEYDOWN:
		switch (wParam) {
		
		// W is pressed
		case 0x57:
			info->model1->ChangeDirection(up);
			break;

		// A is pressed
		case 0x41:
			info->model1->ChangeDirection(left);
			break;

		// S is pressed
		case 0x53:
			info->model1->ChangeDirection(down);
			break;

		// D is pressed
		case 0x44:
			info->model1->ChangeDirection(right);
			break;
		}

	case WM_SIZE:
		Resize();
		return 0;
		break;

	case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case IDC_START:
				EnterGame();
				break;
			case IDC_RADIO1:
				break;
			case IDC_RADIO2:
				break;
			case IDC_RADIO3:
				break;
			default:
				break;
			}
		}
		break;
	}
	// I don't know why this line is here
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}