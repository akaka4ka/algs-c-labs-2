#include <math.h>
#include "framework.h"
#include "resource.h"

#define MAX_LOADSTRING 100
#define WINSIZE 600
#define FAILURE 0
#define SUCCESS 1
#define MAXCOORDINATE 8
#define SQUARESIDE 75
#define PENWIDTH 5

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
int xKnight = 0;
int yKnight = 0;
int xDot = 0;
int yDot = 0;
int xMidDot = 0;
int yMidDot = 0;
int isKnight = 0;
int isDot = 1;
int Pieces = 0;
int isSucceed = 0;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int MoveCheck(int xPiece, int yPiece, int xDot, int yDot)
{
    if (pow(xPiece - xDot, 2) + pow(yPiece - yDot, 2) == 5)
    {
        return 1;
    }
    return 0;
}

int HorSideCheck(int xPiece, int xDot)
{
    if (xDot - xPiece > 0)
    {
        return 1;
    }
    if (xDot - xPiece == 0)
    {
        return 0;
    }
    if (xDot - xPiece < 0)
    {
        return -1;
    }
}

int VerSideCheck(int yPiece, int yDot)
{
    if (yDot - yPiece > 0)
    {
        return 1;
    }
    if (yDot - yPiece == 0)
    {
        return 0;
    }
    if (yDot - yPiece < 0)
    {
        return -1;
    }
}

int Algorithm(int xKnight, int yKnight, int xDot, int yDot)
{
    int onHorSide = 0;
    int onVerSide = 0;

    if (xKnight == xDot && yKnight == yDot)
    {
        return FAILURE;
    }
    if (MoveCheck(xKnight, yKnight, xDot, yDot))
    {
        return SUCCESS;
    }
    else
    {
        onHorSide = HorSideCheck(xKnight, xDot);
        onVerSide = VerSideCheck(yKnight, yDot);

        if (onHorSide == 0)
        {
            xKnight += 1;
            yKnight += 2 * onVerSide;

            if (MoveCheck(xKnight, yKnight, xDot, yDot))
            {
                xMidDot = xKnight;
                yMidDot = yKnight;
                xKnight -= 1;
                yKnight -= 2 * onVerSide;
            }
            else
            {
                xKnight += 1;
                yKnight -= 1 * onVerSide;

                if (MoveCheck(xKnight, yKnight, xDot, yDot))
                {
                    xMidDot = xKnight;
                    yMidDot = yKnight;
                    xKnight -= 2;
                    yKnight -= 1 * onVerSide;
                }
                else
                {
                    return FAILURE;
                }
            }
        }

        if (onVerSide == 0)
        {
            xKnight += 2 * onHorSide;
            yKnight += 1;

            if (MoveCheck(xKnight, yKnight, xDot, yDot))
            {
                xMidDot = xKnight;
                yMidDot = yKnight;
                xKnight -= 2 * onHorSide;
                yKnight -= 1;
            }
            else
            {
                xKnight -= 1 * onHorSide;
                yKnight += 1;

                if (MoveCheck(xKnight, yKnight, xDot, yDot))
                {
                    xMidDot = xKnight;
                    yMidDot = yKnight;
                    xKnight -= 1 * onHorSide;
                    yKnight -= 2;
                }
                else
                {
                    return FAILURE;
                }
            }
        }

        if (onVerSide != 0 && onHorSide != 0)
        {
            xKnight += 1 * onHorSide;
            yKnight += 2 * onVerSide;
            if (MoveCheck(xKnight, yKnight, xDot, yDot))
            {
                xMidDot = xKnight;
                yMidDot = yKnight;
                xKnight -= 1 * onHorSide;
                yKnight -= 2 * onVerSide;
            }
            else
            {
                xKnight -= 1 * onHorSide;
                yKnight -= 2 * onVerSide;

                xKnight += 2 * onHorSide;
                yKnight += 1 * onVerSide;
                if (MoveCheck(xKnight, yKnight, xDot, yDot))
                {
                    xMidDot = xKnight;
                    yMidDot = yKnight;
                    xKnight -= 2 * onHorSide;
                    yKnight -= 1 * onVerSide;
                }
                else
                {
                    return FAILURE;
                }
            }
        }
    }
    if (yMidDot > 8)
    {
        yMidDot = 2 * yDot - yMidDot;
    }
    if (xMidDot > 8)
    {
        xMidDot = 2 * xDot - xMidDot;
    }
    return SUCCESS;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TASK4WINAPI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TASK4WINAPI));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TASK4WINAPI));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TASK4WINAPI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, WINSIZE, WINSIZE, NULL, NULL, hInstance, NULL);

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
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            HPEN hRedPath, hGreenPath, hBluePath;
            HBRUSH hBlackBrush, hWhiteBrush, hKnightBrush, hDotBrush, hMidDotBrush;
            int isWhite = 1;
            int verCount = 0;
            int horCount = 0;


            hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
            hWhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
            while (verCount != MAXCOORDINATE - 1 || horCount != MAXCOORDINATE)
            {
                if (horCount == MAXCOORDINATE)
                {
                    verCount++;
                    horCount = 0;
                    if (isWhite)
                    {
                        isWhite = 0;
                    }
                    else
                    {
                        isWhite = 1;
                    }
                }
                if (isWhite)
                {
                    SelectObject(hdc, hWhiteBrush);
                    isWhite--;
                }
                else
                {
                    SelectObject(hdc, hBlackBrush);
                    isWhite++;
                }

                Rectangle(hdc, SQUARESIDE * horCount, SQUARESIDE * verCount, SQUARESIDE * (horCount + 1), SQUARESIDE * (verCount + 1));
                horCount++;
            }

            hMidDotBrush = CreateSolidBrush(RGB(255, 0, 0));
            hKnightBrush = CreateSolidBrush(RGB(0, 255, 0));
            hDotBrush = CreateSolidBrush(RGB(0, 0, 255));
            if (Pieces == 2)
            {
                isSucceed = Algorithm((xKnight - (xKnight % SQUARESIDE)) / SQUARESIDE + 1, (yKnight - (yKnight % SQUARESIDE)) / SQUARESIDE + 1, (xDot - (xDot % SQUARESIDE)) / SQUARESIDE + 1, (yDot - (yDot % SQUARESIDE)) / SQUARESIDE + 1);
            }
            if (isKnight)
            {
                SelectObject(hdc, hKnightBrush);
                Ellipse(hdc, xKnight - SQUARESIDE / 2, yKnight - SQUARESIDE / 2, xKnight + SQUARESIDE / 2, yKnight + SQUARESIDE / 2);
            }
            if (isKnight && isDot)
            {
                SelectObject(hdc, hDotBrush);
                Ellipse(hdc, xDot - SQUARESIDE / 2, yDot - SQUARESIDE / 2, xDot + SQUARESIDE / 2, yDot + SQUARESIDE / 2);
            }
            if (xMidDot != 0 && yMidDot != 0)
            {
                SelectObject(hdc, hMidDotBrush);
                Ellipse(hdc, (xMidDot - 1) * SQUARESIDE, (yMidDot - 1) * SQUARESIDE, xMidDot * SQUARESIDE, yMidDot * SQUARESIDE);
            }

            hRedPath = CreatePen(PS_SOLID, PENWIDTH, RGB(255, 0, 0));
            hGreenPath = CreatePen(PS_SOLID, PENWIDTH, RGB(0, 255, 0));
            hBluePath = CreatePen(PS_SOLID, PENWIDTH, RGB(0, 0, 255));
            if (isSucceed)
            {   
                if (xMidDot != 0 && yMidDot != 0)
                {
                    SelectObject(hdc, hBluePath);
                    LineTo(hdc, xMidDot * SQUARESIDE - SQUARESIDE / 2, yMidDot * SQUARESIDE - SQUARESIDE / 2);
                    SelectObject(hdc, hGreenPath);
                    LineTo(hdc, xDot, yDot);
                }
                else
                {
                    SelectObject(hdc, hRedPath);
                    MoveToEx(hdc, xKnight, yKnight, NULL);
                    LineTo(hdc, xDot, yDot);
                }
            }
            else
            {
                if (Pieces == 2)
                {
                    SelectObject(hdc, hRedPath);
                    MoveToEx(hdc, xKnight - SQUARESIDE / 2, yKnight - SQUARESIDE / 2, NULL);
                    LineTo(hdc, xKnight + SQUARESIDE / 2, yKnight + SQUARESIDE / 2);
                    MoveToEx(hdc, xKnight + SQUARESIDE / 2, yKnight - SQUARESIDE / 2, NULL);
                    LineTo(hdc, xKnight - SQUARESIDE / 2, yKnight + SQUARESIDE / 2);
                }
                if (Pieces == 2)
                {
                    SelectObject(hdc, hRedPath);
                    MoveToEx(hdc, xDot - SQUARESIDE / 2, yDot - SQUARESIDE / 2, NULL);
                    LineTo(hdc, xDot + SQUARESIDE / 2, yDot + SQUARESIDE / 2);
                    MoveToEx(hdc, xDot + SQUARESIDE / 2, yDot - SQUARESIDE / 2, NULL);
                    LineTo(hdc, xDot - SQUARESIDE / 2, yDot + SQUARESIDE / 2);
                }
            }

            

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_LBUTTONDOWN:
        {
            if (!isDot)
            {
                xDot = SQUARESIDE / 2 - (LOWORD(lParam) % SQUARESIDE) + LOWORD(lParam);
                yDot = SQUARESIDE / 2 - (HIWORD(lParam) % SQUARESIDE) + HIWORD(lParam);
                isDot++;
                Pieces++;
            }
            if (!isKnight)
            {
                xKnight = SQUARESIDE / 2 - (LOWORD(lParam) % SQUARESIDE) + LOWORD(lParam);
                yKnight = SQUARESIDE / 2 - (HIWORD(lParam) % SQUARESIDE) + HIWORD(lParam);
                isKnight++;
                isDot = 0;
                Pieces++;
            }
            InvalidateRect(hWnd, NULL, FALSE);
            return 0;
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
