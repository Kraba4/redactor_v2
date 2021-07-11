#include"Source.h"
HSV temp;
COLORREF pickerColor = RGB(149, 149, 149);
COLORREF pickerColorhelp = RGB(149, 149, 149);
HDC mdc;
//BYTE a[10000000];
BITMAP a;
BYTE* ScreenData = 0;
LPVOID b;
HBITMAP bMem;
bool updatehwnd = true;
unsigned char *bytes;
LRESULT CALLBACK ToolProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc; 
	HANDLE       hOld;
	HBITMAP  hbmMem;
	PAINTSTRUCT ps;
	RECT rect;
	RECT rectC;
	static bool pickerActive = false;
	static bool pickerEnd = false;
	static POINT p;
//	static bool updatehwnd = true;
	static HDC hdcMem = NULL;
	static int wscreen, hscreen;
	static bool drawrgn = false;
	static int scaleSize = 2;
	static int fieldSize = 60;
	switch (msg)
	{
	case WM_CREATE:
	{

		HWND_ToolProc = hwnd;
	//	wscreen = GetDeviceCaps(NULL, HORZRES);
	//	hscreen = GetDeviceCaps("Screen", VERTRES);
		wscreen = 1920;
		hscreen = 1080;
		mdc = GetDC(GetDesktopWindow());
	}
	break;
	case WM_PAINT:
	{
		
		if (drawrgn)
		{
			int xStart = p.x  - ((fieldSize +10) / scaleSize), yStart = p.y - (fieldSize / scaleSize);
			int xEnd = p.x + ((fieldSize +10) / scaleSize), yEnd = p.y + (fieldSize / scaleSize);
				for (int y = yStart; y <= yEnd; y += 1)
			{
				for (int x = xStart; x <= xEnd; x += 1)
				{
					
					HBRUSH fill;
					
					if (x >= 0 && x <= wscreen && y >= 0 && y < hscreen ) fill = CreateSolidBrush(RGB(ScreenData[4 * ((y*wscreen) + x) + 2], ScreenData[4 * ((y*wscreen) + x) + 1], ScreenData[4 * ((y*wscreen) + x)]));
					else fill = CreateSolidBrush(RGB(0, 0, 0));
					//FillRect();
					rect.left = 220 + (x - xStart ) * scaleSize;
					rect.right = 220 + (x - xStart) * scaleSize + scaleSize;
					rect.top = 10 + (y - yStart) * scaleSize;
					rect.bottom = 10 + (y-yStart) * scaleSize + scaleSize;
					FillRect(hdcMem, &rect, fill);
					DeleteObject(fill);
					
				}
			}
				//pickerColor = RGB(ScreenData[4 * ((p.y*wscreen) + p.x) + 2], ScreenData[4 * ((p.y*wscreen) + p.x) + 1], ScreenData[4 * ((p.y*wscreen) + p.x)]);
				HBRUSH fill = CreateSolidBrush(RGB(0, 0, 0));
				rect.left = 240;
				rect.right = 340;
				rect.top = 69;
				rect.bottom = 71;
				FillRect(hdcMem, &rect, fill);

				rect.left = 289;
				rect.right = 291;
				rect.top = 20;
				rect.bottom = 120;
				FillRect(hdcMem, &rect, fill);
				rect.left = 386;
				rect.right = 395;
				rect.top = 40;
				rect.bottom = 45;
				FillRect(hdcMem, &rect, fill);
				DeleteObject(fill);
			//BitBlt(hdcMem, 0, 0, wscreen, hscreen, mdc , 0, 0, SRCCOPY);
			//StretchDIBits(hdcMem, 90,90, 60,60,)

			drawrgn = false;
		}
		hdc = BeginPaint(hwnd, &ps);
		if (updatehwnd)
		{
			//hdc = BeginPaint(hwnd, &ps);
			hdcMem = CreateCompatibleDC(hdc);
			GetClientRect(hwnd, &rectC);

			hbmMem = CreateCompatibleBitmap(hdc, rectC.right, rectC.bottom);
			hOld = SelectObject(hdcMem, hbmMem);
			{
				HBRUSH fill = CreateSolidBrush(RGB(199, 199, 199));
				FillRect(hdcMem, &rectC, fill);
				DeleteObject(fill);
			}
			rect.left = 10;
			rect.right = 50;
			rect.top = 10;
			rect.bottom = 50;
			HBRUSH fill = CreateSolidBrush(pickerColor);
			FillRect(hdcMem, &rect, fill);
			DeleteObject(fill);

			fill = CreateSolidBrush(RGB(39, 39, 39));
			rect.left = 380;
			rect.right = 385;
			rect.top = 10;
			rect.bottom = 135;
			FillRect(hdcMem, &rect, fill);
			HFONT fnt = CreateFont(8, 0, 0, 0, 400, 0, 0, 0, ANSI_CHARSET, 400, 0, 0, 0, L"Arial");
			SelectObject(hdcMem, fnt);
			//SetTextAlign(hdcMem, TA_LEFT);
			rect.left = 380;
			rect.right = 387;
			rect.top = 34 - 12;
			rect.bottom = 36 - 12;
			FillRect(hdcMem, &rect, fill);
			TextOut(hdcMem, rect.left - 10, rect.top -5, L"15", 2);
			rect.left = 380;
			rect.right = 387;
			rect.top = 59 - 12;
			rect.bottom = 61 - 12;
			FillRect(hdcMem, &rect, fill);
			TextOut(hdcMem, rect.left - 10, rect.top - 5, L"10", 2);
			rect.left = 380;
			rect.right = 387;
			rect.top = 84 - 12;
			rect.bottom = 86 - 12; FillRect(hdcMem, &rect, fill);
			TextOut(hdcMem, rect.left - 7, rect.top - 5, L"5", 1);
			rect.left = 380;
			rect.right = 387;
			rect.top = 109 - 12;
			rect.bottom = 111 - 12; FillRect(hdcMem, &rect, fill);
			TextOut(hdcMem, rect.left - 7, rect.top - 5, L"2", 1);
			rect.left = 380;
			rect.right = 387;
			rect.top = 134 - 12;
			rect.bottom = 136 - 12;
			FillRect(hdcMem, &rect, fill);
			TextOut(hdcMem, rect.left - 7, rect.top - 5, L"1", 1);
			DeleteObject(fill);
			//BitBlt()
			DeleteObject(fnt);

			BitBlt(hdc, 0, 0, rectC.right, rectC.bottom, hdcMem, 0, 0, SRCCOPY);

			//SelectObject(hdcMem, hOld);
			DeleteObject(hbmMem);
			//DeleteDC(hdcMem);
			// SetCapture(хэндл окна или формы) и ReleaseCapture().
			//GetCursorPos
			//EndPaint(hwnd, &ps);
			updatehwnd = false;
			//drawrgn = true;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else
		{
//			MessageBox(NULL, L"11", L"22", MB_OK);
		//	hdc = BeginPaint(hwnd, &ps);
			GetClientRect(hwnd, &rectC);
			BitBlt(hdc, 0, 0, rectC.right, rectC.bottom, hdcMem, 0, 0, SRCCOPY);
			
			

		}
		EndPaint(hwnd, &ps);
	}
	break;
	case WM_SIZE:
		updatehwnd = true;
		InvalidateRect(hwnd, NULL, TRUE);
		//drawrgn = true;
		break;
	case WM_LBUTTONDOWN:
	{
		int x, y;
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		if (x >= 10 && x <= 50 && y >= 10 && y <= 50)
		{
			if (!pickerActive&&!pickerEnd)
			{
				pickerActive = true;
				SetCapture(hwnd);
				SetCursor(LoadCursor(NULL, IDC_CROSS));
				HDC tempMem = CreateCompatibleDC(mdc);
				bMem = CreateCompatibleBitmap(mdc, wscreen, hscreen);
				HGDIOBJ hOld = SelectObject(tempMem, bMem);
				BitBlt(tempMem, 0, 0, wscreen, hscreen, mdc, 0, 0, SRCCOPY);
				SelectObject(tempMem, hOld);
				BITMAPINFOHEADER bmi = { 0 };
				bmi.biSize = sizeof(BITMAPINFOHEADER);
				bmi.biPlanes = 1;
				bmi.biBitCount = 32;
				bmi.biWidth = wscreen;
				bmi.biHeight = -hscreen;
				bmi.biCompression = BI_RGB;
				bmi.biSizeImage = 0;// 3 * ScreenX * ScreenY;

				if (ScreenData)
					free(ScreenData);
				ScreenData = (BYTE*)malloc(4 * wscreen * hscreen);

				GetDIBits(hdcMem, bMem, 0, hscreen, ScreenData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
				DeleteDC(tempMem);
			}
		}
		
	}
	break;
	
	case WM_MOUSEMOVE:
	{
		if (pickerActive&&!pickerEnd)
		{
			
			//HDC mdc1 = mdc;
			
			GetCursorPos(&p);
			rect.left = 10;
			rect.right = 50;
			rect.top = 10;
			rect.bottom = 50;
			
			pickerColor = RGB(ScreenData[4 * ((p.y*wscreen) + p.x) + 2], ScreenData[4 * ((p.y*wscreen) + p.x) + 1], ScreenData[4 * ((p.y*wscreen) + p.x)]);
			HBRUSH fill = CreateSolidBrush(pickerColor);
			FillRect(hdcMem, &rect, fill);
			DeleteObject(fill);
			//HBRUSH fill = CreateSolidBrush(RGB(0, 0, 0));
			
			//DeleteObject(bMem);
			
			
			
			InvalidateRect(hwnd, NULL, TRUE);
			drawrgn = true;
			
			//MessageBox(NULL, L"LUL", L"AA", MB_OK);
			if(pickerColor!=pickerColorhelp)
			{

				pickerColorhelp = pickerColor;
				temp = ToHSV(GetRValue(pickerColorhelp), GetGValue(pickerColorhelp), GetBValue(pickerColorhelp));
				
				posColor = temp.H;
				s = temp.S;
				v = 255 - temp.V;
				
				
				startAct = true;
				//InvalidateRect(hwnd, NULL, TRUE);
				//InvalidateRect(HWND_PickColorProc, NULL, TRUE);
				//pickerEnd = false;
				InvalidateRect(HWND_PickColorProc, NULL, TRUE);
			}
			
		}
	}
		break;

	case WM_LBUTTONUP:
	{
		if (pickerActive&&!pickerEnd)
		{
			ReleaseCapture();

			temp = ToHSV(GetRValue(pickerColor), GetGValue(pickerColor), GetBValue(pickerColor));

			posColor = temp.H;
			s = temp.S;
			v = 255 - temp.V;

			startAct = true;
			//pickerEnd = false;
			InvalidateRect(HWND_PickColorProc, NULL, TRUE);
			//pickerColorhelp = pickerColor;
			pickerActive = false;
			//pickerEnd = true;
			//InvalidateRect(hwnd, NULL, TRUE);
			
		}
		globalmouse = 0;
		
	}
	case WM_RBUTTONUP:
		globalmouse = 0;
		break;
	break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
		break;
	}
}