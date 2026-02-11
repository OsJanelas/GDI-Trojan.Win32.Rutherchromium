#include <windows.h>
#define _USE_MATH_DEFINES 1
#include <cmath>

// Variáveis globais para os efeitos
int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);

// --- Efeito 1: Ondas Senoidais ---
DWORD WINAPI sines(LPVOID lpParam) {
    HWND wnd = GetDesktopWindow();
    double angle = 0;
    while (1) {
        HDC desk = GetDC(0);
        for (float i = 0; i < h; i += 0.99f) {
            // Cria o deslocamento baseado no seno
            int a = (int)(sin(angle) * 20);
            BitBlt(desk, 0, (int)i, w, 1, desk, a, (int)i, SRCCOPY);
            angle += M_PI / 40;
        }
        ReleaseDC(wnd, desk);
        // Pequeno delay para não congelar o sistema totalmente
        Sleep(10);
    }
    return 0;
}

// --- Efeito 2: Trepidação (Shake) ---
DWORD WINAPI ScreenShake(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        // Move a tela aleatoriamente em um range de 10 pixels
        BitBlt(hdc, rand() % 10, rand() % 10, w, h, hdc, rand() % 10, rand() % 10, SRCCOPY);
        ReleaseDC(0, hdc);
        Sleep(15);
    }
    return 0;
}

// --- Efeito 3: Ruído e Cores (Thing6) ---
DWORD WINAPI thing6(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP bm = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hdcMem, bm);

        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);

        // O código de operação 0x123456 cria padrões de bits imprevisíveis
        BitBlt(hdc, rand() % 2, rand() % 2, w, h, hdcMem, rand() % 2, rand() % 2, 0x123456);

        DeleteObject(brush);
        DeleteObject(bm);
        DeleteDC(hdcMem);
        ReleaseDC(0, hdc);
        Sleep(1);
    }
    return 0;
}

// --- Função Principal ---
int main() {
    // Esconde o console
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    // Inicia os três efeitos simultaneamente
    HANDLE t1 = CreateThread(0, 0, sines, 0, 0, 0);
    HANDLE t2 = CreateThread(0, 0, ScreenShake, 0, 0, 0);
    HANDLE t3 = CreateThread(0, 0, thing6, 0, 0, 0);

    // Mantém o programa rodando
    if (t1 && t2 && t3) {
        HANDLE threads[3] = { t1, t2, t3 };
        WaitForMultipleObjects(3, threads, TRUE, INFINITE);
    }

    return 0;
}