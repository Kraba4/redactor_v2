#include<Windows.h>
#include<iostream>
#include"Source.h"

//#pragma comment(linker, "/HEAP:2000000")
using namespace std;
RGBE globalColor;
HWND HWND_MainProc;
HWND HWND_WorkProc;
HWND HWND_WorkProc_2;
HWND HWND_CanvasProc;
HWND HWND_TextRGBProc;
HWND HWND_PickColorProc;
HWND HWND_ContainColorProc;
HWND HWND_ToolProc;
HWND HWND_CreateProc;
HWND newHwnd;
HWND workingHwnd;
bool openUpdate = false;
Canvas* can;
Canvas* Canvas::end;
Canvas* Canvas::first;
void DrawOld(HDC hMem, COLORREF rgb, int left, int right, int top, int bottom)
{
	HBRUSH hb = CreateSolidBrush(rgb);
	RECT rect;
	rect.left = left;
	rect.right = right;
	rect.top = top;
	rect.bottom = bottom;
	FillRect(hMem, &rect, hb);
	DeleteObject(hb);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR czCmdLine, int CmdShow)
{
	MainH = hInstance;
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc = MainProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = L"Redactor";
	wndclass.hIconSm = NULL;

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(L"Redactor", L"RedactorV2", WS_OVERLAPPEDWINDOW  | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, 1420, 780, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, CmdShow);
	UpdateWindow(hwnd);
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
