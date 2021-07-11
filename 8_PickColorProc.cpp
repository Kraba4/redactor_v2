#include<Windows.h>
#include<cmath>

#include"Source.h"
#include <TCHAR.H>

COLORREF squareColor[52][52];

int s = 255, v = 0;
int posColor = 30;
int startAct = true;

HSV ToHSV(unsigned char r, unsigned char g, unsigned char b)

	/*

	* 0 <= r,g,b <= 255 - значения красного, зеленого, синего

	*

	*

	* Выход:

	* 0 <= hue <= 360 градусов - оттенок.

	* 0 <= sat <= 255 - Saturation - насыщенность

	* 0 <= val <= 255 - Value - яркость

	*/

{

	double h, rc, gc, bc, dmax;

	unsigned char s, v;

	unsigned char minc, maxc;


	maxc = (r > g) ? ((r > b) ? r : b) : ((g > b) ? g : b);

	minc = (r < g) ? ((r < b) ? r : b) : ((g < b) ? g : b);


	s = 0; // Насыщенность 



	if (maxc)

	{

		s = (maxc - minc) * 255 / maxc;

	}

	int sat, val, hue =0;
	sat = s;

	val = maxc;


	if (!s)

	{ 
		HSV temp;
		temp.H = 0;
		temp.S = sat;
		temp.V = val;
		return temp;

	}

	else

	{ // Хроматический цвет 

		dmax = maxc - minc;

		rc = (maxc - r) / dmax; /* rc - удаленность */

		gc = (maxc - g) / dmax; /* цвета от красного */

		bc = (maxc - b) / dmax;

		if (r == maxc)

		{ // Цвет между желтым и пурпурным 

			h = bc - gc;

		}

		else if (g == maxc)

		{ // Цвет между голубым и желтым 

			h = 2 + rc - bc;

		}

		else

		{ // Цвет между пурпурным и голубым 

			h = 4 + gc - rc;

		}

		h *= 60.0;

		if (h < 0.0)

		{

			h += 360.0;

		}

		hue = h;

		if (hue == 360)

		{

			hue = 0;

		}
		HSV temp;
		temp.H = hue;
		temp.S = sat;
		temp.V = val;
		return temp;
	} // if (!s) 

} // RGB 2 HSV
	COLORREF ToRGB(unsigned int i_hue, unsigned char sat, unsigned char val)

		/* Вход:

		* 0 <= hue < 360 градусов - оттенок. Основные цвета:

		* 0 - красный, 60 - желтый,

		* 120 - зеленый, 180 - голубой

		* 240 - синий, 300 - пурпурный

		* hue == 360 - неопределён !!!

		* Остальные цвета между ними

		* 0 <= sat <= 255 - Saturation - насыщенность

		* 0 <= val <= 255 - Value - светлота

		*

		* Выход:

		* 0 <= r,g,b <= 255 - значения красного, зеленого, синего
		*/
		

	{
		int r, g, b;
		int ii;

		double fr, hue;

		unsigned char c1, c2, c3;

		if (!sat)
		{
			r = val;
			g = val; 
			b = val;
		}
		else

		{ 

			while (i_hue >= 360) i_hue -= 360;

			hue = i_hue;

			ii = (int)(hue /= 60.0);

			fr = hue - ii;

			c1 = (val * (255 - sat)) / 255;

			c2 = (val * (255 - sat * fr)) / 255;

			c3 = (val * (255 - sat * (1.0 - fr))) / 255;

			switch (ii)

			{

			case 0: r = val; g = c3; b = c1; break;

			case 1: r = c2; g = val; b = c1; break;

			case 2: r = c1; g = val; b = c3; break;

			case 3: r = c1; g = c2; b = val; break;

			case 4: r = c3; g = c1; b = val; break;

			case 5: r = val; g = c1; b = c2; break;

			} // switch

		} // if (!sat)
		return RGB(r , g , b );
	} // HSV 2 RGB
	
LRESULT CALLBACK PickColorProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool posAct = false;

	static bool changeHSV = false;

	HDC hdc, hdcMem;
	HWND hwndtext;
	HANDLE       hOld;
	HBITMAP  hbmMem;
	PAINTSTRUCT ps;
	RECT rect;
	RECT rectC;
	HBRUSH oldBrush;
	HFONT fnt;
	HFONT oldFont;
	HPEN hpen, oldPen;
	static bool Tracking = false;
	static bool focused = false;
	switch (msg)
	{
	case WM_CREATE:
		HWND_PickColorProc = hwnd;
		WNDCLASSEX textRGBclass;
		textRGBclass.cbSize = sizeof(textRGBclass);
		textRGBclass.style = CS_NOCLOSE;
		textRGBclass.lpfnWndProc = TextRGBProc;
		textRGBclass.cbClsExtra = 0;
		textRGBclass.cbWndExtra = 0;
		textRGBclass.hInstance = MainH;
		textRGBclass.hIcon = NULL;
		textRGBclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		textRGBclass.hbrBackground = NULL;
		textRGBclass.lpszMenuName = NULL;
		textRGBclass.lpszClassName = L"TextRGB";
		textRGBclass.hIconSm = NULL;
		RegisterClassEx(&textRGBclass);
			hwndtext = CreateWindowEx(0, L"TextRGB", (LPCTSTR)NULL,
					WS_CHILD | WS_VISIBLE, 20, 340,
					500, 40, hwnd, NULL, MainH, NULL);
				ShowWindow(hwndtext, SW_SHOWNORMAL);
		break;
		case WM_PAINT:
		{
			
			hdc = BeginPaint(hwnd, &ps);
			hdcMem = CreateCompatibleDC(hdc);
			GetClientRect(hwnd, &rectC);

			hbmMem = CreateCompatibleBitmap(hdc, rectC.right, rectC.bottom);
			hOld = SelectObject(hdcMem, hbmMem);


			HBRUSH fill = CreateSolidBrush(RGB(199, 199, 199));
				FillRect(hdcMem, &rectC, fill);
				DeleteObject(fill);
				//fill = CreateSolidBrush(RGB(100, 199, 199));
				//DeleteObject(fill);
				//hpen = CreatePen(PS_SOLID, 1, RGB(100, 199, 199));
				//DeleteObject(hpen);
				//oldPen = (HPEN)SelectObject(hdcMem, hpen);
				COLORREF tempColor = ToRGB(posColor, s, 255 - v);
				int r = 255, g = 0, b = 0;
				if (startAct)
				{
					for (int i = 0; i <= 255; i += 5)
					{
						for (int j = 0; j <= 255; j += 5)
						{
							
							squareColor[i / 5][j / 5] = ToRGB(posColor, j, 255 - i);

						}
					}
					startAct = false;
				}
		
				rect.left = 20;
				rect.right = rect.left + 120;
				rect.top = 10 ;
				rect.bottom = rect.top + 260;
				fill = CreateSolidBrush(globalColor);
				FillRect(hdcMem, &rect, fill);
				DeleteObject(fill);
				rect.left = 80;
				rect.right = 120;
				rect.top = 20;
				rect.bottom = 60;
				fill = CreateSolidBrush(tempColor);
				//FillRect(hdcMem, &rect, fill);
				SelectObject(hdcMem, fill);
				if (v > 150)
				{
					hpen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
				}else hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
				SelectObject(hdcMem, hpen);
				Rectangle(hdcMem, rect.left, rect.top, rect.right, rect.bottom);
				DeleteObject(hpen);
				DeleteObject(fill);
				for (int i = 0; i <= 255; i += 5)
				{
					for (int j = 0; j <= 255; j += 5)
					{
						rect.left = 125 + j;
						rect.right = rect.left + 5;
						rect.top = 10 + i;
						rect.bottom = rect.top + 5;
						HBRUSH fill = CreateSolidBrush(squareColor[i / 5][j / 5]);
						FillRect(hdcMem, &rect, fill);
						DeleteObject(fill);

					}
				}
				
				if (v > 150)
				{
					hpen = CreatePen(BS_SOLID, 1, RGB(255, 255, 255));
				}else hpen = CreatePen(BS_SOLID, 1, RGB(0, 0, 0));
				SelectObject(hdcMem, (HPEN)hpen);
				SelectObject(hdcMem, GetStockObject(NULL_BRUSH));
				rect.left = 125 + s;
				rect.right = rect.left + 5;
				rect.top = 10 + v;
				rect.bottom = rect.top + 5;
				Rectangle(hdcMem, rect.left, rect.top, rect.right, rect.bottom);
				DeleteObject(hpen);
				for (int i = 0; i <= 360; i++)
				{
					//if (r > 254) r = 254;
				//	if (g > 254) g = 254;
					//if (b > 254) b = 254;
					//if (i < 85) g += 3; else
					//	if (i >= 85 && i < 170) r -= 3; else
					//		if (i >= 170 && i < 255) b += 3; else
					//			if (i >= 255 && i < 340) g -= 3; else
					//				if (i >= 340 && i < 425) r += 3; else
					//					if (i >= 425 && i < 510) b -= 3;

					fill = CreateSolidBrush(ToRGB(i, 255, 255));

					rect.left = 20 + i;
					rect.right = rect.left + 1;
					rect.top = 280;
					rect.bottom = 288;
					FillRect(hdcMem, &rect, fill);
					DeleteObject(fill);
					

					//FillRect(hdcMem, &rect, fill);
					DeleteObject(fill);

				}
				
				wchar_t str2[4];
				wchar_t str3[4];
				wchar_t str4[4];
				//wchar_t str1[11];
				wchar_t temp[2] = L",";
				wchar_t str1[12];
				/*SendMessage(hwndtext_value[0], WM_GETTEXT, 12, (LPARAM)str1);
				int r_value=0, g_value =0, b_value;
				int i = 0;
				int count = 0;
				while (i < 3 && (str1[count]!=','||str1[count]!='.') )
				{
				
					if (str1[count] >= 48 && str1[count] <= 57)
					{
						r_value *= 10;
						r_value += str1[count] - 48;
						i++;
						count++;
					}
					else
					{
						r_value = -1;
						break;
					}
				}
				count++;
				i = 0;
				while (i < 3 && (str1[count] != ',' || str1[count] != '.'))
				{
					
					if (str1[count] >= 48 && str1[count] <= 57)
					{
						g_value *= 10;
						g_value += str1[count] - 48;
						i++;
						count++;
					}
					else
					{
						g_value = -1;
						break;
					}
				}
				count++;
				i = 0;
				while (i < 3 && (str1[count] != ',' || str1[count] != '.'))
				{
				
					if (str1[count] >= 48 && str1[count] <= 57)
					{
						b_value *= 10;
						b_value += str1[count] - 48;
						i++;
						count++;
					}
					else
					{
						b_value = -1;
						break;
					}
				}
				if (r_value >= 0 && g_value >= 0 && b_value >= 0)
				{
					
				}*/
				//_itow_s(r_value, str2, 10);
				//_itow_s(g_value, str2, 10);
				//_itow_s(b_value, str2, 10);
				//MessageBox(NULL, str2, L"ljl", MB_OK);
				_itow_s(GetRValue(tempColor), str2, 10);
				_itow_s(GetGValue(tempColor), str3, 10);
				_itow_s(GetBValue(tempColor), str4, 10);
			
				wcscpy_s(str1, 12, str2);
				wcscat_s(str1, 12, temp);
				wcscat_s(str1, 12, str3);
				wcscat_s(str1, 12, temp);
				wcscat_s(str1, 12, str4);
			
				LPARAM l;
				SendMessage(hwndtext_value[0], WM_SETTEXT, NULL, (LPARAM)str1);
				wchar_t str1_hsv[12];
				wchar_t str2_hsv[4];
				wchar_t str3_hsv[4];
				wchar_t str4_hsv[4];
				_itow_s(posColor, str2_hsv, 10);
				_itow_s(s, str3_hsv, 10);
				_itow_s(255-v, str4_hsv, 10);

				wcscpy_s(str1_hsv, 12, str2_hsv);
				wcscat_s(str1_hsv, 12, temp);
				wcscat_s(str1_hsv, 12, str3_hsv);
				wcscat_s(str1_hsv, 12, temp);
				wcscat_s(str1_hsv, 12, str4_hsv);
				SendMessage(hwndtext_value[1], WM_SETTEXT, NULL, (LPARAM)str1_hsv);
				wchar_t str1_hex[12];
				wchar_t str2_hex[3];
				wchar_t str3_hex[3];
				wchar_t str4_hex[3];
				_itow_s(GetRValue(tempColor), str2_hex, 16);
				_itow_s(GetGValue(tempColor), str3_hex, 16);
				_itow_s(GetBValue(tempColor), str4_hex, 16);
				wchar_t temp_hex[2] = L"#";
				wcscpy_s(str1_hex, 9, temp_hex);
				wcscat_s(str1_hex, 9, str2_hex);
				wcscat_s(str1_hex,9, str3_hex);
				wcscat_s(str1_hex, 9, str4_hex);
				SendMessage(hwndtext_value[2], WM_SETTEXT, NULL, (LPARAM)str1_hex);
				RECT pos;
				POINT points[3];
				points[0].x = 20 + posColor;
				points[0].y = 288;
				points[1].x = points[0].x - 3;
				points[1].y = points[0].y + 7;
				points[2].x = points[0].x + 3;
				points[2].y = points[0].y + 7;
				fill = CreateSolidBrush(ToRGB(posColor, 255, 255));
				hpen = CreatePen(BS_SOLID, 1, ToRGB(posColor, 255, 255));
				SelectObject(hdcMem, (HBRUSH)fill);
				SelectObject(hdcMem, (HPEN)hpen);
				//fill = CreateSolidBrush(RGB(255, 255, 255));
				//DeleteObject(fill);
				
				Polygon(hdcMem, points, 3);
				DeleteObject(hpen);
				pos.left = 20 + posColor - 7;
				pos.right = 20 + posColor + 7;
				pos.top = points[1].y;
				pos.bottom = pos.top + 14;
				FillRect(hdcMem, &pos, fill);
				DeleteObject(fill);

				/*fill = CreateSolidBrush(ToRGB(posColor, 255, 255));
				pos.left = 10 + posColor - 5;
				pos.right = pos.left + 5;
				pos.top = rect.top - 3;
				pos.bottom = rect.bottom + 3;
				FillRect(hdcMem, &pos, fill);
				DeleteObject(fill);
				DeleteObject(fill);
				SelectObject(hdcMem, oldPen);
				DeleteObject(oldPen);
				DeleteObject(hpen);
				*/
				//DeleteObject(oldBrush);
				//DeleteObject(fill);

			BitBlt(hdc, 0, 0, rectC.right, rectC.bottom, hdcMem, 0, 0, SRCCOPY);

			SelectObject(hdcMem, hOld);
			DeleteObject(hbmMem);
			DeleteDC(hdcMem);

			EndPaint(hwnd, &ps);
		}
	
			break;
			case WM_MOUSEHOVER:
				if (!focused)
				{
					focused = true;
					SetFocus(hwnd);
				}
				break;
			case WM_MOUSELEAVE:
			{
				Tracking = false;
				focused = false;
				changeHSV = false;
				startAct = false;
			}
			break;
		case WM_LBUTTONDOWN:
		{
			int x, y;
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			SetFocus(hwnd);
			if (x >= 20 + posColor - 7 && x <= 20 + posColor + 7 && y >= 295 && y <= 309)
			{
				posAct = true;
				//globalColor = squareColor[(y - 10) / 5][(x - 115) / 5];
				//posColor = x - 10;
				//InvalidateRect(hwnd, NULL, TRUE);
			}
			if (x >= 20 && x <= 380 && y >=280 && y<=288)
			{
				posColor = x - 20;
				startAct = true;
				InvalidateRect(hwnd, NULL, TRUE);
			}
			if (x >= 125 && x <= 380 && y >= 10 && y <= 265)
			{
				changeHSV = true;
				s = x - 125;
				v = y - 10;
				//globalColor = squareColor[(y - 10) / 5][(x - 115) / 5];
				InvalidateRect(hwnd, NULL, TRUE);
			}
			if (x >= 80 && x <= 120 && y >= 20 && y <= 60)
			{
				RGBE temp; temp = ToRGB(posColor, s, 255 - v);
				if (temp.R == globalColor.R && temp.G == globalColor.G &&temp.B == globalColor.B)
				{
				}
				else
				{
					colorBuff_selected = 0; // colorBuff_count;
					for (int i = 69; i>0; i--)
					{
						colorBuff[i] = colorBuff[i - 1];
					}

					colorBuff[0].H = posColor;
					colorBuff[0].S = s;
					colorBuff[0].V = v;
					colorBuff[0].e = true;
					if(colorBuff_count<80) colorBuff_count++;
					InvalidateRect(HWND_ContainColorProc, NULL, TRUE);
					globalColor =temp;
					InvalidateRect(hwnd, NULL, TRUE);
				}
			}
			SetCapture(hwnd);
		}
			break;
	
		case WM_MOUSEMOVE:
			if (!Tracking)
			{
				TrackMouse(hwnd);
				Tracking = true;
			}
			int x, y;
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			
				if (x >= 20 && x <= 380 && posAct) {
					startAct = true;  posColor = x - 20; InvalidateRect(hwnd, NULL, TRUE);
				}
				if (changeHSV)
				{
					if (x < 60000)
					{
						s = x - 125;
						if (s > 255) s = 255;
						if (s < 0) s = 0;
					}
					if (y < 60000)
					{
						v = y - 10;
						if (v > 255) v = 255;
						if (v < 0) v = 0;
					}
					
					//globalColor = squareColor[(y - 10) / 5][(x - 115) / 5];
					InvalidateRect(hwnd, NULL, TRUE);
				}

			break;
		case WM_LBUTTONUP:
			changeHSV = false;
			posAct = false;
			globalmouse = 0;
			ReleaseCapture();
			break;
		case WM_RBUTTONUP:
			globalmouse = 0;
			break;
		case WM_KEYDOWN:
			if (wParam == VK_SPACE)
			{
				RGBE temp; temp = ToRGB(posColor, s, 255 - v);
				if (temp.R == globalColor.R && temp.G == globalColor.G &&temp.B == globalColor.B)
				{
				}
				else
				{
					colorBuff_selected = 0; // colorBuff_count;
					for (int i = 69; i>0; i--)
					{
						colorBuff[i] = colorBuff[i - 1];
					}

					colorBuff[0].H = posColor;
					colorBuff[0].S = s;
					colorBuff[0].V = v;
					colorBuff[0].e = true;
					if (colorBuff_count<80) colorBuff_count++;
					InvalidateRect(HWND_ContainColorProc, NULL, TRUE);
					globalColor = temp;
					InvalidateRect(hwnd, NULL, TRUE);
				}
				
			}
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
			break;
		
		}
	
}
