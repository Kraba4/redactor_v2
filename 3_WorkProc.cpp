#include"Source.h"

LRESULT CALLBACK WorkProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, hdcMem;
	HANDLE       hOld;
	HBITMAP  hbmMem;
	PAINTSTRUCT ps;
	RECT rect;
	RECT rectC;
	static bool ready;
	static bool focused = false;
	LPWSTR b;
	static int i, j;

	switch (msg)
	{
	case WM_CREATE:
	{
		HWND_WorkProc = hwnd;
		HWND hwndCanvas;
		WNDCLASSEX wndclassCanvas;
		wndclassCanvas.cbSize = sizeof(wndclassCanvas);
		wndclassCanvas.style = CS_NOCLOSE;
		wndclassCanvas.lpfnWndProc = CanvasProc;
		wndclassCanvas.cbClsExtra = 0;
		wndclassCanvas.cbWndExtra = 0;
		wndclassCanvas.hInstance = MainH;
		wndclassCanvas.hIcon = NULL;
		wndclassCanvas.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclassCanvas.hbrBackground = NULL;//(HBRUSH)GetStockObject(Bl_BRUSH);
		wndclassCanvas.lpszMenuName = NULL;
		wndclassCanvas.lpszClassName = L"Canvas";
		wndclassCanvas.hIconSm = NULL;
		RegisterClassEx(&wndclassCanvas);
		parent = hwnd;
		

	}

		break;
	
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		hdcMem = CreateCompatibleDC(hdc);
		GetClientRect(hwnd, &rectC);

		hbmMem = CreateCompatibleBitmap(hdc, rectC.right, rectC.bottom);


		hOld = SelectObject(hdcMem, hbmMem);
		{
			HBRUSH fill = CreateSolidBrush(RGB(30, 30, 30));
			FillRect(hdcMem, &rectC, fill);
			DeleteObject(fill);
		}
		for (int i = rectC.top; i < rectC.bottom; i += 15)
		{
			for (int j = rectC.left; j < rectC.right; j += 15)
			{
				rect.left = j;
				rect.right = j + 1;
				rect.top = i;
				rect.bottom = i + 1;
				HBRUSH fill = CreateSolidBrush((RGB(52, 52, 56)));
				FillRect(hdcMem, &rect, fill);
				DeleteObject(fill);
			}
		}


		BitBlt(hdc, 0, 0, rectC.right, rectC.bottom, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, hOld);
		DeleteObject(hbmMem);
		DeleteDC(hdcMem);

		EndPaint(hwnd, &ps);
	}break;
	//case WM_SIZE:
	//	resizeWorkProc = true;
	//	InvalidateRect(HWND_CanvasProc, NULL, TRUE);
		
	//	break;
	case WM_NCLBUTTONUP:
		globalmouse = 0;
		break;
	case WM_NCRBUTTONUP:
		globalmouse = 0;
		break;
	case WM_LBUTTONUP:
		globalmouse = 0;
		break;
	case WM_LBUTTONDOWN:
		globalmouse = 1;
		
		break;
	case WM_RBUTTONDOWN:
		globalmouse = 2;
		break;
	case WM_RBUTTONUP:
		globalmouse = 0;
		break;
	case WM_DESTROY:
	
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
		break;
	}
}