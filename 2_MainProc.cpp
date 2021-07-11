
#include<Windows.h>
#include"Source.h"

#define ID_FILE_CREATE 1001
#define ID_FILE_OPEN  1002
#define ID_FILE_SAVE  1003
bool ActiveCreate = false;
bool IsEmpty = true;
bool newCreate = false;
bool resizeWorkProc = false;
bool save = false;

LRESULT CALLBACK MainProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, hdcMem;
	HANDLE       hOld;
	HBITMAP  hbmMem;
	PAINTSTRUCT ps;
	RECT rect;
	RECT rectC;
	static HMENU hPopMenuFile;
	static HWND hwndCreate;
	switch (msg)
	{

	case WM_COMMAND:
	{
		switch (wParam)
		{
		case ID_FILE_CREATE:
		{
		if (!ActiveCreate) {
				hwndCreate = CreateWindowEx(0, L"Create", L"Новый рисунок",
					WS_CLIPCHILDREN | WS_OVERLAPPED | WS_BORDER | WS_SYSMENU , 800, 300,
					340, 280, hwnd, NULL, MainH, NULL);
				ShowWindow(hwndCreate, SW_NORMAL);
				UpdateWindow(hwndCreate);
			}
			ActiveCreate = true;
			
		}break;
		case ID_FILE_OPEN:
		{
			
			OPENFILENAME ofn;       
			wchar_t szFile[260];     
			HANDLE hf;              

			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFile = szFile;
			
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = NULL;// "All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			GetOpenFileName(&ofn);
		

				ULONG_PTR token;
				GdiplusStartupInput input;
				GdiplusStartup(&token, &input, 0);
				{
					Bitmap bitmap(ofn.lpstrFile);
					UINT w = bitmap.GetWidth(), h = bitmap.GetHeight();
					HBITMAP hBmp;
					bitmap.GetHBITMAP(Color(), &hBmp);

					HWND hwndCanvas;
					hwndCanvas = CreateWindowEx(0, L"Canvas", (LPCTSTR)NULL,
						WS_CHILD, 0, 0,
						w, h, HWND_WorkProc, NULL, MainH, NULL);
					resizeWorkProc = true;
					Canvas* temp;
					temp = Canvas::AddCan(hwndCanvas, w, h, true);

					newHwnd = hwndCanvas;
					SetFocus(hwndCanvas);

					HDC veryNewHwnd = GetWindowDC(hwndCanvas);
					RECT rect;
					GetClientRect(hwndCanvas, &rect);
					temp->hdcMem = CreateCompatibleDC(veryNewHwnd);
					HDC hCompatibleDC = CreateCompatibleDC(temp->hdcMem);
					SelectObject(hCompatibleDC, hBmp);
					hbmMem = CreateCompatibleBitmap(veryNewHwnd, rect.right, rect.bottom);
					hOld = SelectObject(temp->hdcMem, hbmMem);

					update = true;
					can = temp;
					openUpdate = true;
					can->fileName = ofn.lpstrFile;
					BitBlt(temp->hdcMem, 0, 0, w, h, hCompatibleDC, 0, 0, SRCCOPY);
					DeleteObject(hBmp);
				}
				GdiplusShutdown(token);
			
			
			InvalidateRect(HWND_WorkProc_2, NULL, TRUE);
	
			InvalidateRect(hwnd, NULL, TRUE);
		}break;
		case ID_FILE_SAVE:
		{
			save = true;
			if (can == NULL)
			{
				MessageBox(hwnd, L"Не вижу рисунок", L"Ошибка", MB_OK);
			}
			else {
				OPENFILENAME ofn;       // common dialog box structure
				wchar_t szFile[260];       // buffer for file name
				HANDLE hf;              // file handle
				//hdcMem = CreateCompatibleDC(hdc);
				GetClientRect(hwnd, &rectC);
				HWND tempHwnd = CreateWindowEx(0, L"Save", L"Новый рисунок",
					WS_DISABLED, 800, 300,
					can->sizeX, can->sizeY, NULL, NULL, MainH, NULL);

				int bitmap_dx = can->sizeX;
				int bitmap_dy = can->sizeY;
				BITMAPINFOHEADER bmpInfoHeader;
				BITMAPFILEHEADER bmpFileHeader;
				BITMAP* pBitmap;

				bmpFileHeader.bfType = 0x4d42;
				bmpFileHeader.bfSize = 0;
				bmpFileHeader.bfReserved1 = 0;
				bmpFileHeader.bfReserved2 = 0;
				bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

				bmpInfoHeader.biSize = sizeof(bmpInfoHeader);
				bmpInfoHeader.biWidth = bitmap_dx;
				bmpInfoHeader.biHeight = bitmap_dy;
				bmpInfoHeader.biPlanes = 1;
				bmpInfoHeader.biBitCount = 24;
				bmpInfoHeader.biCompression = BI_RGB;
				bmpInfoHeader.biSizeImage = bitmap_dx * bitmap_dy * (24 / 8);
				bmpInfoHeader.biXPelsPerMeter = 0;
				bmpInfoHeader.biYPelsPerMeter = 0;
				bmpInfoHeader.biClrUsed = 0;
				bmpInfoHeader.biClrImportant = 0;
				BITMAPINFO info;
				info.bmiHeader = bmpInfoHeader;

				BYTE* memory;
				HDC winDC = GetWindowDC(tempHwnd);

				HDC bmpDC = CreateCompatibleDC(winDC);

				//Создаем битмэп
				HBITMAP hbmMem = CreateCompatibleBitmap(winDC, can->sizeX, can->sizeY);
				SelectObject(bmpDC, hbmMem);
				int border = 700;
				// BitBlt(bmpDC, 0, 0, can->sizeX, can->sizeY, can->hdcMem, border, border, SRCCOPY);
				BitBlt(winDC, 0, 0, can->sizeX, can->sizeY, can->hdcMem, border, border, SRCCOPY);
				HBITMAP bitmap = CreateDIBSection(winDC, &info, DIB_RGB_COLORS, (void**)& memory, NULL, 0);
				SelectObject(bmpDC, bitmap);//Выбираем в контекст битмэп
				BitBlt(bmpDC, 0, 0, bitmap_dx, bitmap_dy, winDC, 0, 0, SRCCOPY);
				ReleaseDC(can->hwnd, winDC);

				// Initialize OPENFILENAME
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hwnd;
				ofn.lpstrFile = szFile;
				// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
				// use the contents of szFile to initialize itself.
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = L"*.png;*.jpg;*jpeg";// "All\0*.*\0Text\0*.TXT\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

				GetSaveFileName(&ofn); //MessageBox(hwnd,"Невозможно сохранить файл", "О программе...",MB_ICONINFORMATION);

				HANDLE hFile = CreateFile(
					ofn.lpstrFile, GENERIC_WRITE,
					0, NULL, OPEN_ALWAYS,
					FILE_ATTRIBUTE_NORMAL, NULL);


				can->fileName = ofn.lpstrFile;
				DWORD dwWritten = 0;
				WriteFile(hFile, &bmpFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
				WriteFile(hFile, &bmpInfoHeader, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
				WriteFile(hFile, memory, bmpInfoHeader.biSizeImage, &dwWritten, NULL);
				CloseHandle(hFile);
				//HBITMAP bitmap;
				//bitmap = (HBITMAP)GetCurrentObject(can->hdcMem, OBJ_BITMAP);
			}
			
		}break;
		}
	}break;
	case WM_CREATE:
	{

		WNDCLASSEX createclass;
		createclass.cbSize = sizeof(createclass);
		createclass.style = CS_CLASSDC;
		createclass.lpfnWndProc = CreateProc;
		createclass.cbClsExtra = 0;
		createclass.cbWndExtra = 0;
		createclass.hInstance = MainH;
		createclass.hIcon = NULL;
		createclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		createclass.hbrBackground = NULL;//(HBRUSH)GetStockObject(Bl_BRUSH);
		createclass.lpszMenuName = NULL;
		createclass.lpszClassName = L"Create";
		createclass.hIconSm = NULL;
		RegisterClassEx(&createclass);


		//GetClientRect(hwnd, &rectC);


		HMENU hmenu1;
		hmenu1 = CreateMenu();
		hPopMenuFile = CreatePopupMenu();


		AppendMenu(hmenu1, MF_STRING | MF_POPUP, (UINT)hPopMenuFile, L"&Файл");
		AppendMenu(hPopMenuFile, MF_STRING, ID_FILE_CREATE, L"&Создать");
		AppendMenu(hPopMenuFile, MF_STRING, ID_FILE_OPEN, L"&Открыть");
		AppendMenu(hPopMenuFile, MF_STRING, ID_FILE_SAVE, L"&Сохранить");
		//AppendMenu()
		AppendMenu(hmenu1, MF_STRING, 0, L"&Справка");
		//AppendMenu(hmenu1, MF_STRING, 0, L"&Help");


		SetMenu(hwnd, hmenu1);

		HWND_MainProc = hwnd;
		GetClientRect(hwnd, &rectC);
		HWND hwnd2;
		doc = hwnd;
		WNDCLASSEX wndclass2;
		wndclass2.cbSize = sizeof(wndclass2);
		wndclass2.style = CS_NOCLOSE;
		wndclass2.lpfnWndProc = WorkProc;
		wndclass2.cbClsExtra = 0;
		wndclass2.cbWndExtra = 0;
		wndclass2.hInstance = MainH;
		wndclass2.hIcon = NULL;
		wndclass2.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass2.hbrBackground = NULL;//(HBRUSH)GetStockObject(Bl_BRUSH);
		wndclass2.lpszMenuName = NULL;
		wndclass2.lpszClassName = L"Workspace";
		wndclass2.hIconSm = NULL;
		RegisterClassEx(&wndclass2);
		hwnd2 = CreateWindowEx(0, L"Workspace", (LPCTSTR)NULL,
			WS_CHILD | WS_BORDER | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | WS_CLIPCHILDREN, 0, 30,
			1000, 600, hwnd, NULL, MainH, NULL);

		ShowWindow(hwnd2, SW_NORMAL);
		UpdateWindow(hwnd2);

		HWND hwnd3;
		WNDCLASSEX wndclass3;
		wndclass3.cbSize = sizeof(wndclass3);
		wndclass3.style = CS_NOCLOSE;
		wndclass3.lpfnWndProc = WorkProc_2;
		wndclass3.cbClsExtra = 0;
		wndclass3.cbWndExtra = 0;
		wndclass3.hInstance = MainH;
		wndclass3.hIcon = NULL;
		wndclass3.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass3.hbrBackground = NULL;//(HBRUSH)GetStockObject(Bl_BRUSH);
		wndclass3.lpszMenuName = NULL;
		wndclass3.lpszClassName = L"Workspace2";
		wndclass3.hIconSm = NULL;
		RegisterClassEx(&wndclass3);
		GetClientRect(hwnd, &rectC);
		hwnd3 = CreateWindowEx(0, L"Workspace2", (LPCTSTR)NULL,
			WS_CHILD | WS_BORDER | WS_VISIBLE | WS_CLIPCHILDREN, 0, 0,
			1000, 25, hwnd, NULL, MainH, NULL);
		ShowWindow(hwnd3, SW_NORMAL);
		UpdateWindow(hwnd3);

		HWND hwndtool;
		WNDCLASSEX toolclass;
		toolclass.cbSize = sizeof(toolclass);
		toolclass.style = CS_NOCLOSE;
		toolclass.lpfnWndProc = ToolProc;
		toolclass.cbClsExtra = 0;
		toolclass.cbWndExtra = 0;
		toolclass.hInstance = MainH;
		toolclass.hIcon = NULL;
		toolclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		toolclass.hbrBackground = NULL;
		toolclass.lpszMenuName = NULL;
		toolclass.lpszClassName = L"Tool";
		toolclass.hIconSm = NULL;
		RegisterClassEx(&toolclass);

		hwndtool = CreateWindowEx(0, L"Tool", (LPCTSTR)NULL,
			WS_CHILD | WS_VISIBLE, rectC.right - 400, 0,
			400, 300, hwnd, NULL, MainH, NULL);
		ShowWindow(hwndtool, SW_NORMAL);
		UpdateWindow(hwndtool);

		HWND hwndpick;
	
		WNDCLASSEX pickclass;
		pickclass.cbSize = sizeof(pickclass);
		pickclass.style = CS_NOCLOSE;
		pickclass.lpfnWndProc = PickColorProc;
		pickclass.cbClsExtra = 0;
		pickclass.cbWndExtra = 0;
		pickclass.hInstance = MainH;
		pickclass.hIcon = NULL;
		pickclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		pickclass.hbrBackground = NULL;
		pickclass.lpszMenuName = NULL;
		pickclass.lpszClassName = L"PickColor";
		pickclass.hIconSm = NULL;
		RegisterClassEx(&pickclass);

		hwndpick = CreateWindowEx(0, L"PickColor", (LPCTSTR)NULL,
			WS_CHILD | WS_VISIBLE, rectC.right - 400, 0,
			400, 300, hwnd, NULL, MainH, NULL);
		ShowWindow(hwndpick, SW_NORMAL);
		UpdateWindow(hwndpick);
	
		HWND hwndContainColor;
		WNDCLASSEX wndclassContainColor;
		wndclassContainColor.cbSize = sizeof(wndclassContainColor);
		wndclassContainColor.style = CS_NOCLOSE;
		wndclassContainColor.lpfnWndProc = ContainColorProc;
		wndclassContainColor.cbClsExtra = 0;
		wndclassContainColor.cbWndExtra = 0;
		wndclassContainColor.hInstance = MainH;
		wndclassContainColor.hIcon = NULL;
		wndclassContainColor.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclassContainColor.hbrBackground = NULL;
		wndclassContainColor.lpszMenuName = NULL;
		wndclassContainColor.lpszClassName = L"ContainColor";
		wndclassContainColor.hIconSm = NULL;
		RegisterClassEx(&wndclassContainColor);
		parent = hwnd;
		hwndContainColor = CreateWindowEx(0, L"ContainColor", (LPCTSTR)NULL,
			WS_CHILD  | WS_VISIBLE , rectC.right - 400, 400,
			400, rectC.bottom - 400, hwnd, NULL, MainH, NULL);
		//SetFocus(hwndContainColor);
		ShowWindow(hwndContainColor, SW_NORMAL);
		UpdateWindow(hwndContainColor);
	}
		break;
	case WM_SIZE:
		GetClientRect(hwnd, &rectC);
		
		
		MoveWindow(HWND_WorkProc, 0, 30, rectC.right - 400, rectC.bottom - 30,  TRUE);
		MoveWindow(HWND_WorkProc_2, 0, 0, rectC.right - 400, 32, TRUE);
		
	
		if (rectC.bottom < 850) {
			MoveWindow(HWND_ToolProc, rectC.right - 400, 0, 400, 140, TRUE);
			MoveWindow(HWND_PickColorProc, rectC.right - 400, 140, 400, rectC.bottom - 325, TRUE);
			size = 26;
			MoveWindow(HWND_ContainColorProc, rectC.right - 400, rectC.bottom - 185, 400, 185, TRUE);
		}
		else {
		
				MoveWindow(HWND_ToolProc, rectC.right - 400, 0, 400, 140, TRUE);
				MoveWindow(HWND_PickColorProc, rectC.right - 400, 140, 400, rectC.bottom - 465, TRUE);
				size = 36;
				MoveWindow(HWND_ContainColorProc, rectC.right - 400, rectC.bottom - 325, 400, 325, TRUE);
			
		}
		resizeWorkProc = true;
		//update = true;
		updatehwnd = true;
		InvalidateRect(HWND_CanvasProc, NULL, TRUE);
		InvalidateRect(HWND_WorkProc, NULL, TRUE);
		InvalidateRect(HWND_ToolProc, NULL, TRUE);
	
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		hdcMem = CreateCompatibleDC(hdc);
		GetClientRect(hwnd, &rectC);

		hbmMem = CreateCompatibleBitmap(hdc, rectC.right, rectC.bottom);
		hOld = SelectObject(hdcMem, hbmMem);
		{
			HBRUSH fill = CreateSolidBrush(RGB(241, 241, 241));
			FillRect(hdcMem, &rectC, fill);
			DeleteObject(fill);
		}


		BitBlt(hdc, 0, 0, rectC.right, rectC.bottom, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, hOld);
		DeleteObject(hbmMem);
		DeleteDC(hdcMem);

		EndPaint(hwnd, &ps);
	}
		break;
	case WM_NCLBUTTONUP:
		globalmouse = 0;
		break;
	case WM_NCRBUTTONUP:
		globalmouse = 0;
		break;
	case WM_DESTROY:
		DestroyMenu(hPopMenuFile);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

}