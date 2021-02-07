#include <Windows.h>
#include <string>
#include <tchar.h>

int ScreenWidth, ScreenHeight;
int Interval = 5;

LRESULT CALLBACK Melter(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_CREATE:
	{
		HDC Desktop = GetDC(HWND_DESKTOP);
		HDC Window = GetDC(hWnd);

		BitBlt(Window, 0, 0, ScreenWidth, ScreenHeight, Desktop, 0, 0, SRCCOPY);
		ReleaseDC(hWnd, Window);
		ReleaseDC(HWND_DESKTOP, Desktop);

		SetTimer(hWnd, 0, Interval, 0);
		ShowWindow(hWnd, SW_SHOW);
		break;
	}
	case WM_PAINT:
	{
		ValidateRect(hWnd, 0);
		break;
	}
	case WM_TIMER:
	{
		HDC Window = GetDC(hWnd);
		int X = (rand() % ScreenWidth) - (300 / 2),
			Y = (rand() % 300) - (rand() % 300),
			Width = (rand() % 50),

			Height = (rand() % ScreenHeight);
		BitBlt(Window, X, Y, Width, Height, Window, X, 0, SRCCOPY);
		ReleaseDC(hWnd, Window);
		Beep(rand() % 1000, 50);
		break;
	}
	case WM_DESTROY:
	{
		KillTimer(hWnd, 0);
		PostQuitMessage(0);
		break;
	}
	return 0;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT CALLBACK HackedBy(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (Msg)
	{
	case WM_CREATE:
	{
		HDC Desktop = GetDC(HWND_DESKTOP);
		HDC Window = GetDC(hWnd);

		BitBlt(Window, 0, 0, ScreenWidth, ScreenHeight, Desktop, 0, 0, SRCCOPY);
		ReleaseDC(hWnd, Window);
		ReleaseDC(HWND_DESKTOP, Desktop);
		ShowWindow(hWnd, SW_SHOW);
		break;
	}
	case WM_PAINT:
	{
		RECT rect;
		HBRUSH hBrush;
		HFONT hFont;

		hdc = BeginPaint(hWnd, &ps);
		SetBkMode(hdc, 1);
		hFont = CreateFont(256, 0, 0, 450, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
			CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial Black"));
		SelectObject(hdc, hFont);
		SetRect(&rect, 0, 0, ScreenWidth, ScreenHeight);
		SetTextColor(hdc, RGB(0, 255, 0));

		DrawText(hdc, TEXT("Hacked by II Bg"), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		DeleteObject(hFont);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_TIMER:
	{
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	return 0;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE Inst, HINSTANCE Prev, LPSTR Cmd, int showcmd)
{
	// Get the width & height of current display
	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	Beep(2500, 750);

	WNDCLASS wndClass = { 0, Melter, 0,0, Inst, 0, LoadCursorW(0, IDC_WAIT), 0, 0, L"ScreenMelter" };
	if (RegisterClass(&wndClass))
	{
		// Create the "melter" overlapping window.
		HWND hWnd = CreateWindowExA(WS_EX_TOPMOST, "ScreenMelter", 0, WS_POPUP,
			0, 0, ScreenWidth, ScreenHeight, HWND_DESKTOP, 0, Inst, 0);
		Beep(2000, 250);
		Beep(4000, 350);
		if (hWnd)
		{
			// seed for randomization
			srand(GetTickCount64());
			MSG Msg = { 0 };
			ULONGLONG startTime = GetTickCount64();
			// Run the melter loop
			while (Msg.message != WM_QUIT && GetTickCount64() < startTime + 5000)
			{
				if (PeekMessage(&Msg, 0, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&Msg);
					DispatchMessage(&Msg);
				}
			}
		}
	}

	WNDCLASS wndClass2 = { 0, HackedBy, 0,0, Inst, 0, LoadCursorW(0, IDC_ARROW), 0, 0, L"HackedBy" };
	if (RegisterClass(&wndClass2))
	{
		// Create the "melter" overlapping window.
		HWND hWnd = CreateWindowExA(WS_EX_TOPMOST, "HackedBy", 0, WS_POPUP,
			0, 0, ScreenWidth, ScreenHeight, HWND_DESKTOP, 0, Inst, 0);
		if (hWnd)
		{
			// seed for randomization
			srand(GetTickCount64());
			MSG Msg = { 0 };
			ULONGLONG startTime = GetTickCount64();
			// Run the melter loop
			while (Msg.message != WM_QUIT && GetTickCount64() < startTime + 10000)
			{
				if (PeekMessage(&Msg, 0, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&Msg);
					DispatchMessage(&Msg);
				}
			}
		}
	}

	return 0;
}