#include <vector> 

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

			if (SUCCEEDED(hr)) {
				CalculateLayout();
			}
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

		if (info->board->isAlive()) {
			// Draw Grid
			int offset = 10;
			int scale = 40;
			int rows = 10;

			pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			for (int i = 0; i <= rows; i++) {
				pRenderTarget->DrawLine(D2D1::Point2F(i * scale + offset, offset), D2D1::Point2F(i * scale + offset, rows * scale + offset), pBrush, 1);
				pRenderTarget->DrawLine(D2D1::Point2F(offset, i * scale + offset), D2D1::Point2F(rows * scale + offset, i * scale + offset), pBrush, 1);
			}

			// Draw Snake Cells

			std::vector <cell> list = info->board->updateBoard().getList();
			for (cell cur : list) {
				switch (cur.second) {
				case 0:
					pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
					break;
				case 1:
					pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
					break;
				case 2:
					pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::GreenYellow));
					break;
				}
				ellipse = D2D1::Ellipse(D2D1::Point2F(offset + cur.first.first * scale - scale / 2, offset + cur.first.second * scale - scale / 2), scale * .4, scale * .4);
				pRenderTarget->FillEllipse(ellipse, pBrush);
			}
		}

		hr = pRenderTarget->EndDraw();
		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
			DiscardGraphicsResources();
		}
		EndPaint(m_hwnd, &ps);
	}
}

void SnakeWindow::CalculateLayout() {
	if (pRenderTarget != NULL)
	{
		D2D1_SIZE_F size = pRenderTarget->GetSize();
		const float x = size.width / 2;
		const float y = size.height / 2;
		const float radius = min(x, y);
		ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
	}
}

void SnakeWindow::Resize() {
	if (pRenderTarget != NULL) {
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		pRenderTarget->Resize(size);
		CalculateLayout();
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
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		OnPaint();
		return 0;

	case WM_TIMER:
		if (!info->board->doTurn()) {
			// add end game
			info->board->endGame();
			KillTimer(m_hwnd, 1);
			return 0;
		}
		OnPaint();
		return 0;

	case WM_KEYDOWN:
		switch (wParam) {
		
		// W is pressed
		case 0x57:
			info->board->ChangeDirection(up);
			break;

		// A is pressed
		case 0x41:
			info->board->ChangeDirection(left);
			break;

		// S is pressed
		case 0x53:
			info->board->ChangeDirection(down);
			break;

		// D is pressed
		case 0x44:
			info->board->ChangeDirection(right);
			break;
		}


	case WM_SIZE:
		Resize();
		return 0;
	}
	// I don't know why this line is here
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}