#pragma once
#include <windows.h>

LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegisterChild(HINSTANCE hInst);
