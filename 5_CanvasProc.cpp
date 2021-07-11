#include"Source.h"
#include<cmath>
#include<TCHAR.H>
#define _CRT_SECURE_NO_WARNINGS

HWND parent = NULL;
HINSTANCE MainH = NULL;
HWND doc = NULL;
RGBE selected_rgbe = { 255,255,255,true };
int globalmouse = 0;
bool update = true;
bool sizeupdate = false;
void TrackMouse(HWND hwnd)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_HOVER | TME_LEAVE; //Type of events to track & trigger.
	tme.dwHoverTime = 1; //How long the mouse has to be in the window to trigger a hover event.
	tme.hwndTrack = hwnd;
	TrackMouseEvent(&tme);
}
LRESULT CALLBACK CanvasProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	static RECT rectC;
	static RECT rectL;
	static RECT firstRect;
	PAINTSTRUCT ps;
	HDC hdc;
	HANDLE       hOld;
	HBITMAP  hbmMem;
	static bool vova = false;
	static int sizePen = 1;
	static POINT posxy;
	static bool focused = false;
	static bool hold;
	static bool erase;
	static int resizeX, resizeY;
	static bool Tracking = false;
	static int lastx = -1, lasty = 1;
	static bool act = false;
	static double tg;
	static int sx, sy;
	static int border = 700;
	static int borderX = 700;
	static int borderY = 700;
	static int num = 0;
	//	static HDC activehdc;
	static bool updatesize = true;
	static bool updatesize2 = false;
	static int vovasize = 1;
	switch (msg)
	{
	case WM_CREATE:
	{

		HWND_CanvasProc = hwnd;
		erase = false;
		hold = false;
		can = Canvas::Search(hwnd);
		//HDC hdc = GetDC(hwnd);
		//activehdc = CreateCompatibleDC(hdc);
		//HBITMAP hbm;
		//hbm = CreateCompatibleBitmap(hdc, rectC.right, rectC.bottom);
		//SelectObject(activehdc, hbm);
	}
	break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		if (selectupdate)
		{
			can = Canvas::Search(hwnd);
			updatesize = true;
			GetClientRect(HWND_WorkProc, &rect);
			int centerx = rect.right / 2;
			int centery = rect.bottom / 2;
			posxy.x = centerx - can->sizeX / 2 * can->size - borderX;
			posxy.y = centery - can->sizeY / 2 * can->size - borderY;

			resizeWorkProc = false;
			InvalidateRect(hwnd, NULL, TRUE);
			MoveWindow(hwnd, posxy.x, posxy.y, can->sizeX * can->size + 2 * borderX, can->sizeY * can->size + 2 * borderY, TRUE);

			GetClientRect(hwnd, &rectC);

			selectupdate = false;

		}
		if (update)
		{


			bool resize = false;
			if(!openUpdate)
			if (can != Canvas::Search(hwnd))
			{
				resize = true;
				updatesize = true;
			}

			can = Canvas::Search(hwnd);
			/*if (can->init == false)
			{
				for (int i = 0; i < can->sizeY; i++)
				{
					for (int j = 0; j < can->sizeX; j++)
					{
						can->SetPos(RGB(255, 255, 255), j, i);
					}
				}

			}*/
			/*if (resize)
			{
				firstRect.left = 0;
				firstRect.right = can->sizeX;
				firstRect.top = 0;
				firstRect.bottom = can->sizeY;
			}*/
			if (resizeWorkProc)
			{
				GetClientRect(HWND_WorkProc, &rect);
				int centerx = rect.right / 2;
				int centery = rect.bottom / 2;
				posxy.x = centerx - can->sizeX / 2 * can->size - borderX;
				posxy.y = centery - can->sizeY / 2 * can->size - borderY;

				resizeWorkProc = false;
				MoveWindow(hwnd, posxy.x, posxy.y, can->sizeX * can->size + 2 * borderX, can->sizeY * can->size + 2 * borderY, TRUE);
			}

			GetClientRect(hwnd, &rectC);
			if (openUpdate)
			{
				HDC temp = CreateCompatibleDC(hdc);
			//	can->hdcMem = CreateCompatibleDC(hdc);
				hbmMem = CreateCompatibleBitmap(hdc, rectC.right, rectC.bottom);
				hOld = SelectObject(temp, hbmMem);

				DrawOld(temp, RGB(199, 199, 199), rectC.left, rectC.right, rectC.top, rectC.bottom);
				for (int i = 5; i > 0; i--)
				{

					DrawOld(temp, RGB(199 - ((5 - i) * 10), 199 - ((5 - i) * 10), 199 - ((5 - i) * 10)), borderX - 2,
						rectC.right - borderX + 3 + i, borderY - 2, rectC.bottom - borderY + 3 + i);
				}
				DrawOld(temp, RGB(200, 255, 255), borderX, rectC.right - borderX, borderY, rectC.bottom - borderY);
				/*if (can->init == false)
				{
					DrawOld(can->hdcMem, RGB(255, 255, 255), rectC.left, rectC.right, rectC.top, rectC.bottom);
					can->init = true;
				}
				else/
					/*for (int i = 0; i < can->sizeY; i++)
					{
						for (int j = 0; j < can->sizeX; j++)
						{
							if (can->GetPos(j, i).e == true)
							{

								DrawOld(can->hdcMem, can->GetPos(j, i), j* can->size + border, j* can->size + can->size + border, i* can->size + border,
									i* can->size + can->size + border);
							}

						}
					}*/
			
				BitBlt(temp, border, border, can->sizeX, can->sizeY, can->hdcMem, 0, 0, SRCCOPY);
				can->hdcMem = CreateCompatibleDC(hdc);
				hbmMem = CreateCompatibleBitmap(hdc, rectC.right, rectC.bottom);
				hOld = SelectObject(can->hdcMem, hbmMem);

				BitBlt(can->hdcMem, 0, 0, rectC.right, rectC.bottom, temp, 0, 0, SRCCOPY);
				BitBlt(hdc, 0, 0, rectC.right, rectC.bottom, can->hdcMem, 0, 0, SRCCOPY);
				openUpdate = false;
				//selectupdate = false;
			}
			else {
				can->hdcMem = CreateCompatibleDC(hdc);
				hbmMem = CreateCompatibleBitmap(hdc, rectC.right, rectC.bottom);
				hOld = SelectObject(can->hdcMem, hbmMem);

				DrawOld(can->hdcMem, RGB(199, 199, 199), rectC.left, rectC.right, rectC.top, rectC.bottom);
				for (int i = 5; i > 0; i--)
				{

					DrawOld(can->hdcMem, RGB(199 - ((5 - i) * 10), 199 - ((5 - i) * 10), 199 - ((5 - i) * 10)), borderX - 2,
						rectC.right - borderX + 3 + i, borderY - 2, rectC.bottom - borderY + 3 + i);
				}
				DrawOld(can->hdcMem, RGB(255, 255, 255), borderX, rectC.right - borderX, borderY, rectC.bottom - borderY);
				/*if (can->init == false)
				{
					DrawOld(can->hdcMem, RGB(255, 255, 255), rectC.left, rectC.right, rectC.top, rectC.bottom);
					can->init = true;
				}
				else/
					/*for (int i = 0; i < can->sizeY; i++)
					{
						for (int j = 0; j < can->sizeX; j++)
						{
							if (can->GetPos(j, i).e == true)
							{

								DrawOld(can->hdcMem, can->GetPos(j, i), j* can->size + border, j* can->size + can->size + border, i* can->size + border,
									i* can->size + can->size + border);
							}

						}
					}*/

				BitBlt(hdc, 0, 0, rectC.right, rectC.bottom, can->hdcMem, 0, 0, SRCCOPY);
			}
			DeleteObject(hbmMem);

			update = false;

		}
		else
		{
			//int truSize = can->sizeX + border * 2;
			GetClientRect(hwnd, &rectC);
			SetMapMode(can->hdcMem, MM_ANISOTROPIC);
			/*	SetWindowExtEx(can->hdcMem, rectC.right, rectC.bottom, NULL);
				SetViewportExtEx(can->hdcMem, rectC.right, rectC.bottom, NULL);
				SetViewportOrgEx(can->hdcMem, 0, 0, NULL);

				SetWindowExtEx(can->hdcMem, rectC.right* can->size, rectC.bottom*can->size, NULL);
				SetViewportExtEx(can->hdcMem, rectC.right, rectC.bottom, NULL);*/
				//SetViewportOrgEx(can->hdcMem, border*(vovasize-1), border* (vovasize - 1), NULL);

				/*BitBlt(hdc, 0, 0, rectC.right, rectC.bottom, can->hdcMem, (rectC.right/2)* (can->size - 1)
					, (rectC.bottom/2 )* (can->size - 1), SRCCOPY);*/
			SetMapMode(hdc, MM_ANISOTROPIC);
			SetWindowExtEx(hdc, rectC.right, rectC.bottom, NULL);
			SetViewportExtEx(hdc, rectC.right * can->size, rectC.bottom * can->size, NULL);
			if (can->size > 1)	SetViewportOrgEx(hdc, -(rectC.right / 2) * (can->size - 1), -(rectC.bottom / 2) * (can->size - 1), NULL);
			BitBlt(hdc, 0, 0, rectC.right, rectC.bottom, can->hdcMem, 0, 0, SRCCOPY);
			/*if (can->size == 1)BitBlt(hdc, 0, 0, rectC.right, rectC.bottom, can->hdcMem, 0, 0, SRCCOPY); else
				BitBlt(hdc, border, border, rectC.right - 2 * border, rectC.bottom - 2 * border, can->hdcMem, border, border, SRCCOPY);*/
			//SetViewportOrgEx(can->hdcMem, (rectC.right / 2)* (can->size - 1), (rectC.bottom / 2)* (can->size - 1), NULL);
			//border = rectC
			borderX = (rectC.right - can->sizeX * can->size) / 2;
			borderY = (rectC.bottom - can->sizeY * can->size) / 2;
			//SetViewportOrgEx(hdc, 20, 40, NULL);
		//	SetViewportOrgEx(hdc, rectL.right/2 -border, rectL.bottom/2 -border , NULL);

			/*SetMapMode(can->hdcMem, MM_ISOTROPIC);
			SetViewportExtEx(can->hdcMem, rectC.right / vovasize, rectC.bottom / vovasize, NULL);
			SetWindowExtEx(can->hdcMem, rectC.right, rectC.bottom, NULL);
			BitBlt(hdc, 0, 0, rectC.right, rectC.bottom, can->hdcMem, 0, 0, SRCCOPY);


			SetMapMode(hdc, MM_ISOTROPIC);
			SetViewportExtEx(hdc, rectC.right / vovasize, rectC.bottom / vovasize, NULL);
			SetWindowExtEx(hdc, rectC.right, rectC.bottom, NULL);*/

			//BitBlt(hdc, 0, 0, rectC.right, rectC.bottom, activehdc, 0, 0, SRCCOPY);


		}
		if (updatesize)
		{
			GetClientRect(hwnd, &rectC);

			//	SetMapMode(hdc, MM_ISOTROPIC);
			//	SetViewportExtEx(hdc, rectC.right / vovasize, rectC.bottom / vovasize, NULL);
			//	SetWindowExtEx(hdc, rectC.right, rectC.bottom, NULL);
		 //activehdc = CreateCompatibleDC(hdc);
			HBITMAP temphbm;
			HDC activehdc = CreateCompatibleDC(hdc);
			//SetMapMode(activehdc, MM_ISOTROPIC);
			//SetViewportExtEx(activehdc, rectC.right/vovasize, rectC.bottom / vovasize, NULL);
			//SetWindowExtEx(activehdc, rectC.right, rectC.bottom, NULL);
			temphbm = CreateCompatibleBitmap(hdc, rectC.right / vovasize, rectC.bottom / vovasize);
			hOld = SelectObject(activehdc, temphbm);

			GetClientRect(hwnd, &rectC);

			DrawOld(activehdc, RGB(199, 199, 199), rectC.left / vovasize, rectC.right / vovasize, rectC.top / vovasize, rectC.bottom / vovasize);
			for (int i = 5; i > 0; i--)
			{

				DrawOld(activehdc, RGB(199 - ((5 - i) * 10), 199 - ((5 - i) * 10), 199 - ((5 - i) * 10)), borderX - 2, rectC.right / vovasize - borderX + 3 + i,
					borderY - 2, rectC.bottom / vovasize - borderY + 3 + i);
			}

			DrawOld(activehdc, RGB(255, 255, 255), borderX, rectC.right / vovasize - borderX, borderY, rectC.bottom / vovasize - borderY);

			StretchBlt(activehdc, borderX, borderY, rectC.right / vovasize - borderX * 2, rectC.bottom / vovasize - borderY * 2, can->hdcMem, borderX, borderY, can->sizeX, can->sizeY, SRCCOPY);
			//BitBlt(activehdc,  border, border, rectC.right, rectC.bottom, activehdc, 0, 0, SRCCOPY);
			//Stre
			updatesize = false;
			BitBlt(can->hdcMem, 0, 0, rectC.right / vovasize, rectC.bottom / vovasize, activehdc, 0, 0, SRCCOPY);
			BitBlt(hdc, 0, 0, rectC.right / vovasize, rectC.bottom / vovasize, can->hdcMem, 0, 0, SRCCOPY);
			//SetMapMode(hdc, MM_ISOTROPIC);
			//SetViewportExtEx(hdc, rectC.right / vovasize, rectC.bottom / vovasize, NULL);
			//SetWindowExtEx(hdc, rectC.right, rectC.bottom, NULL);
			InvalidateRect(hwnd, NULL, TRUE);
		}
		if (resizeWorkProc && can != NULL)
		{
			GetClientRect(HWND_WorkProc, &rect);
			int centerx = rect.right / 2;
			int centery = rect.bottom / 2;
			posxy.x = centerx - can->sizeX / 2 * can->size - borderX;
			posxy.y = centery - can->sizeY / 2 * can->size - borderY;



			resizeWorkProc = false;
			MoveWindow(hwnd, posxy.x, posxy.y, can->sizeX * can->size + 2 * borderX, can->sizeY * can->size + 2 * borderY, TRUE);
			InvalidateRect(hwnd, NULL, TRUE);

		}

		EndPaint(hwnd, &ps);
	}

	break;

	case WM_LBUTTONDOWN:
	{
		SetFocus(hwnd);
		int x;
		int y;
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		wchar_t str[20];
		wchar_t str1[20];
		if (x >= borderX && x < rectC.right - borderX && y >= borderY && y < rectC.bottom - borderY)
		{

			x = x - borderX;
			y = y - borderY;

			//	y = y / can->size;
			//	x = x / can->size;



			//	can->SetPos(globalColor, x, y);

				//DrawOld(can->hdcMem, globalColor, x + border, x + 1 + border, y + border, y + 1 + border);

				//DrawOld(activehdc, globalColor, x* can->size + border, x* can->size + 1 * can->size + border, y * can->size + border,
				//	y* can->size + 1 * can->size + border);
			//MoveToEx(can->hdcMem, x + borderX, y + borderY, NULL);
			//LineTo(can->hdcMem, x + borderX, y + borderY);
			InvalidateRect(hwnd, NULL, true);
		}
		hold = true;
		globalmouse = 1;
		SetCapture(hwnd);
	}
	break;
	case WM_RBUTTONDOWN:
	{
		int x;
		int y;
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		if (x >= borderX && x < rectC.right - borderX && y >= borderY && y < rectC.bottom - borderY)
		{

			x = x - borderX;
			y = y - borderY;
			//y = y / can->size;
			//x = x / can->size;
			globalmouse = 2;
			erase = true;
			{
				//can->pos[y][x].e = false;

			//	DrawOld(can->hdcMem, RGB(0, 0, 0), x + border, x + 1 + border, y + border, y + 1 + border);

				DrawOld(can->hdcMem, RGB(0, 0, 0), x * can->size + borderX, x * can->size + can->size + borderX, y * can->size + borderY, y * can->size + can->size + borderY);
				/*	SetMapMode(activehdc, MM_ANISOTROPIC);
					SetViewportExtEx(activehdc, rectC.right * 2, rectC.bottom * 2, NULL);
					SetWindowExtEx(activehdc, rectC.right, rectC.bottom, NULL);*/
				InvalidateRect(hwnd, NULL, true);
			}

		}
		erase = true;
		globalmouse = 2;
		SetCapture(hwnd);
	}
	break;
	case WM_RBUTTONUP:
		globalmouse = 0;
		//lastx = -1;
		act = false;
		erase = false;
		ReleaseCapture();
		break;
	case WM_MOUSEHOVER:
		if (!focused)
		{
			focused = true;
			SetFocus(hwnd);
		}
		if (globalmouse == 1)
		{
			hold = true;
		}
		if (globalmouse == 2)
		{
			erase = true;
		}
		break;
	case WM_NCLBUTTONUP:
		globalmouse = 0;
		act = false;
		break;

	case WM_NCRBUTTONUP:
		globalmouse = 0;
		//lastx = -1;
		act = false;
		break;
	case WM_MOUSELEAVE:
	{
		focused = false;
		hold = false;
		erase = false;
		Tracking = false;
		//lastx = -1;
		act = false;
		InvalidateRect(hwnd, NULL, true);
	}
	break;

	case WM_MBUTTONDOWN:
		/*for (int i = 0; i < can->sizeY; i++)
		{
			for (int j = 0; j < can->sizeX; j++)
			{
				can->pos[i][j].e = false;
			}
		}*/

		DrawOld(can->hdcMem, RGB(0, 0, 0), borderX, can->sizeX + borderX, borderY, borderY + can->sizeY);
		//update = true;
		updatesize = true;
		erase = false;
		hold = false;
		act = false;
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_MOUSEMOVE:
	{
		if (!Tracking)
		{
			TrackMouse(hwnd);
			Tracking = true;
		}
		int x;
		int y;

		x = LOWORD(lParam);
		y = HIWORD(lParam);
		wchar_t str[20];
		wchar_t str1[20];
		//x = x / can->size;
		_itow_s((x - borderX) / can->size, str, 10);
		_itow_s(y - borderY, str1, 10);
		SetWindowText(doc, str);
		if (x >= borderX && x < rectC.right - borderY && y >= borderY && y < rectC.bottom - borderY)
		{
			x = x - borderX;
			y = y - borderY;
			y = y / can->size;
			x = x / can->size;
			if (hold)
			{
				SetWindowText(doc, L"lol");
				HPEN hpen = CreatePen(PS_SOLID, sizePen, globalColor);
				SelectObject(can->hdcMem, hpen);
				if (act)
				{
					LineTo(can->hdcMem, x + border, y + border);
				}
				DrawOld(can->hdcMem, globalColor, x + border, x + 1 + border, y + border, y + 1 + border);

				act = true;
				lastx = x + border;
				lasty = y + border;
				MoveToEx(can->hdcMem, lastx, lasty, NULL);
				DeleteObject(hpen);
				InvalidateRect(hwnd, NULL, true);

			}
			else
				if (erase)
				{
					HPEN hpen = CreatePen(PS_SOLID, sizePen, RGB(0, 0, 0));
					SelectObject(can->hdcMem, hpen);
					if (act)
					{
						LineTo(can->hdcMem, x + borderX + (rectC.right / 2) * (can->size - 1), y + borderY + (rectC.right / 2) * (can->size - 1));
					}
					//DrawOld(activehdc, RGB(0, 0, 0), x * can->size + border, x * can->size + can->size + border, y * can->size + border, y * can->size + can->size + border);
				//	LineTo(activehdc, x + border, y + border);
					act = true;
					lastx = x;
					lasty = y;
					MoveToEx(can->hdcMem, lastx + borderX + (rectC.right / 2) * (can->size - 1), lasty + borderY + (rectC.right / 2) * (can->size - 1), NULL);
					//	can->pos[y][x].e = false;
					//DrawOld(can->hdcMem, RGB(0, 0, 0), x + border, x + 1 + border, y + border, y + 1 + border);
					DeleteObject(hpen);
					InvalidateRect(hwnd, NULL, true);
				}

		}
		else
		{
			x = x - borderX;
			y = y - borderY;
			y = y / can->size;
			x = x / can->size;
			lastx = x + border;
			lasty = y + border;
			act = false;
		}
	}
	break;
	case WM_LBUTTONUP:
		hold = false;
		//lastx = -1;
		act = false;
		globalmouse = 0;
		ReleaseCapture();
		break;
	case WM_MOUSEWHEEL:
	{
		/*if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			sx = can->sizeX * can->size;
			sy = can->sizeY * can->size;
			can->size++;
			GetClientRect(HWND_WorkProc, &rect);
			int centerx = rect.right / 2;
			int centery = rect.bottom / 2;
			posxy.x = centerx - can->sizeX / 2 * can->size - borderX;
			posxy.y = centery - can->sizeY / 2 * can->size - borderY;
			// = true;
			MoveWindow(hwnd, posxy.x, posxy.y, can->sizeX * can->size + 2 * borderX, can->sizeY * can->size + 2 * borderY, TRUE);
			//	updatesize = true;
		}
		else
			if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
			{
				if (can->size > 1) {
					sx = can->sizeX * can->size;
					sy = can->sizeY * can->size;
					can->size--;

					GetClientRect(HWND_WorkProc, &rect);
					int centerx = rect.right / 2;
					int centery = rect.bottom / 2;
					posxy.x = centerx - can->sizeX / 2 * can->size - borderX;
					posxy.y = centery - can->sizeY / 2 * can->size - borderY;
					//update = false;
					//updatesize = true;
					MoveWindow(hwnd, posxy.x, posxy.y, can->sizeX * can->size + 2 * borderX, can->sizeY * can->size + 2 * borderY, TRUE);
					updatesize = true;

				}
			}
		//sizeupdate = true;
		//update = true;
		resizeWorkProc = true;
		InvalidateRect(hwnd, NULL, TRUE);*/
	}
	break;
	case WM_KEYDOWN:
	{
		if (wParam == VK_F1)
		{
			GetClientRect(hwnd, &rectC);
			//	HDC hdc = GetDC(hwnd);
			sizePen = 1;

			vova = true;
			GetClientRect(HWND_WorkProc, &rect);
			//can->size++;

			can->size++;
			int tsize = can->size;
			int centerx = rect.right / 2;
			int centery = rect.bottom / 2;
			posxy.x = centerx - can->sizeX / 2 * tsize - borderX;
			posxy.y = centery - can->sizeY / 2 * tsize - borderY;
			rectL = rectC;
			//	MoveWindow(hwnd, posxy.x, posxy.y, can->sizeX* tsize + 2 * border, can->sizeY * tsize + 2 * border, TRUE);
				//updatesize = true;
			InvalidateRect(hwnd, NULL, TRUE);			//SetMapMode(activehdc, MM_ANISOTROPIC);
			//SetViewportExtEx(activehdc, rectC.right / 8, rectC.bottom / 8, NULL);
			//SetWindowExtEx(activehdc, rectC.right, rectC.bottom, NULL);
			//updatesize = true;

		}
		else if (wParam == VK_F2)
		{
			sizePen = 2;
			if(can->size > 1) can->size--;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else if (wParam == VK_F3)
		{
			sizePen = 3;

		}
		else if (wParam == VK_F4)
		{
			sizePen = 4;

		}
		else if (wParam == VK_F5)
		{
			sizePen = 5;

		}
		else if (wParam == VK_F6)
		{
			sizePen = 6;

		}
		else if (wParam == VK_F7)
		{
			sizePen = 7;

		}
		else if (wParam == VK_F9)
		{
			sizePen = 20;

		}
		else if (wParam == VK_DELETE)
		{
			DrawOld(can->hdcMem, RGB(0, 0, 0), border, can->sizeX + border, border, border + can->sizeY);
			//update = true;
			//updatesize = true;
			erase = false;
			hold = false;
			act = false;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
	}break;
	case WM_SIZE:
	{
		resizeX = LOWORD(lParam);
		resizeY = HIWORD(lParam);
		break;
	case	WM_EXITSIZEMOVE:
	{


		/*if (abs(resizeX - firstRect.right*can->size) + abs(resizeY - firstRect.bottom*can->size) != 0)
		{
			if (abs(resizeX - firstRect.right*can->size) > abs(resizeY - firstRect.bottom*can->size))
			{
				can->size = (resizeX + 5) / can->sizeX;
			}
			else if (abs(resizeX - firstRect.right*can->size) < abs(resizeY - firstRect.bottom*can->size))
			{
				can->size = (resizeY + 5) / can->sizeY;
			}
			update = true;
			MoveWindow(hwnd, posxy.x, posxy.y, can->sizeX*can->size + 2 * border, can->sizeY*can->size + 2 * border, TRUE);
			InvalidateRect(hwnd, NULL, TRUE);
		}
		*/
	}
	}
	break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
