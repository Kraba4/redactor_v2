#include"Source.h"
#include<Windows.h>
bool selectupdate = false;
LRESULT CALLBACK WorkProc_2(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, hdcMem;
	HANDLE       hOld;
	HBITMAP  hbmMem;
	PAINTSTRUCT ps;
	RECT rect;
	static RECT rectC;
	static int lenght = 0;
	static wchar_t a[22][15];
	static HWND aHwnd[22];
	static int selected = 0;

	switch (msg)
	{
	case WM_CREATE:
	{
	
		HWND_WorkProc_2 = hwnd;
		
		
	
		
	
	}
		break;
	case WM_LBUTTONDOWN:
	{
		int x, y;
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		if (lenght < 6)
		{
			if (selected != x / (rectC.right / 6) && y >= 5 && x / (rectC.right / 6) < lenght)
			{

				ShowWindow(workingHwnd, SW_HIDE);
				//update = true;
				resizeWorkProc = true;
				selected = x / (rectC.right / 6);
				workingHwnd = aHwnd[selected];
				ShowWindow(workingHwnd, SW_NORMAL);
				InvalidateRect(hwnd, NULL, TRUE);
				selectupdate = true;
			}
		}
		else
		{
			if (selected != x / (rectC.right / lenght) && y >= 5 && x / (rectC.right / lenght) < lenght)
			{

				ShowWindow(workingHwnd, SW_HIDE);
				//update = true;
				resizeWorkProc = true;
				selected = x / (rectC.right / lenght);
				workingHwnd = aHwnd[selected];
				ShowWindow(workingHwnd, SW_NORMAL);
				InvalidateRect(hwnd, NULL, TRUE);
				selectupdate = true;
			}
		}
	}
		break;
	case WM_PAINT:
	{
		if (newCreate)
		{
			newCreate = false;
			ShowWindow(workingHwnd, SW_HIDE);
			workingHwnd = newHwnd;
			aHwnd[lenght] = newHwnd;
			ShowWindow(workingHwnd, SW_NORMAL);
			wchar_t str[15];
			_itow_s(lenght, str, 10);
			selected = lenght;
			int j = 0;
			while (str[j] != NULL)
			{
				a[lenght][j] = str[j];
				j++;
			}
			
			lenght++;
			
			
		}
		hdc = BeginPaint(hwnd, &ps);
		hdcMem = CreateCompatibleDC(hdc);
		GetClientRect(hwnd, &rectC);

		hbmMem = CreateCompatibleBitmap(hdc, rectC.right, rectC.bottom);
		hOld = SelectObject(hdcMem, hbmMem);
		{
			
		HBRUSH fill = CreateSolidBrush(RGB(169, 169, 169));
			FillRect(hdcMem, &rectC, fill);
			DeleteObject(fill);
		}
		rectC.left = 5;
	
		rect.top = 3;
		rect.bottom = rectC.bottom;
		HBRUSH fill = CreateSolidBrush(RGB(149, 149, 149));
		HBRUSH fillSelected = CreateSolidBrush(RGB(199, 199, 199));
		
		SelectObject(hdcMem, fill);
		SelectObject(hdcMem, GetStockObject( NULL_PEN));
		if(lenght<6)
		for (int i = 0; i < lenght; i++)
		{
			if (i == selected)
			{
				SelectObject(hdcMem, fillSelected);
				SetTextColor(hdcMem, RGB(0, 0, 0));
			}
			rect.left = rectC.left + (rectC.right-15)/6*i;
			rect.right = (rectC.right - 15) /6 + (rectC.right-15)/6*i ;
			SetBkMode(hdcMem, TRANSPARENT);
	
			
			RoundRect(hdcMem, rect.left, rect.top, rect.right, rect.bottom+5, 10, 10);
			TextOut(hdcMem, rect.left+10, rectC.top + 5, a[i], 2);
			SetTextColor(hdcMem, RGB(0,0,0));
			SelectObject(hdcMem, fill);
		}else
			for (int i = 0; i < lenght; i++)
			{
				if (i == selected)
				{
					SelectObject(hdcMem, fillSelected);
					SetTextColor(hdcMem, RGB(0, 0, 0));
				}
				rect.left = rectC.left + (rectC.right - 15) / lenght * i;
				rect.right = rectC.right / 6 + (rectC.right - 15) / lenght * i;
				SetBkMode(hdcMem, TRANSPARENT);
				

				RoundRect(hdcMem, rect.left, rect.top, rect.right, rect.bottom + 5, 10, 10);
				TextOut(hdcMem, rect.left + 10, rectC.top + 5, a[i], 2);
				SelectObject(hdcMem, fill);
				SetTextColor(hdcMem, RGB(0, 0, 0));
			}
		DeleteObject(fill);
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