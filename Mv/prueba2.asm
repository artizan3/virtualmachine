        xor efx, efx
        mov [4], 0
        mov al, %01
        mov edx, ds
        mov cl, 1
        mov ch, 4
otro:   sys 1
        cmp [0], 0
        jn sigue
        add [4], [0]
        add efx, 1
        jmp otro
sigue:  cmp 0, efx
        jz fin
        div [4], efx
fin:    mov edx, ds
        add edx, 4
        mov cl, 1
        mov ch, 4
        mov al, %01
        sys 2
        stop