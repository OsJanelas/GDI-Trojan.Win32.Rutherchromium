[BITS 16]           ; Define arquitetura de 16 bits
[ORG 0x7C00]        ; Endereço padrão onde o BIOS carrega a MBR

start:
    ; Configura o modo de vídeo (VGA 320x200 - 256 cores)
    mov ax, 0x0013
    int 0x10

    mov ax, 0xA000  ; Endereço do segmento da memória de vídeo
    mov es, ax

main_loop:
    xor di, di      ; Começa no pixel 0
    inc bl          ; Incrementa offset de cor (animação)

draw_pixels:
    ; Cálculo simples de padrão visual (XOR entre coordenadas + cor)
    mov ax, di
    xor dx, dx
    mov cx, 320
    div cx          ; ax = y, dx = x

    xor al, dl      ; Operação lógica para padrão de "túnel/grade"
    add al, bl      ; Adiciona o deslocamento da animação
    
    mov [es:di], al ; Escreve o pixel na memória de vídeo
    
    inc di
    cmp di, 64000   ; 320 * 200 pixels
    jne draw_pixels

    ; Pequeno atraso para não rodar rápido demais
    mov cx, 0x0FFF
delay:
    loop delay

    jmp main_loop   ; Repete infinitamente

; Preenchimento da MBR (deve ter exatamente 512 bytes)
times 510-($-$$) db 0
dw 0xAA55           ; Assinatura de boot obrigatória