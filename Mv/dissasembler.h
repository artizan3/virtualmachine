#define MAX 12
void Muesta_dissasembler(short int cant,long int op1,long int op2,char top1,char top2,char operacion,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void Muestra_op(short int cant,long int op,char top);
void Muestra_byte(char valor);

typedef char* nmonico;
nmonico nmonico1[MAX]={"MOV","ADD","SUB","SWAP","MUL","DIV","CMP","SHL","SHR","AND","OR","XOR"};
nmonico nmonico2[MAX]={"SYS","JMP","JZ","JP","JN","JNZ","JNP","JNN","LDL","LDH","RND","NOT"};

typedef char *reg_nom;
reg_nom nombres[15]={"CS","DS",0,0,0,"IP",0,0,"CC","AC","EAX","EBX","ECX","EDX","EEX","EFX"};

void Dissasembler_mostrar(char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char top1,top2,operacion;
    short int cant;
    long int op1,op2;
    int corte;
    long int cont=0;
    while (cont!=TablaDeDatos[1].pos && cant!=0)
    {
        op1=0;
        op2=0;
        operacion=TablaMemoria[cont]&0x0F;
        Muestra_byte(TablaMemoria[cont]);
        Cant_op(TablaMemoria[cont],&top1,&top2,&cant);
        corte=top1^0x3;//le asigno el opuesto del tipo1 el cual representa la longitud
        if (cant==2){
            for (int i=1;i<=corte;i++){
                cont++;
                Muestra_byte(TablaMemoria[cont]);
                op1+=(TablaMemoria[cont])&0x000000FF;
                if (i!=corte)
                    op1=op1<<8;
            }
            corte=top2^0x3;//le asigno el opuesto del tipo2 el cual representa la longitud
            for (int i=1;i<=corte;i++){
                cont++;
                Muestra_byte(TablaMemoria[cont]);
                op2+=(TablaMemoria[cont])&0x000000FF;
                if (i!=corte)
                    op2=op2<<8;
            }
        }else{
            for (int i=1;i<=corte;i++){
                cont++;
                Muestra_byte(TablaMemoria[cont]);
                op1+=(TablaMemoria[cont])&0x000000FF;
                if (i!=corte)
                    op1=op1<<8;
            }
        }
        cont++;// le sumo uno mas al ip antes de que se ejecute la operacion.
        Muestra_dissasembler(cant,op1,op2,top1,top2,operacion,TablaMemoria,TablaRegistros,TablaDeDatos);//con esto arracamos la parte de hacer la funcion
    }
}
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
