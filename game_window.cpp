#include "game_window.h"
#define UNICODE
#define _UNICODE
#include <vector>
#include "framework.h"
#include "Resource.h"

int GRID_SIZE = 5;
int CELL_SIZE = 80;

POINT startPoint = { 0, 0 };
POINT endPoint = { GRID_SIZE - 1, GRID_SIZE - 1 };

std::vector<POINT> path;
bool drawing = false;

extern MainElements mainElements;
COLORREF bgcolor;

// === Утилиты ===
POINT GridFromPos(int x, int y) {
    return { x / CELL_SIZE, y / CELL_SIZE };
}

bool IsSamePoint(const POINT& a, const POINT& b) {
    return a.x == b.x && a.y == b.y;
}

bool IsInBounds(const POINT& pt) {
    return pt.x >= 0 && pt.x < GRID_SIZE && pt.y >= 0 && pt.y < GRID_SIZE;
}

bool IsInPath(const POINT& pt) {
    for (const POINT& p : path)
        if (IsSamePoint(p, pt))
            return true;
    return false;
}

// === Основная оконная процедура ===
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_SHOWWINDOW: // обрабатываем когда окно показывается
        if (wParam == TRUE) {
            if (SendMessage(mainElements.hRadio1, BM_GETCHECK, 0, 0) == BST_CHECKED) GRID_SIZE = 3;
            if (SendMessage(mainElements.hRadio2, BM_GETCHECK, 0, 0) == BST_CHECKED) GRID_SIZE = 6;
            if (SendMessage(mainElements.hRadio3, BM_GETCHECK, 0, 0) == BST_CHECKED) GRID_SIZE = 9;
            endPoint = { GRID_SIZE - 1, GRID_SIZE - 1 };
            CELL_SIZE = 540 / GRID_SIZE;
            if (SendMessage(mainElements.hCheckbox, BM_GETCHECK, 0, 0) == BST_CHECKED) bgcolor = mainElements.bgcolor;
            else bgcolor = RGB(255, 255, 255);
        }
        else {
            path.clear();
        }
        break;
    case WM_ERASEBKGND:
    {
        HDC hdc = (HDC)wParam;
        RECT rect;
        GetClientRect(hwnd, &rect);

        HBRUSH hBrush = CreateSolidBrush(bgcolor);
        FillRect(hdc, &rect, hBrush);
        DeleteObject(hBrush);

        break;
    }
    case WM_LBUTTONDOWN: {
        POINT pt = GridFromPos(LOWORD(lParam), HIWORD(lParam));
        if (IsSamePoint(pt, startPoint)) {
            path.clear();
            path.push_back(pt);
            drawing = true;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        break;
    }

    case WM_MOUSEMOVE: {
        if (drawing && (wParam & MK_LBUTTON)) {
            POINT pt = GridFromPos(LOWORD(lParam), HIWORD(lParam));
            POINT last = path.back();

            int dx = abs(pt.x - last.x);
            int dy = abs(pt.y - last.y);

            // Соседняя клетка и не повтор
            if (IsInBounds(pt) && (dx + dy == 1) && !IsInPath(pt)) {
                path.push_back(pt);
                InvalidateRect(hwnd, NULL, TRUE);
            }
        }
        break;
    }

    case WM_LBUTTONUP: {
        if (drawing) {
            drawing = false;
            if (IsSamePoint(path.back(), endPoint)) {
                MessageBox(hwnd, mainElements.win_text, L"Итог", MB_OK);
            }
            else {
                MessageBox(hwnd, mainElements.lose_text, L"Итог", MB_ICONERROR);
            }
        }
        break;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Сетка
        /*for (int y = 0; y < GRID_SIZE; ++y) {
            for (int x = 0; x < GRID_SIZE; ++x) {
                Rectangle(hdc,
                    x * CELL_SIZE, y * CELL_SIZE,
                    (x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE);
            }
        }*/
        for (int y = 0; y < GRID_SIZE; ++y) {
            MoveToEx(hdc, CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2, NULL);
            for (int x = 0; x < GRID_SIZE; ++x) {
                LineTo(hdc, x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2);
            }
        }
        for (int x = 0; x < GRID_SIZE; ++x) {
            MoveToEx(hdc, x * CELL_SIZE + CELL_SIZE / 2, CELL_SIZE / 2, NULL);
            for (int y = 0; y < GRID_SIZE; ++y) {
                LineTo(hdc, x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2);
            }
        }

        // Старт (зелёный) и финиш (красный)
        HBRUSH green = CreateSolidBrush(RGB(0, 255, 0));
        HBRUSH red = CreateSolidBrush(RGB(255, 0, 0));

        RECT r1 = { startPoint.x * CELL_SIZE + CELL_SIZE / 3, startPoint.y * CELL_SIZE + CELL_SIZE / 3,
                    (startPoint.x + 1) * CELL_SIZE - CELL_SIZE / 3, (startPoint.y + 1) * CELL_SIZE - CELL_SIZE / 3 };
        FillRect(hdc, &r1, green);

        RECT r2 = { endPoint.x * CELL_SIZE + CELL_SIZE / 3, endPoint.y * CELL_SIZE + CELL_SIZE / 3,
                    (endPoint.x + 1) * CELL_SIZE - CELL_SIZE / 3, (endPoint.y + 1) * CELL_SIZE - CELL_SIZE / 3 };
        FillRect(hdc, &r2, red);

        HPEN hPen = CreatePen(PS_SOLID, CELL_SIZE / 6, RGB(127, 255, 212));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        // Путь
        if (!path.empty()) {
            MoveToEx(hdc, path[0].x * CELL_SIZE + CELL_SIZE / 2, path[0].y * CELL_SIZE + CELL_SIZE / 2, NULL);
            for (size_t i = 1; i < path.size(); ++i) {
                LineTo(hdc, path[i].x * CELL_SIZE + CELL_SIZE / 2, path[i].y * CELL_SIZE + CELL_SIZE / 2);
            }
        }

        SelectObject(hdc, hOldPen);

        DeleteObject(green);
        DeleteObject(red);
        DeleteObject(hPen);
        EndPaint(hwnd, &ps);
        break;
    }
               
    case WM_CLOSE:
        ShowWindow(hwnd, SW_HIDE);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    return 0;
    }

}

ATOM RegisterChild(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;
    WCHAR szChildClass[100];
    LoadStringW(hInstance, IDC_CNAME, szChildClass, 100);

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = ChildWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB0));
    wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR));
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = szChildClass;
    wcex.lpszClassName = szChildClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}