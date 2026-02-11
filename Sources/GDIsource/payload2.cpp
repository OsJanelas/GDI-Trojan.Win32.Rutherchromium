#include <windows.h>

// Efeito 1: Trepidação/Cópia de Bits (BitBlt com SRCAND)
DWORD WINAPI Effect1(LPVOID lpParam) {
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (1) {
        HDC hdc = GetDC(0);
        // Cria um efeito de "sujeira" e deslocamento na tela
        BitBlt(hdc, rand() % 2, rand() % 2, w, h, hdc, rand() % 2, rand() % 2, SRCAND);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return 0;
}

// Efeito 2: Manipulação de Cores via DIBSection (RGB Math)
DWORD WINAPI Effect2(LPVOID lpParam) {
    HWND wnd = GetDesktopWindow();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = sw;
    bmi.bmiHeader.biHeight = -sh; // Negativo para top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;

    RGBTRIPLE* rgbtriple;

    while (1) {
        HDC desk = GetDC(0);
        HDC deskMem = CreateCompatibleDC(desk);
        HBITMAP scr = CreateDIBSection(desk, &bmi, DIB_RGB_COLORS, (void**)&rgbtriple, NULL, 0);
        SelectObject(deskMem, scr);

        // Copia a tela atual para a memória
        BitBlt(deskMem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);

        // Processamento de pixels
        for (int i = 0; i < sw * sh; i++) {
            // Lógica matemática de cores baseada no seu código
            rgbtriple[i].rgbtRed += GetRValue(i) & i;
            rgbtriple[i].rgbtGreen += GetGValue(i) & i;
            rgbtriple[i].rgbtBlue += GetBValue(i) & i;
        }

        // Desenha o resultado de volta na tela
        BitBlt(desk, 0, 0, sw, sh, deskMem, 0, 0, SRCCOPY);

        // Limpeza essencial para não travar o sistema
        ReleaseDC(wnd, desk);
        DeleteDC(deskMem);
        DeleteObject(scr);

        Sleep(30); // Delay leve para estabilidade
    }
    return 0;
}

int main() {
    // Cria as duas threads para rodar os códigos ao mesmo tempo
    HANDLE thread1 = CreateThread(NULL, 0, Effect1, NULL, 0, NULL);
    HANDLE thread2 = CreateThread(NULL, 0, Effect2, NULL, 0, NULL);

    // Aguarda as threads (loop infinito)
    if (thread1 && thread2) {
        HANDLE threads[2] = { thread1, thread2 };
        WaitForMultipleObjects(2, threads, TRUE, INFINITE);
    }

    return 0;
}