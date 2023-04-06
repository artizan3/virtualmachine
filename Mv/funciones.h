//funciones a implementar {MOV,ADD,SUB,SWAP,MUL,DIV,CMP,SHL,SHR,AND,OR,XOR};

typedef struct{
char pos;
int tamano;
}TDD;

typedef void Funciones2op(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);

void MOV(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);

Funciones2op *funciones[]={MOV};

long int Operando2(long int op2,char top2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);


void Pre_Funcion(short int cant,long int op1,long int op2,char top1,char top2,char operacion,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){

    if (cant==2){
        long int valor=Operando2(op2,top2,TablaMemoria,TablaRegistros,TablaDeDatos);
        funciones[operacion](op1,top1,valor,TablaMemoria,TablaRegistros,TablaDeDatos);
    }else{}
}

long int Operando2(long int op2,char top2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
 char tiporeg;
 long int resultado;
    if (top2==2){
        tiporeg=op2>>4;
        if (tiporeg==3)
            resultado=TablaRegistros[(op2&0x0F)]&0x0000FFFF;
        if (tiporeg==2)
            resultado=(TablaRegistros[(op2&0x0F)]&0x0000FF00)>>8;
        if (tiporeg==1)
            resultado=TablaRegistros[(op2&0x0F)]&0x000000FF;
        if (tiporeg==0)
            resultado=TablaRegistros[(op2&0x0F)];
    }
    if (top2==0)
        if (op2&0x00FF0000==0)
            resultado=TablaMemoria[TablaDeDatos[1].pos + op2&0x00FFFF];//DS+offset
        else
            resultado=TablaMemoria[TablaRegistros[(op2&0x0F0000)>>16]+op2&0x00FFFF];
    if (top2==1)
        resultado=op2;
    return resultado;
}// lo que hace la funcion es ya tomar directamente el valor de el operando 2 y dejarlo listo para oprerar (en caso de ser posible lo deja en los bytes menos significativos)

void MOV(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
char tiporeg;
    if (top1==0){
        if (op1&0x00FF0000==0)
            TablaMemoria[TablaDeDatos[1].pos+op1&0x00FFFF]=valor;
        else
            TablaMemoria[TablaRegistros[(op1&0x0F0000)>>16]+op1&0x00FFFF]=valor;
        }else{
            tiporeg=op1>>4;
            if (tiporeg==0)
                TablaRegistros[op1&0x0F]=TablaRegistros[op1&0x0F]&0x00000000;
            if (tiporeg==1){
                TablaRegistros[op1&0x0F]=TablaRegistros[op1&0x0F]&0xFFFFFF00;
                valor=valor&0x000000FF;
            }
            if (tiporeg==2){
                TablaRegistros[op1&0x0F]=TablaRegistros[op1&0x0F]&0xFFFF00FF;
                valor=(valor&0x000000FF)<<8;
            }
            if (tiporeg==3){
                TablaRegistros[op1&0x0F]=TablaRegistros[op1&0x0F]&0xFFFF0000;
                valor=valor&0x000FFFF;
            }
            TablaRegistros[op1&0x0F]+=valor;
        }
}

