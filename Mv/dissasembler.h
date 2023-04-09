#define MAX 12
void Muesta_dissasembler(short int cant,long int op1,long int op2,char top1,char top2,char operacion,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void Muestra_op(short int cant,long int op,char top);
void Muestra_byte(char valor);

typedef char* nmonico;
nmonico nmonico1[MAX]={"MOV","ADD","SUB","SWAP","MUL","DIV","CMP","SHL","SHR","AND","OR","XOR"};
nmonico nmonico2[MAX]={"SYS","JMP","JZ","JP","JN","JNZ","JNP","JNN","LDL","LDH","RND","NOT"};

typedef char *reg_nom;
reg_nom nombres[15]={"CS","DS",0,0,0,"IP",0,0,"CC","AC","EAX","EBX","ECX","EDX","EEX","EFX"};

void Muestra_mem(char ip){
    printf("[%4.4X] ",ip);
}
void Muestra_byte(char valor){
    printf("%2.2X ",valor);
}
void Muestra_dissasembler(short int cant,long int op1,long int op2,char top1,char top2,char operacion,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    printf(" | ");
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
