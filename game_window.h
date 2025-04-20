#pragma once
#include "Windows.h"

LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegisterChild(HINSTANCE hInst);