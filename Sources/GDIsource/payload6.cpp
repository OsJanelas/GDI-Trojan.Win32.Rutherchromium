#include <windows.h>

// --- Efeito 1: Elipse Saltitante (DVD Style) ---
DWORD WINAPI BouncingEllipse(LPVOID lpParam) {
    int sw = GetSystemMetrics(0);
    int sh = GetSystemMetrics(1);

    int x = 10, y = 10;
    int signX = 1, signY = 1;
    int incrementor = 10;

    while (1) {
        HDC hdc = GetDC(0);

        x += incrementor * signX;
        y += incrementor * signY;

        // Dimensões da elipse baseadas no seu código (5x300 - fica parecendo um risco vertical)
        int top_x = x;
        int top_y = y;
        int bottom_x = 5 + x;
        int bottom_y = 300 + y;

        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);

        Ellipse(hdc, top_x, top_y, bottom_x, bottom_y);

        // Lógica de colisão com as bordas da tela
        if (y >= sh) signY = -1;
        if (x >= sw) signX = -1;
        if (y <= 0)  signY = 1;
        if (x <= 0)  signX = 1;

        DeleteObject(brush);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return 0;
}

// --- Efeito 2: BitBlt Glitch (SRCPAINT) ---
DWORD WINAPI BitBltGlitch(LPVOID lpParam) {
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);

    while (1) {
        HDC hdc = GetDC(0);

        // Desloca a tela levemente e mistura as cores (SRCPAINT)
        // Isso cria um efeito de rastro e acúmulo de cores
        BitBlt(hdc, rand() % 10, rand() % 10, w, h, hdc, rand() % 12, rand() % 12, SRCPAINT);

        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return 0;
}

int main() {
    // Esconde a janela do console para o efeito ocupar a tela toda
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    // Cria as duas threads para rodar os códigos ao mesmo tempo
    HANDLE t1 = CreateThread(NULL, 0, BouncingEllipse, NULL, 0, NULL);
    HANDLE t2 = CreateThread(NULL, 0, BitBltGlitch, NULL, 0, NULL);

    // Mantém o programa rodando
    if (t1 && t2) {
        WaitForSingleObject(t1, INFINITE);
    }

    return 0;
}