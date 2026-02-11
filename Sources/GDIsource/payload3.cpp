#include <windows.h>

// --- Efeito 1: Distorção de Memória e Cores ---
DWORD WINAPI Effect_Noise(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        HDC hdcMem = CreateCompatibleDC(hdc);
        int sw = GetSystemMetrics(0);
        int sh = GetSystemMetrics(1);
        HBITMAP bm = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, bm);

        // O seu código original usava um ROP code customizado (0x123456)
        // Isso gera padrões de bits imprevisíveis na tela
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);

        BitBlt(hdc, rand() % 2, rand() % 2, sw, sh, hdcMem, rand() % 2, rand() % 2, 0x123456);

        // Limpeza de objetos GDI para evitar crash do sistema
        DeleteObject(brush);
        DeleteObject(bm);
        DeleteDC(hdcMem);
        ReleaseDC(0, hdc);
        Sleep(1);
    }
    return 0;
}

// --- Efeito 2: Círculos Inversores (Invert Circles) ---
VOID WINAPI ci(int x, int y, int w, int h) {
    HDC hdc = GetDC(0);
    HRGN hrgn = CreateEllipticRgn(x, y, w + x, h + y);
    SelectClipRgn(hdc, hrgn);
    // NOTSRCCOPY inverte as cores da área selecionada
    BitBlt(hdc, x, y, w, h, hdc, x, y, NOTSRCCOPY);
    DeleteObject(hrgn);
    ReleaseDC(NULL, hdc);
}

DWORD WINAPI Effect_Circles(LPVOID lpParam) {
    RECT rect;
    HWND desk = GetDesktopWindow();
    GetWindowRect(desk, &rect);

    int w = rect.right - rect.left - 500;
    int h = rect.bottom - rect.top - 500;

    for (;;) {
        const int size = 1000;
        int x = rand() % (w + size) - size / 2;
        int y = rand() % (h + size) - size / 2;

        for (int i = 0; i < size; i += 100) {
            ci(x - i / 2, y - i / 2, i, i);
            Sleep(25);
        }
    }
    return 0;
}

// --- Função Principal ---
int main() {
    // Cria a primeira thread (Ruído e Cores)
    HANDLE t1 = CreateThread(NULL, 0, Effect_Noise, NULL, 0, NULL);

    // Cria a segunda thread (Círculos que invertem cores)
    HANDLE t2 = CreateThread(NULL, 0, Effect_Circles, NULL, 0, NULL);

    // Mantém o programa vivo infinitamente
    if (t1 && t2) {
        WaitForMultipleObjects(2, new HANDLE[2]{ t1, t2 }, TRUE, INFINITE);
    }

    return 0;
}