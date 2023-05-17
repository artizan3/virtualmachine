#define MAX 15
#define MAXREG 16

void Muestra_mem(long int ip);
void Muesta_dissasembler(long int cant,long int op1,long int op2,char top1,char top2,char operacion);
void Muestra_op(long int cant,long int op,char top);
void Muestra_byte(char valor);
void Dissasembler_mostrar(MV mv);

typedef char* nmonico;
nmonico nmonico1[MAX]={"MOV","ADD","SUB","SWAP","MUL","DIV","CMP","SHL","SHR","AND","OR","XOR"};
nmonico nmonico2[MAX]={"SYS","JMP","JZ","JP","JN","JNZ","JNP","JNN","LDL","LDH","RND","NOT","PUSH","POP","CALL"};

typedef char *reg_nom;
reg_nom nombres[MAXREG]={"CS","DS","KS","ES","SS","IP","SP","BP","CC","AC","EAX","EBX","ECX","EDX","EEX","EFX"};

void Dissasembler_mostrar(MV mv){
    char top1,top2,operacion;
    short int espacios,cant;
    long int op1,op2;
    int corte;
    long int cont=0;
    while (cont<mv.TablaDeDatos[1].pos){
        espacios=0;
        op1=0;
        op2=0;
        operacion=mv.TablaMemoria[cont]&0xF;
        Muestra_mem(cont);
        Muestra_byte(mv.TablaMemoria[cont]);
        espacios++;
        Cant_op(mv.TablaMemoria[cont],&top1,&top2,&cant);
        corte=top1^0x3;
        if (cant==2){
            for (int i=1;i<=corte;i++){
                cont++;
                Muestra_byte(mv.TablaMemoria[cont]);
                op1+=(mv.TablaMemoria[cont])&0x000000FF;
                if (i!=corte)
                    op1=op1<<8;
            }
            espacios+=corte;
            corte=top2^0x3;
            for (int i=1;i<=corte;i++){
                cont++;
                Muestra_byte(mv.TablaMemoria[cont]);
                op2+=(mv.TablaMemoria[cont])&0x000000FF;
                if (i!=corte)
                    op2=op2<<8;
            }
            espacios+=corte;
        }else{
            if (cant==0)
                op1=operacion;
            else
                for (int i=1;i<=corte;i++){
                    cont++;
                    Muestra_byte(mv.TablaMemoria[cont]);
                    op1+=(mv.TablaMemoria[cont])&0x000000FF;
                    if (i!=corte)
                        op1=op1<<8;
                }
                espacios+=corte;
        }
        cont++;
        if (espacios!=7)
            for (int i=espacios;i<7;i++)
                printf("%s","   ");
        Muestra_dissasembler(cant,op1,op2,top1,top2,operacion);
    }
    printf("------------------------\n");
}
void Muestra_mem(long int ip){
    printf("[%4.4X] ",ip);
}
void Muestra_byte(char valor){
    printf("%2.2X ",valor&0xFF);
}
void Muestra_dissasembler(long int cant,long int op1,long int op2,char top1,char top2,char operacion){
    printf("| ");
    if (cant==2){
        printf("%s ",nmonico1[operacion]);
    }
    if (cant==1){
        printf("%s ",nmonico2[operacion]);
    }
    if (cant==0)
        if (op1==0)
            printf("STOP");
        else
            printf("RET");
    Muestra_op(cant,op1,top1);
    if (cant==2){
        printf(",");
        Muestra_op(cant,op2,top2);
    }
    printf("\n");
}
void Muestra_op(long int cant,long int op,char top){
    char tiporeg;
    if (top==1)
        printf("%X",op);
    if (top==0){
        short int offset=op;
        char tipo_mem=op>>22;
        if (tipo_mem==0)
            printf("l");
        if (tipo_mem==2)
            printf("w");
        if (tipo_mem==3)
            printf("b");
        if (((op&0x000F0000)>>16)==1)
            printf("[%d]",offset);
        else{
            char reg=(op>>16)&0x0F;
            printf("[%s+%d]",nombres[reg],offset);
        }
    }
    if (top==2){
        tiporeg=(op>>4)&0x3;
        if ((op&0x0F)==1){
            printf("%s",nombres[1]);
        }else{
            if (tiporeg==0)
                printf("%s",nombres[op&0x0F]);
            if (tiporeg==1)
                printf("%XL",op&0x0F);
            if (tiporeg==2)
                printf("%XH",op&0x0F);
            if (tiporeg==3)
                printf("%XX",op&0x0F);
        }

    }
}

