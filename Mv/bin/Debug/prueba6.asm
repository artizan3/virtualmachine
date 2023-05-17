                sys 15
		mov ebx, 5
                mov ecx, 3
		mov bp,sp
                push ebx
                push ecx
                call potencia
                add sp,8
                mov [bp-4],eax
                mov edx,bp
                sub edx,4
                mov ch,4
                mov cl,1
                mov al 1
                sys 2
                stop

potencia:       push bp 
                mov bp,sp
                push eex
                push efx
                mov eex,[bp+8]           ;exponente
                mov efx,[bp+12]          ;numero a potenciar
                cmp eex,1
                jnp caso_base        
                sub eex,1
                push efx
                push eex
                call potencia
                add sp,8
                mul eax,efx
                jmp fin
caso_base:      mov eax,efx
fin:            pop efx
                pop eex
                pop bp
                ret         

                