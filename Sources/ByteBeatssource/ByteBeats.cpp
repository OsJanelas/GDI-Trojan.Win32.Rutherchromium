#include <windows.h>
#include <mmsystem.h>
#include <iostream>

// Isso força o linker a incluir a biblioteca de som do Windows
#pragma comment(lib, "winmm.lib")

// Função principal do som
void playBytebeat() {
    // Configurações de áudio: 1 canal (Mono), 32100Hz, 8 bits por amostra
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32100, 32100, 1, 8, 0 };
    HWAVEOUT hWaveOut = NULL;

    // Tenta abrir o dispositivo de áudio
    if (waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) {
        std::cerr << "Erro ao abrir o dispositivo de áudio!" << std::endl;
        return;
    }

    // Buffer de áudio (aproximadamente 1 minuto de som por bloco)
    const int bufferSize = 17000 * 60;
    BYTE* sbuffer = new BYTE[bufferSize];

    DWORD c = 0;      // Contador de tempo
    float d = 4.9f;   // Multiplicador de frequência
    int e = 0;        // Contador de iterações do ciclo

    bool statementA = true;
    bool statementB = false;
    bool statementC = false;

    std::cout << "Tocando Bytebeat... Pressione Ctrl+C para parar." << std::endl;

    while (true) {
        for (DWORD t = 0; t < bufferSize; t++) {
            // Lógica de Modulação (O coração do seu algoritmo)
            if (c % 10000 == 0 && c > 0) {
                if (statementA) {
                    d -= 0.1f;
                    if (d <= 4.0f) { d = 8.9f; e++; }
                    if (e == 5) { statementA = false; statementB = true; d = 3.2f; e = 0; }
                }
                else if (statementB) {
                    d += 0.1f;
                    if (d >= 3.9f) { d = 3.2f; e++; }
                    if (e == 9) { statementB = false; statementC = true; d = 3.8f; e = 0; }
                }
                else if (statementC) {
                    d -= 0.1f;
                    if (d <= 3.1f) { d = 3.9f; e++; }
                    if (e == 2) { statementC = false; statementA = true; d = 4.9f; e = 0; }
                }
            }

            // Cálculo do Bytebeat
            int dx = (int)(c * d);
            int freq = dx % 500;

            // O resultado final deve caber em um BYTE (0-255)
            sbuffer[t] = (BYTE)(c | (freq % 100000));

            c++;
        }

        // Prepara e envia para a placa de som
        WAVEHDR header = { (LPSTR)sbuffer, (DWORD)bufferSize, 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));

        // Espera o som tocar antes de recarregar o buffer
        // (Aproximadamente 31 segundos conforme o seu código original)
        Sleep(31000);

        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    }

    delete[] sbuffer;
    waveOutClose(hWaveOut);
}

int main() {
    playBytebeat();
    return 0;
}