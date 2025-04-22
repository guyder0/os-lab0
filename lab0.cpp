// lab0.cpp : Определяет 
// точку входа для приложения.
//

#include "framework.h"
#include "lab0.h"
#include "game_window.h"
#include <commdlg.h>

#define MAX_LOADSTRING 100
#define ID_PUSHBUTTON 1001
#define ID_CHECKBOX 1002
#define ID_RADIOBUTTON1 1003
#define ID_RADIOBUTTON2 1004
#define ID_RADIOBUTTON3 1005
#define ID_SCROLL 1006
#define ID_EDIT 1007
#define ID_LISTBOX 1008
#define ID_COMBOBOX 1009
#define POPUP_MENU_COLOR 2001
#define POPUP_MENU_TEXT 2002
#define POPUP_MENU_EXIT 2003

MainElements mainElements;

// Глобальные переменные:
HWND hWndChild;
HWND hWndMain;
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна 
WCHAR szChildClass[MAX_LOADSTRING];             // имя класса окна-ребенка
HBITMAP background;                             // битмап фона
RECT winrect = {0, 0, 0, 0};                    // размер окна (подстроится далее под загружаемый битмап)


// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK  DlgProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB0, szWindowClass, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CNAME, szChildClass, MAX_LOADSTRING);
    // инициализация битмапа фона
    background = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BACKGROUND));
    BITMAP bm;
    GetObject(background, sizeof(BITMAP), &bm);
    winrect.right = bm.bmWidth;
    winrect.bottom = bm.bmHeight;
    AdjustWindowRect(&winrect, WS_OVERLAPPEDWINDOW, TRUE); // Корректирует размер с учётом стиля

    MyRegisterClass(hInstance);
    RegisterChild(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }   

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB0));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB0));
    wcex.hCursor        = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR));
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB0);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VSCROLL,
      CW_USEDEFAULT, 0, winrect.right, winrect.bottom, nullptr, nullptr, hInstance, nullptr);
   winrect.right = 540;
   winrect.bottom = 540;
   AdjustWindowRect(&winrect, WS_OVERLAPPEDWINDOW, FALSE);
   hWndChild = CreateWindowW(szChildClass, szChildClass, WS_CHILD | WS_CAPTION | WS_SYSMENU | WS_POPUP,
       10, 10, winrect.right + 10, winrect.bottom + 35, hWnd, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   mainElements.hPushButton = CreateWindow(L"BUTTON", L"START", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 10, 80, 30, hWnd, (HMENU)ID_PUSHBUTTON, hInstance, NULL);
   mainElements.hCheckbox = CreateWindow(L"BUTTON", L"Нестандартный цвет фона", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 10, 50, 200, 30, hWnd, (HMENU)ID_CHECKBOX, hInstance, NULL);
   mainElements.hStatic = CreateWindow(L"STATIC", L"Размер поля", WS_CHILD | WS_VISIBLE, 220, 10, 100, 20, hWnd, NULL, hInstance, NULL);
   mainElements.hRadio1 = CreateWindow(L"BUTTON", L"3x3", WS_CHILD | WS_VISIBLE | WS_GROUP | BS_AUTORADIOBUTTON, 220, 40, 50, 20, hWnd, (HMENU)ID_RADIOBUTTON1, hInstance, NULL);
   SendMessage(mainElements.hRadio1, BM_SETCHECK, BST_CHECKED, 0);
   mainElements.hRadio2 = CreateWindow(L"BUTTON", L"6x6", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 220, 70, 50, 20, hWnd, (HMENU)ID_RADIOBUTTON2, hInstance, NULL);
   mainElements.hRadio3 = CreateWindow(L"BUTTON", L"9x9", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 220, 100, 50, 20, hWnd, (HMENU)ID_RADIOBUTTON3, hInstance, NULL);

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
            // обработка кнопок меню
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            // обработка кнопок управления
            case ID_PUSHBUTTON:
                ShowWindow(hWndChild, SW_SHOW);
                break;
            case ID_CHECKBOX:
                SendMessage(mainElements.hCheckbox, BM_SETCHECK,
                    (SendMessage(mainElements.hCheckbox, BM_GETCHECK, 0, 0) == BST_CHECKED) ? BST_UNCHECKED : BST_CHECKED, 0);
                break;
            // обработка popup меню
            case POPUP_MENU_COLOR: {
                COLORREF acrCustClr[16] = { 0 };
                CHOOSECOLOR cc = { 0 };
                cc.lStructSize = sizeof(cc);
                cc.hwndOwner = NULL;
                cc.lpCustColors = acrCustClr;
                cc.rgbResult = RGB(255, 0, 0);
                cc.Flags = CC_FULLOPEN | CC_RGBINIT;
                if (ChooseColor(&cc)) mainElements.bgcolor = cc.rgbResult;
            }
                break;
            case POPUP_MENU_TEXT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN_DIALOG), hWnd, DlgProc);
                break;
            case POPUP_MENU_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_VSCROLL: {
        int pos = GetScrollPos(hWnd, SB_VERT);
        switch (LOWORD(wParam))
        {
        case SB_LINEUP:     pos--; break;
        case SB_LINEDOWN:   pos++; break;
        case SB_PAGEUP:     pos -= 5; break;
        case SB_PAGEDOWN:   pos += 5; break;
        case SB_THUMBTRACK: pos = HIWORD(wParam); break;
        }
        SetScrollPos(hWnd, SB_VERT, pos, TRUE);
    }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            HDC hMemDC = CreateCompatibleDC(hdc);
            SelectObject(hMemDC, background);
            BitBlt(hdc, 0, 0, 640, 360, hMemDC, 0, 0, SRCCOPY);
            DeleteDC(hMemDC);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_RBUTTONUP: {
        POINT pt;
        GetCursorPos(&pt);

        HMENU hPopup = CreatePopupMenu();
        AppendMenu(hPopup, MF_STRING, POPUP_MENU_COLOR, L"Выбор цвета фона");
        AppendMenu(hPopup, MF_STRING, POPUP_MENU_TEXT, L"Выбор сообщений выигрыша/проигрыша");
        AppendMenu(hPopup, MF_SEPARATOR, 0, NULL);
        AppendMenu(hPopup, MF_STRING, POPUP_MENU_EXIT, L"Выход");

        TrackPopupMenu(hPopup, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
        DestroyMenu(hPopup);
    }
        break;
    case WM_DESTROY: {
        DeleteObject(background);
        PostQuitMessage(0);
    }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
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

INT_PTR CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    wchar_t list[2][256] = {L"текст 1", L"текст 2"};
    switch (message) {
    case WM_INITDIALOG: {
        SendDlgItemMessage(hWnd, IDC_EDIT, WM_SETTEXT, 256, (LPARAM)mainElements.win_text);

        SendDlgItemMessage(hWnd, IDC_LISTBOX, LB_ADDSTRING, 0, (LPARAM)list[0]);
        SendDlgItemMessage(hWnd, IDC_LISTBOX, LB_ADDSTRING, 0, (LPARAM)list[1]);

        SendDlgItemMessage(hWnd, IDC_COMBOBOX, CB_ADDSTRING, 0, (LPARAM)list[0]);
        SendDlgItemMessage(hWnd, IDC_COMBOBOX, CB_ADDSTRING, 0, (LPARAM)list[1]);
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId) {
        case IDC_LISTBOX: {
            int index = (int)SendDlgItemMessage(hWnd, IDC_LISTBOX, LB_GETCURSEL, 0, 0);
            SendDlgItemMessage(hWnd, IDC_LISTBOX, LB_GETTEXT, index, (LPARAM)mainElements.win_text);
        }
            break;
        case IDC_EDIT:
            if (HIWORD(wParam) == EN_CHANGE) {
                SendDlgItemMessage(hWnd, IDC_EDIT, WM_GETTEXT, 256, (LPARAM)mainElements.win_text);
            }
            break;
        case IDC_COMBOBOX:
            GetDlgItemText(hWnd, IDC_COMBOBOX, mainElements.lose_text, 256);
            break;
        case IDCANCEL: {
            EndDialog(hWnd, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
            break;
        }
    }
        break;
    }
    return (INT_PTR)FALSE;
}