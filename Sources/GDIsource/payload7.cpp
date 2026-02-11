#include <windows.h>
#include <cmath>

// Variáveis Globais
int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);
bool runAto1 = true;

// Estruturas para manipulação de cores
struct HSL { float h, s, l; };

// --- Funções de Conversão de Cor ---
float hue2rgb(float p, float q, float t) {
    if (t < 0.0f) t += 1.0f;
    if (t > 1.0f) t -= 1.0f;
    if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
    if (t < 1.0f / 2.0f) return q;
    if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
    return p;
}

HSL RGBtoHSL(RGBQUAD rgb) {
    float r = rgb.rgbRed / 255.0f, g = rgb.rgbGreen / 255.0f, b = rgb.rgbBlue / 255.0f;
    float maxV = fmaxf(fmaxf(r, g), b), minV = fminf(fminf(r, g), b);
    float h, s, l = (maxV + minV) / 2.0f;
    if (maxV == minV) h = s = 0.0f;
    else {
        float diff = maxV - minV;
        s = (l > 0.5f) ? diff / (2.0f - maxV - minV) : diff / (maxV + minV);
        if (maxV == r) h = (g - b) / diff + (g < b ? 6.0f : 0.0f);
        else if (maxV == g) h = (b - r) / diff + 2.0f;
        else h = (r - g) / diff + 4.0f;
        h /= 6.0f;
    }
    return { h, s, l };
}

RGBQUAD HSLtoRGB(HSL hsl) {
    float r, g, b, p, q;
    if (hsl.s == 0.0f) r = g = b = hsl.l;
    else {
        q = (hsl.l < 0.5f) ? (hsl.l * (1.0f + hsl.s)) : (hsl.l + hsl.s - hsl.l * hsl.s);
        p = 2.0f * hsl.l - q;
        r = hue2rgb(p, q, hsl.h + 1.0f / 3.0f);
        g = hue2rgb(p, q, hsl.h);
        b = hue2rgb(p, q, hsl.h - 1.0f / 3.0f);
    }
    return { (BYTE)(b * 255), (BYTE)(g * 255), (BYTE)(r * 255), 0 };
}

// --- Efeito de Pulso ---
double globalIntensity = 0.0;
bool fadeState = false;
int getFadeIntensity(int maxIntensity) {
    if (!fadeState) {
        globalIntensity += 0.1;
        if (globalIntensity >= maxIntensity) fadeState = true;
    }
    else {
        globalIntensity -= 0.1;
        if (globalIntensity <= 0) fadeState = false;
    }
    return (int)globalIntensity;
}

// --- THREADS DE PAYLOAD ---

DWORD WINAPI ScreenShake(LPVOID lp) {
    while (true) {
        HDC hdc = GetDC(0);
        BitBlt(hdc, rand() % 10, rand() % 10, w, h, hdc, rand() % 10, rand() % 10, SRCCOPY);
        ReleaseDC(0, hdc);
        Sleep(15);
    }
}

DWORD WINAPI HSLShader(LPVOID lp) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    BITMAPINFO bmpi = { 0 };
    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = -h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    RGBQUAD* rgbquad;
    HBITMAP bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);
    int r_factor = 0;
    while (true) {
        HDC currentHdc = GetDC(NULL);
        BitBlt(hdcCopy, 0, 0, w, h, currentHdc, 0, 0, SRCCOPY);
        for (int i = 0; i < w * h; i++) {
            HSL hsl = RGBtoHSL(rgbquad[i]);
            hsl.h = fmod(hsl.h + 0.01f, 1.0f);
            rgbquad[i] = HSLtoRGB(hsl);
            BYTE* pixelBytes = (BYTE*)&rgbquad[i];
            pixelBytes[1 % 2] = pixelBytes[r_factor & 2];
        }
        BitBlt(currentHdc, 0, 0, w, h, hdcCopy, 0, 0, SRCCOPY);
        r_factor = getFadeIntensity(3);
        ReleaseDC(NULL, currentHdc);
        Sleep(10);
    }
}

DWORD WINAPI TextPayload(LPVOID lp) {
    while (runAto1) {
        HDC hdc = GetDC(NULL);
        SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        SetBkMode(hdc, TRANSPARENT);
        TextOutA(hdc, rand() % w, rand() % h, "ALERT", 5);
        ReleaseDC(NULL, hdc);
        Sleep(100);
    }
    return 0;
}

// --- FUNÇÃO PRINCIPAL ---

int main() {
    // 1. Espera 20 segundos antes de começar
    Sleep(20000);

    // 2. Esconde o console
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    // 3. Dispara as threads restantes
    CreateThread(NULL, 0, ScreenShake, NULL, 0, NULL);
    CreateThread(NULL, 0, HSLShader, NULL, 0, NULL);
    CreateThread(NULL, 0, TextPayload, NULL, 0, NULL);

    // Loop infinito para manter o processo vivo
    while (true) {
        Sleep(1000);
    }

    return 0;
}