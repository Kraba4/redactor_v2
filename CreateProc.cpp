#include"Source.h"
LRESULT CALLBACK CreateProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, hdcMem;
	HANDLE       hOld;
	HBITMAP  hbmMem;
	PAINTSTRUCT ps;
	RECT rect;
	RECT rectC;
	static HWND hwndInput, hwndInput1;
	static HFONT hfont;
	static bool start = false;
	switch (msg)
	{
	case WM_CREATE:
	{
		HWND_CreateProc = hwnd;
		hwndInput = CreateWindowEx(0, L"Edit", (LPCTSTR)NULL,
			WS_CHILD | WS_VISIBLE, 100, 20,
			50, 20, hwnd, NULL, MainH, NULL);
		 hfont = CreateFont(20, 0, 0, 0, 0, 
			false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE, L"Arial");

			SendMessage(hwndInput, WM_SETFONT, (WPARAM)hfont, (LPARAM)hfont);
		SetWindowLongPtr(hwndInput, GWL_STYLE, ES_CENTER);
		ShowWindow(hwndInput, SW_SHOWNORMAL);

		hwndInput1 = CreateWindowEx(0, L"Edit", (LPCTSTR)NULL,
			WS_CHILD | WS_VISIBLE, 100, 60,
			50, 20, hwnd, NULL, MainH, NULL);

		SendMessage(hwndInput1, WM_SETFONT, (WPARAM)hfont, (LPARAM)hfont);
		SetWindowLongPtr(hwndInput1, GWL_STYLE, ES_CENTER);
		ShowWindow(hwndInput1, SW_SHOWNORMAL);
	
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
			HBRUSH fill = CreateSolidBrush(RGB(230, 230, 230));
			FillRect(hdcMem, &rectC, fill);
			DeleteObject(fill);
		}
		SelectObject(hdcMem, hfont);
		SetBkColor(hdcMem, RGB(230, 230, 230));
		
		TextOut(hdcMem, 30, 20, L"Øèğèíà", 6);
		TextOut(hdcMem, 30, 60, L"Âûñîòà", 6);
		HFONT hfont1 = CreateFont(24, 0, 0, 0, 0,
			false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE, L"Arial");
		SetBkColor(hdcMem, RGB(255, 255, 255));
		SelectObject(hdcMem, hfont1);
		HBRUSH fill = CreateSolidBrush(RGB(255, 255, 255));
		HPEN pen;
        if(!start) pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		else pen = CreatePen(PS_SOLID, 1, RGB(199, 199, 199));

		rect.left = 115;
		rect.right = 207;
		rect.top = 177;
		rect.bottom = 207;
		SelectObject(hdcMem, pen);
		SelectObject(hdcMem, fill);
		//FillRect(hdcMem, &rect, fill);
		RoundRect(hdcMem, rect.left, rect.top, rect.right, rect.bottom, 10, 10);
		DeleteObject(fill);
		TextOut(hdcMem, 120, 180, L"Ñîçäàòü", 7);
		DeleteObject(hfont1);
		BitBlt(hdc, 0, 0, rectC.right, rectC.bottom, hdcMem, 0, 0, SRCCOPY);
		
		SelectObject(hdcMem, hOld);
		DeleteObject(hbmMem);
		DeleteDC(hdcMem);

		EndPaint(hwnd, &ps);
	}break;
	case WM_LBUTTONDOWN:
	{
		int x, y;
		x = LOWORD(lParam);
		y = HIWORD(lParam);
	

		if (x >= 115 && x <= 207 && y >= 177 && y <= 207)
		{
			//MessageBox(NULL, L"AA", L"LOL", MB_OK);
			start = true;
			wchar_t str[6]; wchar_t str1[6];
			SendMessage(hwndInput, WM_GETTEXT, (LPARAM)str, (LPARAM)str);
			int temph=0, tempw=0;
			int i = 0;
			while (str[i] != NULL )
			{
				if (str[i] >= '0' && str[i] <= '9')
				{
					tempw *= 10;
					tempw += str[i] - 48;
					i++;
				}
				else {
					tempw = -1; break;
				}
				
			}


			SendMessage(hwndInput1, WM_GETTEXT, (LPARAM)str1, (LPARAM)str1);
		
			 i = 0;
			while (str1[i] != NULL)
			{
				if (str1[i] >= '0' && str1[i] <= '9')
				{
					temph *= 10;
					temph += str1[i] - 48;
					i++;
				}
				else {
					temph = -1; break;
				}
			}
			HWND hwndCanvas;
			
			hwndCanvas = CreateWindowEx(0, L"Canvas", (LPCTSTR)NULL,
				WS_CHILD , 0, 0,
				tempw, temph, HWND_WorkProc, NULL, MainH, NULL);
		 
			//SendMessage(HWND_WorkProc_2, WM_COPYDATA, (WPARAM)hwndCanvas, NULL);
			resizeWorkProc = true;
			update = true;
			Canvas::AddCan(hwndCanvas, tempw, temph,false);
			newHwnd = hwndCanvas;
			SetFocus(hwndCanvas);
		//	ShowWindow(hwndCanvas, SW_NORMAL);
		//	UpdateWindow(hwndCanvas);
		
			
			InvalidateRect(HWND_WorkProc_2, NULL, TRUE);
		}
		else start = false;
		InvalidateRect(hwnd, NULL, TRUE);
	}
		break;
	case WM_MOUSEMOVE:
		int x, y;
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		if (start)
		{
			if (x >= 115 && x <= 207 && y >= 177 && y <= 207)
			{
				SetCursor(LoadCursor(NULL, IDC_HAND));
			}
			else { start = false; InvalidateRect(hwnd, NULL, TRUE); }

		}else
		if (x >= 115 && x <= 207 && y >= 177 && y <= 207)
		{
			//MessageBox(NULL, L"AA", L"LOL", MB_OK);
			start = true;
			SetCursor(LoadCursor(NULL, IDC_HAND));
			InvalidateRect(hwnd, NULL, TRUE);

		}

		
		break;
	case WM_DESTROY:
		//ShowWindow(hwnd, SW_HIDE);
		ActiveCreate = false;
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
		break;
	}
}