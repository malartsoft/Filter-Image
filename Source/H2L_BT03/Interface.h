//#include "stdafx.h"
#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include "Resource.h"
//#include "H2L_BT03.h"
using namespace std;
#pragma comment(lib, "gdiplus.lib")

int countSave ;
class Interface{
	Gdiplus::GdiplusStartupInput	gdiplusStartupInput;
	ULONG_PTR						gdiplusToken;
	Gdiplus::Graphics*				runGraphics;
	HWND							AppHandle;
	HFONT hFont;
	HWND							browse;
	HWND							filters;
	HWND							lowPass;
	HWND							mask;
	HWND							median;
	int								nMask;
	wstring path;
	wstring pathFilter;
public :

	//set handle
	void SetAppHandle(HWND handle)
	{
		AppHandle = handle;
	}
	void GDIP_Init()
	{
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		runGraphics = new Gdiplus::Graphics(GetDC(AppHandle));
	}

	//drawpicbefore filter
	void DrawPic(wstring path){
		if (path.size() == 0)
			return;
		Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(path.c_str(), false);
		runGraphics->DrawImage(bitmap, 10, 100, 400, 270);
	}

	//drawpic after filter
	void DrawPicAfter(){
		if (path.size() == 0)
			return;
		string save = "bm" + to_string(countSave-1) + ".bmp";
		wstring re(save.length(), L' ');
		copy(save.begin(), save.end(), re.begin());

		Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(re.c_str(), false);
		
		runGraphics->DrawImage(bitmap, 520, 100, 400, 270);
		

	}

	//init interface in WM_Create
	void InitInterface(){
		LOGFONT lf;
		GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
		 hFont = CreateFont(19, lf.lfWidth,
			lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
			lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
			lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
			lf.lfPitchAndFamily, lf.lfFaceName);
		browse = CreateWindowEx(0, L"Button", L"Browse", WS_CHILD | WS_VISIBLE | BS_FLAT, 50, 30, 80, 40, AppHandle, (HMENU)IDC_BUTTON_BROWSE, 0, 0);
		 SendMessage(browse, WM_SETFONT, WPARAM(hFont), TRUE);

		 CreateWindowEx(0, L"static", L"", WS_CHILD | WS_VISIBLE | BS_FLAT|WS_BORDER, 10, 100, 400, 270, AppHandle, 0, 0, 0);
		 CreateWindowEx(0, L"static", L"", WS_CHILD | WS_VISIBLE | BS_FLAT | WS_BORDER, 520, 100, 400, 270, AppHandle, 0, 0, 0);

		 filters = CreateWindowEx(0, L"Button", L"Filters", WS_CHILD | WS_VISIBLE | BS_FLAT, 425, 100, 80, 40, AppHandle,(HMENU) IDC_BUTTON_FILTER, 0, 0);
		SendMessage(filters, WM_SETFONT, WPARAM(hFont), TRUE);

		lowPass = CreateWindowEx(0, L"Button", L"Lowpass", WS_CHILD | WS_VISIBLE | BS_FLAT, 425, 170, 80, 40, AppHandle, (HMENU)IDC_BUTTON_LOWPASS, 0, 0);
		SendMessage(lowPass, WM_SETFONT, WPARAM(hFont), TRUE);

		HWND sMask = CreateWindowEx(0, L"static", L"Mask", WS_CHILD | WS_VISIBLE | BS_FLAT, 425, 250, 40, 40, AppHandle, 0, 0, 0);
		SendMessage(sMask, WM_SETFONT, WPARAM(hFont), TRUE);

		mask = CreateWindowEx(0, L"Edit", L"", WS_CHILD | WS_VISIBLE | BS_FLAT|WS_BORDER, 470, 247, 40, 25, AppHandle, 0, 0, 0);
		SendMessage(mask, WM_SETFONT, WPARAM(hFont), TRUE);

		median = CreateWindowEx(0, L"Button", L"Median", WS_CHILD | WS_VISIBLE | BS_FLAT, 425, 310, 80, 40, AppHandle, (HMENU)IDC_BUTTON_MEDIAN, 0, 0);
		SendMessage(median, WM_SETFONT, WPARAM(hFont), TRUE);
	}

	//interface openfile
	wstring OpenFile(  HWND hWnd,int style)
	{

		OPENFILENAME ofn;
		WCHAR szFile[256];
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.nMaxFile = 256;
		// Chú ý chuỗi bên dưới double zero terminated!
		if (style==1)
			ofn.lpstrFilter = L"Image(*.png, *.jpg, *.bmp) \0*.png; *.jpg; *.bmp\0";
		else
			ofn.lpstrFilter = L"Text(*.txt) \0*.txt\0";
		ofn.nFilterIndex = 1; // Text | *.txt
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		if (GetOpenFileName(&ofn) == TRUE) {
			return szFile;
		}
		else return L"";
	}
	
	//process button browse
	void ActionBrowse(){
		path=OpenFile(AppHandle,1);
		if (path.size() == 0)
			return;
		HWND hPath=CreateWindowEx(0, L"Static", path.c_str(), WS_CHILD | WS_VISIBLE, 150, 35, 850, 50, AppHandle, 0, 0, 0);
		SendMessage(hPath, WM_SETFONT, WPARAM(hFont), TRUE);
		DrawPic(path);
	}

	//process button filter
	void ActionFilter(){
		pathFilter = OpenFile(AppHandle,2);
		if (path.size() == 0)
			return;
		DrawPic(path);
	}

	//check is char
	bool IsChar(WCHAR *s) {
		int i = 0;
		while (s[i] != '\0') {
			if (s[i] > 57 || s[i] < 48)
				return 1;
			i++;
		}
		return 0;
	}

	//process button median
	void ActionMedian(){
		WCHAR * buffer = new WCHAR[36];
		int size;
		size= GetWindowTextLength(mask);
		GetWindowText(mask, buffer, size + 1);
		if (buffer[0] == '-') {
			MessageBox(0, L"Don't input number smaller 0 - Try again", L"Error", 0);
			return ;
		}
		if (IsChar(buffer)) {
			MessageBox(0, L"Exist char in textbox - Try again", L"Error", 0);
			return ;
		}
		if (buffer[0] == '\0') {
			MessageBox(0, L"Can't be empty - Try again", L"Error", 0);
			return;
		}
		nMask = _wtoi(buffer);
	}

	//get attitute
	int Get_N_Mask(){
		return nMask;
	}

	//get path
	wstring GetPath(){
		return path;
	}

	//get path filter
	wstring GetPathFilter(){
		return pathFilter;
	}
};