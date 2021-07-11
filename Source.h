#pragma once
#include<Windows.h>
//#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;
extern COLORREF ToRGB(unsigned int i_hue, unsigned char sat, unsigned char val);
extern bool IsEmpty;
struct RGBE
{
	byte R;
	byte G;
	byte B;
	bool e;

	void operator=(COLORREF a)
	{
		R = GetRValue(a);
		G = GetGValue(a);
		B = GetBValue(a);
		e = true;
	}
	void operator=(RGBE a)
	{
		R = a.R;
		G = a.G;
		B = a.B;
		e = a.e;
	}
	operator COLORREF() {
		return RGB(R, G, B);
	}

};
struct HSV
{
	unsigned int H, S, V;
	bool e;
	operator COLORREF() {

		return ToRGB(H,S,V);
	}
};
extern bool newCreate;
extern HWND HWND_WorkProc_2;
class Canvas
{
public:RGBE** pos;
public:
	static Canvas* first;
	static Canvas* end;
	HWND hwnd;
	Canvas* next;
	int size;
	int sizeX, sizeY;
	int startX, startY;
	bool init;
	HDC hdcMem;
	int number;
	LPWSTR fileName;
	Canvas(int x, int y)
	{
		size = 1;
		startX = 0;
		startY = 0;
		pos = new RGBE*[y];
		for (int i = 0; i < y; i++)
		{
			pos[i] = new RGBE[x];
		}
	}
	void SetPos(COLORREF a, int x, int y)
	{
		pos[y][x] = a;
	}
	void ErasePos(int x, int y)
	{
		pos[y][x].e = false;
	}
	RGBE GetPos(int x, int y)
	{
		return pos[y][x];
	}
	static Canvas* AddCan(HWND h, int x, int y,bool enable)
	{
		Canvas* cur;
		if (IsEmpty)
		{
			first = new Canvas(x, y);
			first->hwnd = h;
			first->sizeX = x;
			first->sizeY = y;
			first->init = false;
			IsEmpty = false;
			end = first;
		
		}
     	else
		{
			
			end->next = new Canvas(x, y);
			
			
			end = end->next;
			end->hwnd = h;
			end->sizeX = x;
			end->sizeY = y;
			if (!enable)end->init = false; else end->init = true;
		}
		
			newCreate = true;
			
			return end;
	}
   static Canvas* Search(HWND h)
	{
		Canvas* temp;
		temp = first;
		while (temp != NULL)
		{
			if (temp->hwnd == h) break;
			else temp = temp->next;
		}
		return temp;

	}
   static bool SearchFile(LPWSTR name,Canvas* open)
   {
	   Canvas* temp;
	   temp = first;
	   while (temp != NULL)
	   {
		   if (temp->fileName == name) {
			   return true;
			   open = temp;
		   }
		   else temp = temp->next;
	   }
	   return false;
   }
};
HSV ToHSV(unsigned char r, unsigned char g, unsigned char b);
extern COLORREF pickerColor;
extern HSV temp;
extern Canvas* main;
extern HWND parent;
extern HINSTANCE MainH;
extern HWND doc;
extern RGBE selected_rgbe;
extern RGBE globalColor;
extern int globalmouse;
extern HWND hwndtext_value[3];
extern HSV colorBuff[70];
extern int colorBuff_count;
extern HWND HWND_MainProc;
extern HWND HWND_WorkProc;
extern HWND newHwnd;
extern HWND workingHwnd;
extern HWND HWND_CanvasProc;
extern HWND HWND_TextRGBProc;
extern HWND HWND_PickColorProc;
extern HWND HWND_ContainColorProc;
extern HWND HWND_ToolProc;
extern HWND HWND_CreateProc;
extern bool openUpdate;
extern bool resizeWorkProc;
extern int size, line;
extern int startAct;
extern int posColor;
extern int colorBuff_selected;
extern  int s , v;
extern bool ActiveCreate;
extern bool update;
extern bool selectupdate;
extern bool save;
extern bool updatehwnd;
extern HDC activehdc;
extern Canvas* can;
void TrackMouse(HWND hwnd);
 LRESULT CALLBACK MainProc(HWND, UINT, WPARAM, LPARAM);
 LRESULT CALLBACK CanvasProc(HWND, UINT, WPARAM, LPARAM);
 LRESULT CALLBACK WorkProc(HWND, UINT, WPARAM, LPARAM);
 LRESULT CALLBACK WorkProc_2(HWND, UINT, WPARAM, LPARAM);
 LRESULT CALLBACK ContainColorProc(HWND, UINT, WPARAM, LPARAM);
 LRESULT CALLBACK TextRGBProc(HWND, UINT, WPARAM, LPARAM);
 LRESULT CALLBACK PickColorProc(HWND, UINT, WPARAM, LPARAM);
 LRESULT CALLBACK ToolProc(HWND, UINT, WPARAM, LPARAM);
 LRESULT CALLBACK CreateProc(HWND, UINT, WPARAM, LPARAM);
 void DrawOld(HDC hMem, COLORREF rgb, int left, int right, int top, int bottom);
 void DrawLineOld(int left, int right, LPARAM lparam);