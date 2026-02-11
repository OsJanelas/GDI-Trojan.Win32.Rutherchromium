#include <windows.h>

// Variáveis globais para dimensões da tela
int sw = GetSystemMetrics(0);
int sh = GetSystemMetrics(1);

// --- Efeito 1: Curvas de Bézier Verdes ---
DWORD WINAPI BezierPayload(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        // Define 4 pontos aleatórios para a curva
        POINT p[4] = {
            {rand() % sw, rand() % sh},
            {rand() % sw, rand() % sh},
            {rand() % sw, rand() % sh},
            {rand() % sw, rand() % sh}
        };

        HPEN hPen = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
        SelectObject(hdc, hPen);

        PolyBezier(hdc, p, 4);

        DeleteObject(hPen);
        ReleaseDC(0, hdc);
        Sleep(10); // Pequena pausa para não travar o processador
    }
    return 0;
}

// --- Efeito 2: Deslocamento Horizontal (BitBlt) ---
DWORD WINAPI HorizontalShift(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        // Move a tela para a esquerda e preenche a borda
        BitBlt(hdc, -30, 0, sw, sh, hdc, 0, 0, SRCCOPY);
        BitBlt(hdc, sw - 30, 0, sw, sh, hdc, 0, 0, SRCCOPY);

        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return 0;
}

// --- Efeito 3: Zoom Pulsante (StretchBlt) ---
DWORD WINAPI ZoomPayload(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        // Efeito de esticar e encolher a tela simultaneamente
        StretchBlt(hdc, -10, -10, sw + 20, sh + 20, hdc, 0, 0, sw, sh, SRCCOPY);
        StretchBlt(hdc, 10, 10, sw - 20, sh - 20, hdc, 0, 0, sw, sh, SRCCOPY);

        ReleaseDC(0, hdc);
        Sleep(50); // Ajustado para o efeito ser visível
    }
    return 0;
}

int main() {
    // Esconder o console
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    // Inicia os dois primeiros efeitos imediatamente
    HANDLE h1 = CreateThread(NULL, 0, BezierPayload, NULL, 0, NULL);
    HANDLE h2 = CreateThread(NULL, 0, HorizontalShift, NULL, 0, NULL);

    // Aguarda 10 segundos
    Sleep(10000);

    // Inicia o terceiro efeito (Zoom) após o tempo passar
    HANDLE h3 = CreateThread(NULL, 0, ZoomPayload, NULL, 0, NULL);

    // Mantém o programa vivo
    WaitForSingleObject(h3, INFINITE);

    return 0;
}