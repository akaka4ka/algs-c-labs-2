#include "resource.h"
#include "framework.h"
#include "code.h"
#include <SDKDDKVer.h>

#define LINEWIDTH 5
#define MAX_LOADSTRING 100

TRACE trace;
LINE line;

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Code();

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TASK5, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TASK5));

    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TASK5));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TASK5);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, WINSIZE, WINSIZE, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            SetTimer(hWnd, 1, 100, NULL);
        }
    case WM_TIMER:
        {
            TraceCalculation();
            InvalidateRect(hWnd, NULL, FALSE);
        }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            int i = 0;
            int colorStep = 16 / trace.traceLength;
            int colorIndex = 15;

            COLORREF color = 0xffff00;

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            HPEN hLinePen, hTracePen;
            HBRUSH hBgBrush;
            
            if ((line.xPos <= 5) && line.dx == -1)
            {
                line.dx = 0;
            }
            if ((line.xPos + line.length >= WINSIZE - 5) && line.dx == 1)
            {
                line.dx = 0;
            }
            if ((line.yPos - line.length <= 5) && line.dy == -1)
            {
                line.dy = 0;
            }
            if ((line.yPos >= WINSIZE - 5) && line.dy == 1)
            {
                line.dy = 0;
            }
            
            line.xPos += line.dx * line.velocity;
            line.yPos += line.dy * line.velocity;

            hBgBrush = CreateSolidBrush(RGB(0, 0, 0));
            hLinePen = CreatePen(PS_SOLID, LINEWIDTH, RGB(255, 255, 0));

            SelectObject(hdc, hBgBrush);
            Rectangle(hdc, 0, 0, WINSIZE, WINSIZE);
            
            for (i = 0; i < trace.traceLength; i++)
            {
                colorIndex -= colorStep;
                   
                color = color & 0x000f0f;
                color = color | ((colorIndex << (4 * 1)) | (colorIndex << (4 * 3)));

                hTracePen = CreatePen(PS_SOLID, LINEWIDTH, color);

                SelectObject(hdc, hTracePen);
                MoveToEx(hdc, trace.xTracePos[i], trace.yTracePos[i], NULL);
                LineTo(hdc, trace.xTracePos[i] + line.length, trace.yTracePos[i] - line.length);

                DeleteObject(hTracePen);
            }

            SelectObject(hdc, hLinePen);
            MoveToEx(hdc, line.xPos, line.yPos, NULL);
            LineTo(hdc, line.xPos + line.length, line.yPos - line.length);

            DeleteObject(hBgBrush);
            DeleteObject(hLinePen);
            EndPaint(hWnd, &ps);

            line.dx = 0;
            line.dy = 0;
        }
        break;
    case WM_KEYDOWN:
        {
            if (wParam == VK_LEFT)
            {
                line.dx = -1;
            }
            if (wParam == VK_RIGHT)
            {
                line.dx = 1;
            }
            if (wParam == VK_UP)
            {
                line.dy = -1;

            }
            if (wParam == VK_DOWN)
            {
                line.dy = +1;
            }
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
