#include<Windows.h>
#include"Source.h"
HSV colorBuff[70];
int colorBuff_count = 0;
int size, line;

int colorBuff_selected;

LRESULT CALLBACK ContainColorProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, hdcMem;
	HANDLE       hOld;
	HBITMAP  hbmMem;
	PAINTSTRUCT ps;
	RECT rect;
	RECT rectC;
	HPEN oldPen;
	//static int size, line;
	switch (msg)
	{
	case WM_CREATE:
	{
		HWND_ContainColorProc = hwnd;
		//colorBuff_count = 0;
		/*colorBuff[0] = RGB(255, 0, 0);
		colorBuff[1] = RGB(70, 255, 0);
		colorBuff[2] = RGB(70, 255, 0);
		colorBuff[3] = RGB(70, 255, 0);
		colorBuff[4] = RGB(70, 255, 15);
		colorBuff[5] = RGB(70, 255, 0);
		colorBuff[6] = RGB(10, 255, 0);
		colorBuff[7] = RGB(70, 255, 0);
		colorBuff[8] = RGB(70, 0, 0);
		colorBuff[9] = RGB(70, 255, 0);
		colorBuff[10] = RGB(70, 20, 200);
		colorBuff[11] = RGB(70, 0, 240);*/

	}
	break;
	case WM_SIZE:
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		hdcMem = CreateCompatibleDC(hdc);
		GetClientRect(hwnd, &rectC);

		hbmMem = CreateCompatibleBitmap(hdc, rectC.right, rectC.bottom);
		hOld = SelectObject(hdcMem, hbmMem);
		{
			HBRUSH fill = CreateSolidBrush(RGB(199, 199, 199));
			FillRect(hdcMem, &rectC, fill);
			DeleteObject(fill);
		}
		
		 line = (rectC.right - 20) / size;
		 HPEN hpen = CreatePen(PS_SOLID, 1, RGB(30, 30, 30));
		 SelectObject(hdcMem, hpen);
		for (int i = 0; i < 70; i++)
		{
			rect.left = 20 + (i%line)*(size );
			rect.right = rect.left + size;
			rect.top = 20 + (i / line) * (size );
			rect.bottom = rect.top + size;
			if (colorBuff[i].e == true) {
				HBRUSH fill = CreateSolidBrush(ToRGB(colorBuff[i].H, colorBuff[i].S, 255 - colorBuff[i].V));
				SelectObject(hdcMem, fill);
				if (i == colorBuff_selected)
				{
					
				}
				else
				{
					Rectangle(hdcMem, rect.left+1, rect.top+1, rect.right-1, rect.bottom-1);

					DeleteObject(fill);
				}
				DeleteObject(fill);
			}
			else
			{
				HPEN hpen1 = CreatePen(PS_SOLID, 1, RGB(179, 179, 179));
				SelectObject(hdcMem, hpen1);
				SelectObject(hdcMem, GetStockObject(NULL_BRUSH));
				Rectangle(hdcMem, rect.left+1, rect.top+1, rect.right-1, rect.bottom-1);
				SelectObject(hdcMem, hpen);
				DeleteObject(hpen1);
			}
		
			
		}
		
		if (colorBuff_count > 0)
		{
			rect.left = 20 + (colorBuff_selected % line) * (size)+4;
			rect.right = rect.left + size - 8;
			rect.top = 20 + (colorBuff_selected / line) * (size)+4;
			rect.bottom = rect.top + size - 8;

			HBRUSH fill;
			fill = CreateSolidBrush(ToRGB(colorBuff[colorBuff_selected].H, colorBuff[colorBuff_selected].S, 255 - colorBuff[colorBuff_selected].V));
			SelectObject(hdcMem, fill);
			SelectObject(hdcMem, hpen);
			Rectangle(hdcMem, rect.left, rect.top, rect.right, rect.bottom);
			DeleteObject(fill);
			DeleteObject(hpen);
		}
		BitBlt(hdc, 0, 0, rectC.right, rectC.bottom, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, hOld);
		DeleteObject(hbmMem);
		DeleteDC(hdcMem);

		EndPaint(hwnd, &ps);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		int x, y;
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		GetClientRect(hwnd, &rectC);
		if (x >= 20 && x < rectC.right - 20 && y >= 10 && y <= (70 / line+1)*size)
		{
			
			if ((x - 20) / size + (y - 10) / size*line < colorBuff_count) {
				globalColor = ToRGB(colorBuff[(x - 20) / size + (y - 10) / size*line].H, colorBuff[(x - 20) / size + (y - 10) / size*line].S, 255 - colorBuff[(x - 20) / size + (y - 10) / size*line].V);
				startAct = true;
				colorBuff_selected = (x - 20) / size + (y - 10) / size*line;
				posColor = colorBuff[(x - 20) / size + (y - 10) / size*line].H;
				s = colorBuff[(x - 20) / size + (y - 10) / size*line].S;
				v = colorBuff[(x - 20) / size + (y - 10) / size*line].V;
				InvalidateRect(HWND_PickColorProc, NULL, TRUE);
				InvalidateRect(hwnd, NULL, TRUE);
			}
		}
	}
	case WM_LBUTTONUP:
		globalmouse = 0;
		break;
	case WM_RBUTTONUP:
		globalmouse = 0;
		break;
	case WM_MOUSEMOVE:
	{

		int x, y;
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		GetClientRect(hwnd, &rectC);
		if (x >= 20 && x < rectC.right - 20 && y >= 20 && y <= (70/line+1) *size)
		{
			if ((x - 20) / size + (y - 10) / size*line < colorBuff_count)
			SetCursor(LoadCursor(NULL, IDC_HAND));
		}
	}
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
		break;
	}
}