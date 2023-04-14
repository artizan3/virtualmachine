#define MAX 12
void Muesta_dissasembler(short int cant,long int op1,long int op2,char top1,char top2,char operacion,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void Muestra_op(short int cant,long int op,char top);
void Muestra_byte(char valor);

typedef char* nmonico;
nmonico nmonico1[MAX]={"MOV","ADD","SUB","SWAP","MUL","DIV","CMP","SHL","SHR","AND","OR","XOR"};
nmonico nmonico2[MAX]={"SYS","JMP","JZ","JP","JN","JNZ","JNP","JNN","LDL","LDH","RND","NOT"};

typedef char *reg_nom;
reg_nom nombres[16]={"CS","DS",0,0,0,"IP",0,0,"CC","AC","EAX","EBX","ECX","EDX","EEX","EFX"};

void Dissasembler_mostrar(char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char top1,top2,operacion;
    short int espacios,cant;
    long int op1,op2;
    int corte;
    long int cont=0;
    while (cont<TablaDeDatos[1].pos)
    {
        espacios=0;
        op1=0;
        op2=0;
        operacion=TablaMemoria[cont]&0x0F;
        Muestra_mem(cont);//muestra la pos ip
        Muestra_byte(TablaMemoria[cont]);//muestra la operacion
        espacios++;
        Cant_op(TablaMemoria[cont],&top1,&top2,&cant);
        corte=top1^0x3;
        if (cant==2){
            for (int i=1;i<=corte;i++){
                cont++;
                Muestra_byte(TablaMemoria[cont]);
                op1+=(TablaMemoria[cont])&0x000000FF;
                if (i!=corte)
                    op1=op1<<8;
            }
            espacios+=corte;
            corte=top2^0x3;
            for (int i=1;i<=corte;i++){
                cont++;
                Muestra_byte(TablaMemoria[cont]);
                op2+=(TablaMemoria[cont])&0x000000FF;
                if (i!=corte)
                    op2=op2<<8;
            }
            espacios+=corte;
        }else{
            for (int i=1;i<=corte;i++){
                cont++;
                Muestra_byte(TablaMemoria[cont]);
                op1+=(TablaMemoria[cont])&0x000000FF;
                if (i!=corte)
                    op1=op1<<8;
            }
            espacios+=corte;
        }
        cont++;
        if (espacios!=7)
            for (int i=espacios;i<7;i++)
                printf("%s","   ");
        Muestra_dissasembler(cant,op1,op2,top1,top2,operacion,TablaMemoria,TablaRegistros,TablaDeDatos);
    }
}
void Muestra_mem(char ip){
    printf("[%4.4X] ",ip);
}
void Muestra_byte(char valor){
    printf("%2.2X ",valor&0xFF);
}
void Muestra_dissasembler(short int cant,long int op1,long int op2,char top1,char top2,char operacion,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    printf("| ");
    if (cant==2){
        printf("%s ",nmonico1[operacion]);
    }
    if (cant==1){
        printf("%s ",nmonico2[operacion]);
    }
    if (cant==0)
        printf("STOP");
    Muestra_op(cant,op1,top1);
    if (cant==2){
        printf(",");
        Muestra_op(cant,op2,top2);
    }
    printf("\n");
}
void Muestra_op(short int cant,long int op,char top){
char tiporeg;
    if (top==1)
        printf("%d",op);
    if (top==0){
        if (((op&0x000F0000)>>16)==1)
            printf("[%d]",op&0x0000FFFF);
        else
            printf("[%s+%d]",nombres[op>>16],op&0x0000FFFF);
    }
    if (top==2){
        tiporeg=op>>4;
        if (op&0x0F==1){
            printf("%s",nombres[1]);
        }else
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
