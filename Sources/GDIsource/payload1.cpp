#include <windows.h>
#include <iostream>

#pragma comment(lib, "Msimg32.lib")

int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);

// PAYLOAD1BLUR
DWORD WINAPI BlurEffect(LPVOID lpvd) {
    HDC dc = GetDC(NULL);
    HDC dcCopy = CreateCompatibleDC(dc);
    HBITMAP bmp = CreateCompatibleBitmap(dc, w, h);
    SelectObject(dcCopy, bmp);

    BLENDFUNCTION blur = { AC_SRC_OVER, 0, 10, 0 };

    while (true) {
        StretchBlt(dcCopy, rand() % 10, rand() % 10, w, h, dc, rand() % -10, rand() % -10, w, h, SRCCOPY);
        AlphaBlend(dc, 0, 0, w, h, dcCopy, 0, 0, w, h, blur);
        if (rand() % 10 == 0) {
            Sleep(5);
            InvalidateRect(NULL, NULL, TRUE);
        }
    }
    return 0;
}

// PAYLOAD1SHAKE
DWORD WINAPI ShakeEffect(LPVOID lpvd) {
    while (true) {
        HDC hdc = GetDC(0);
        BitBlt(hdc, rand() % 5, rand() % 5, w, h, hdc, rand() % 5, rand() % 5, SRCCOPY);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return 0;
}

// PAYLOAD1INVERT
DWORD WINAPI InvertEffect(LPVOID lpvd) {
    while (true) {
        HDC hdc = GetDC(0);
        PatBlt(hdc, 0, 0, w, h, PATINVERT);
        ReleaseDC(0, hdc);
        Sleep(100);
    }
    return 0;
}

int main() {
    std::cout << "" << std::endl;

    CreateThread(NULL, 0, BlurEffect, NULL, 0, NULL);
    std::cout << "" << std::endl;

    Sleep(15000);

    CreateThread(NULL, 0, ShakeEffect, NULL, 0, NULL);
    std::cout << "" << std::endl;

    CreateThread(NULL, 0, InvertEffect, NULL, 0, NULL);
    std::cout << "" << std::endl;

    while (true) {
        Sleep(1000);
    }

    return 0;
}