#include <windows.h>

// Variáveis globais para dimensões da tela
int sw = GetSystemMetrics(0);
int sh = GetSystemMetrics(1);

// --- Efeito 1: Ruído GDI (thing6) ---
DWORD WINAPI payload_noise(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP bm = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, bm);

        // Pincel verde conforme seu código: RGB(rand() % 1, rand() % 255, rand() % 1)
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 1, rand() % 255, rand() % 1));
        SelectObject(hdc, brush);

        // Aplica o BitBlt com ROP code customizado
        BitBlt(hdc, rand() % 2, rand() % 2, sw, sh, hdcMem, rand() % 2, rand() % 2, 0x123456);

        DeleteObject(brush);
        DeleteObject(bm);
        DeleteDC(hdcMem);
        ReleaseDC(0, hdc);
        Sleep(1);
    }
    return 0;
}

// --- Efeito 2: Túneis de Inversão Circular ---
VOID WINAPI ci(int x, int y, int w, int h) {
    HDC hdc = GetDC(0);
    HRGN hrgn = CreateEllipticRgn(x, y, w + x, h + y);
    SelectClipRgn(hdc, hrgn);
    BitBlt(hdc, x, y, w, h, hdc, x, y, NOTSRCCOPY);
    DeleteObject(hrgn);
    ReleaseDC(NULL, hdc);
}

DWORD WINAPI payload_circles(LPVOID lpParam) {
    int w_limit = sw - 500;
    int h_limit = sh - 500;
    for (;;) {
        const int size = 1000;
        int x = rand() % (w_limit + size) - size / 2;
        int y = rand() % (h_limit + size) - size / 2;

        for (int i = 0; i < size; i += 100) {
            ci(x - i / 2, y - i / 2, i, i);
            Sleep(25);
        }
    }
    return 0;
}

// --- Efeito 3: Colunas Deslizantes (Pixel Shifting) ---
DWORD WINAPI payload_stretching(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(NULL);
        int rx = rand() % sw;
        // Pega uma coluna de 100px e "empurra" 10px para baixo
        BitBlt(hdc, rx, 10, 100, sh, hdc, rx, 0, SRCCOPY);
        ReleaseDC(NULL, hdc);
        // Sleep pequeno para não consumir 100% da CPU mas manter velocidade
        Sleep(1);
    }
    return 0;
}

// --- Função Principal ---
int main() {
    // Esconde o console para o efeito ser mais limpo
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    // Cria as 3 threads simultâneas
    HANDLE t1 = CreateThread(NULL, 0, payload_noise, NULL, 0, NULL);
    HANDLE t2 = CreateThread(NULL, 0, payload_circles, NULL, 0, NULL);
    HANDLE t3 = CreateThread(NULL, 0, payload_stretching, NULL, 0, NULL);

    // Aguarda infinitamente
    WaitForMultipleObjects(3, new HANDLE[3]{ t1, t2, t3 }, TRUE, INFINITE);

    return 0;
}