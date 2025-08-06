// JETBanner.cpp : Minimal Banner AppBar with Proper Fill and Font Colors
#include "framework.h"
#include <shellapi.h>
#include <fstream>
#include <sstream>
#include <string>

#define MAX_LOADSTRING 100

// Globals
HINSTANCE hInst;
WCHAR szWindowClass[MAX_LOADSTRING] = L"JETBannerClass";
APPBARDATA abd;
HWND hStatic;
HBRUSH hBrushBanner;

COLORREF bannerColor = RGB(0, 0, 0);
COLORREF fontColor = RGB(255, 255, 255);
std::wstring bannerText = L"";

// Forward declarations
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void LoadConfiguration()
{
    std::wifstream file(L"config.txt");
    if (!file.is_open()) return;

    std::wstring line;
    while (std::getline(file, line)) {
        if (line.find(L"color=") == 0) {
            int r, g, b;
            swscanf_s(line.c_str(), L"color=%d,%d,%d", &r, &g, &b);
            bannerColor = RGB(r, g, b);
        }
        else if (line.find(L"fontcolor=") == 0) {
            int r, g, b;
            swscanf_s(line.c_str(), L"fontcolor=%d,%d,%d", &r, &g, &b);
            fontColor = RGB(r, g, b);
        }
        else if (line.find(L"text=") == 0) {
            bannerText = line.substr(5);
        }
    }
    file.close();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadConfiguration();
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    hBrushBanner = CreateSolidBrush(bannerColor);

    WNDCLASSEXW wcex{};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = hBrushBanner;
    wcex.lpszClassName = szWindowClass;

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int bannerHeight = 30;

    HWND hWnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        szWindowClass,
        bannerText.c_str(),
        WS_POPUP,
        0, 0, screenWidth, bannerHeight,
        nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
        return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    hStatic = CreateWindowExW(0, L"STATIC", bannerText.c_str(),
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        0, 5, screenWidth, 20,
        hWnd, nullptr, hInstance, nullptr);

    // Register AppBar
    ZeroMemory(&abd, sizeof(abd));
    abd.cbSize = sizeof(APPBARDATA);
    abd.hWnd = hWnd;
    abd.uCallbackMessage = WM_USER + 1;
    SHAppBarMessage(ABM_NEW, &abd);

    abd.uEdge = ABE_TOP;
    abd.rc.left = 0;
    abd.rc.top = 0;
    abd.rc.right = screenWidth;
    abd.rc.bottom = bannerHeight;

    SHAppBarMessage(ABM_QUERYPOS, &abd);
    SHAppBarMessage(ABM_SETPOS, &abd);

    MoveWindow(hWnd, abd.rc.left, abd.rc.top,
        abd.rc.right - abd.rc.left,
        abd.rc.bottom - abd.rc.top, TRUE);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        SetTextColor(hdcStatic, fontColor);
        SetBkColor(hdcStatic, bannerColor);
        return (INT_PTR)hBrushBanner;
    }
    case WM_DESTROY:
        SHAppBarMessage(ABM_REMOVE, &abd);
        DeleteObject(hBrushBanner);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
