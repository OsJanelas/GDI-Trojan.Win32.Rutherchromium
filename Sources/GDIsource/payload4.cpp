#include <windows.h>

// --- Efeito 1: Círculos Inversores ---
VOID WINAPI ci(int x, int y, int w, int h) {
    HDC hdc = GetDC(0);
    HRGN hrgn = CreateEllipticRgn(x, y, w + x, h + y);
    SelectClipRgn(hdc, hrgn);
    BitBlt(hdc, x, y, w, h, hdc, x, y, NOTSRCCOPY);
    DeleteObject(hrgn);
    ReleaseDC(NULL, hdc);
}

DWORD WINAPI Thread_Circles(LPVOID lpParam) {
    RECT rect;
    GetWindowRect(GetDesktopWindow(), &rect);
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

// --- Efeito 2: PatBlt (Piscada de Tela / Inversão de Padrão) ---
DWORD WINAPI Thread_PatBlt(LPVOID lpParam) {
    int x = GetSystemMetrics(0);
    int y = GetSystemMetrics(1);

    while (1) {
        HDC hdc = GetDC(0);
        // Cria um padrão de inversão na tela inteira
        PatBlt(hdc, 0, 0, x, y, PATINVERT);
        ReleaseDC(0, hdc);
        Sleep(100);
    }
    return 0;
}

// --- Efeito 3: Pixel Stretching (Esticar Colunas) ---
DWORD WINAPI Thread_Stretch(LPVOID lpParam) {
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);

    while (1) {
        HDC hdc = GetDC(0);
        int x = rand() % w;
        // Move uma coluna de pixels 1 pixel para baixo, criando o efeito de "esticar"
        BitBlt(hdc, x, 1, 10, h, hdc, x, 0, SRCCOPY);
        ReleaseDC(0, hdc);
        Sleep(2);
    }
    return 0;
}

// --- Função Principal ---
int main() {
    // Esconde o console para o efeito ser mais limpo
    // ShowWindow(GetConsoleWindow(), SW_HIDE);

    // Inicia as 3 threads simultâneas
    HANDLE threads[3];
    threads[0] = CreateThread(NULL, 0, Thread_Circles, NULL, 0, NULL);
    threads[1] = CreateThread(NULL, 0, Thread_PatBlt, NULL, 0, NULL);
    threads[2] = CreateThread(NULL, 0, Thread_Stretch, NULL, 0, NULL);

    // Mantém o programa rodando enquanto as threads estiverem ativas
    WaitForMultipleObjects(3, threads, TRUE, INFINITE);

    return 0;
}