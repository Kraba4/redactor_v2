#include<Windows.h>
#include"Source.h"
HWND hwndtext_value[3];
LRESULT CALLBACK TextRGBProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

		HDC hdc, hdcMem;
		HANDLE       hOld;
		HBITMAP  hbmMem;
		PAINTSTRUCT ps;
		RECT rect;
		RECT rectC;
		HBRUSH oldBrush;
		HFONT fnt;
		HFONT oldFont;
		HPEN hpen, oldPen;
	
	case WM_CREATE:
		HWND_TextRGBProc = hwnd;
		GetClientRect(hwnd, &rectC);
		hwndtext_value[0] = CreateWindowEx(0, L"Edit", (LPCTSTR)NULL,
			WS_CHILD | WS_VISIBLE, 15, 20,
			80, 15, hwnd, NULL, MainH, NULL);
		oldBrush = CreateSolidBrush(RGB(199, 199, 199));
		SendMessage(hwndtext_value[0], WM_CTLCOLOREDIT, (WPARAM)oldBrush, (LPARAM)oldBrush);
		DeleteObject(oldBrush);
		SetWindowLongPtr(hwndtext_value[0], GWL_STYLE, ES_CENTER);
		ShowWindow(hwndtext_value[0], SW_SHOWNORMAL);
		
		hwndtext_value[1] = CreateWindowEx(0, L"Edit", (LPCTSTR)NULL,
			WS_CHILD | WS_VISIBLE, 140, 20,
			80, 15, hwnd, NULL, MainH, NULL);
		
		SetWindowLongPtr(hwndtext_value[1], GWL_STYLE, ES_CENTER);
		ShowWindow(hwndtext_value[1], SW_SHOWNORMAL);
		hwndtext_value[2] = CreateWindowEx(0, L"Edit", (LPCTSTR)NULL,
			WS_CHILD | WS_VISIBLE, 265, 20,
			80, 15, hwnd, NULL, MainH, NULL);
	
		SetWindowLongPtr(hwndtext_value[2], GWL_STYLE, ES_CENTER);
		ShowWindow(hwndtext_value[2], SW_SHOWNORMAL);
	
		break;
	case WM_CTLCOLOREDIT:
		SetTextColor((HDC)wParam, RGB(70, 70, 70));
		SetBkColor((HDC)wParam, RGB(199, 199, 199));
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		hdcMem = CreateCompatibleDC(hdc);
		GetClientRect(hwnd, &rectC);

		hbmMem = CreateCompatibleBitmap(hdc, rectC.right, rectC.bottom);
		hOld = SelectObject(hdcMem, hbmMem);
		{
			HBRUSH fill= CreateSolidBrush(RGB(199, 199, 199));
			FillRect(hdcMem, &rectC, fill);
			SetBkMode(hdcMem, TRANSPARENT);
			//fill = CreateSolidBrush(RGB(19, 169, 169));
			hpen = CreatePen(PS_SOLID, 1, RGB(169, 169, 169));
			oldPen = (HPEN)SelectObject(hdcMem, hpen);
			oldBrush = (HBRUSH)SelectObject(hdcMem, fill);
			RoundRect(hdcMem, 0, 0, 110,
				rectC.bottom, 10, 10);
			RoundRect(hdcMem, 125, 0, 235,
				rectC.bottom, 10, 10);
			RoundRect(hdcMem, 250, 0, 360,
			rectC.bottom, 10, 10);
			//CreateFont()
		fnt = CreateFont(15, 0, 0, 0, 400, 0, 0, 0, ANSI_CHARSET, 400, 0, 0, 0, L"Arial");
		SetTextColor(hdcMem, RGB(100, 100, 100));
			oldFont = (HFONT)SelectObject(hdcMem, fnt);
			TextOut(hdcMem,40 , 3, L"RGB", 3);
			TextOut(hdcMem, 40 + 125, 3, L"HSV", 3);
			TextOut(hdcMem, 40 + 250, 3, L"HEX", 3);
			SelectObject(hdc, oldFont);
			SelectObject(hdc, oldBrush);
			SelectObject(hdc, oldPen);
			
			DeleteObject(hpen);
			DeleteObject(oldFont);
			DeleteObject(oldBrush);
			DeleteObject(oldPen);
			DeleteObject(fill);
			DeleteObject(fnt);
		}


		BitBlt(hdc, 0, 0, rectC.right, rectC.bottom, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, hOld);
		DeleteObject(hbmMem);
		DeleteDC(hdcMem);

		EndPaint(hwnd, &ps);
		
	}
	break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
		break;
}
}