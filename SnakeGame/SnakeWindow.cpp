#include <vector> 
#include <string>

#include "SnakeWindow.h"

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

		if (info->model->isAlive()) {
			// Draw Grid
			int offset = 10;
			int scale = 40;
			int rows = 10;
			float cellBoarder = .8;

			pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			for (int i = 0; i <= rows; i++) {
				pRenderTarget->DrawLine(D2D1::Point2F(i * scale + offset, offset), D2D1::Point2F(i * scale + offset, rows * scale + offset), pBrush, 1);
				pRenderTarget->DrawLine(D2D1::Point2F(offset, i * scale + offset), D2D1::Point2F(rows * scale + offset, i * scale + offset), pBrush, 1);
			}

			// Draw Snake Cells

			std::vector <cell> list = info->model->GetCells();
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

void SnakeWindow::Resize() {
	if (pRenderTarget != NULL) {
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
		
		pRenderTarget->Resize(size);
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

LRESULT SnakeWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CREATE:
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory))) {
			return -1; // this will fail CreateWindowEx
		}
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
		SetTimer(this->Window(), 1, 400, NULL);
		if (!info->model->DoTurn()) {
			// add end game
			info->model->endGame();
			KillTimer(m_hwnd, 1);
			std::wstring score = L"Game Over! Your score is: " + std::to_wstring(info->model->Score());
			MessageBox(m_hwnd, (LPCWSTR)score.c_str(), (LPCWSTR)L"Game Over", MB_OK);
			DestroyWindow(this->Window());
			return 0;
		}
		OnPaint();
		return 0;

	case WM_KEYDOWN:
		switch (wParam) {
		
		// W is pressed
		case 0x57:
			info->model->ChangeDirection(up);
			break;

		// A is pressed
		case 0x41:
			info->model->ChangeDirection(left);
			break;

		// S is pressed
		case 0x53:
			info->model->ChangeDirection(down);
			break;

		// D is pressed
		case 0x44:
			info->model->ChangeDirection(right);
			break;
		}

	case WM_SIZE:
		Resize();
		return 0;
	}
	// I don't know why this line is here
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}