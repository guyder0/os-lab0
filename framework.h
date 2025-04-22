// header.h: включаемый файл для стандартных системных включаемых файлов
// или включаемые файлы для конкретного проекта
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
// Файлы заголовков Windows
#include <windows.h>
// Файлы заголовков среды выполнения C
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
struct MainElements {
    HWND hPushButton;
    HWND hCheckbox;
    HWND hStatic;
    HWND hRadio1;
    HWND hRadio2;
    HWND hRadio3;
    COLORREF bgcolor;
    wchar_t win_text[256] = L"Вы победили!";
    wchar_t lose_text[256] = L"Вы проиграли.";
};