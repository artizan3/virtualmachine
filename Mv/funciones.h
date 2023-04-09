#include<time.h>
typedef struct{
char pos;
int tamano;
}TDD;

typedef void Funciones2op(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void MOV(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void ADD(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void SUB(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void SWAP(long int op1,long int op2,char top1,char top2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void MUL(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void DIV(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void CMP(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void SHL(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void SHR(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void AND(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void OR(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void XOR(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
Funciones2op *funciones[]={MOV,ADD,SUB,0,MUL,DIV,CMP,SHL,SHR,AND,OR,XOR};

typedef void Func_1op_tipo2(long int valor,long int TablaRegistros);
void JMP(long int valor,long int TablaRegistros[]);
void JZ(long int valor,long int TablaRegistros[]);
void JP(long int valor,long int TablaRegistros[]);
void JN(long int valor,long int TablaRegistros[]);
void JNZ(long int valor,long int TablaRegistros[]);
void JNP(long int valor,long int TablaRegistros[]);
void JNN(long int valor,long int TablaRegistros[]);
void LDH(long int valor,long int TablaRegistros[]);
void LDL(long int valor,long int TablaRegistros[]);
void RND(long int valor,long int TablaRegistros[]);
Func_1op_tipo2 *tipo2[]={0,JMP,JZ,JP,JN,JNZ,JNP,JNN,LDL,LDH,RND,0};

typedef void Func_1op_tipo1(long int op,char top,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void NOT(long int op,char top,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void SYS(long int op,char top,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
Func_1op_tipo1 *tipo1[]={SYS,0,0,0,0,0,0,0,0,0,0,NOT};


long int Operando2(long int op2,char top2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
long int Valor_mem(long int op,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void Ultima_operacion(long int TablaRegistros[],long int nz);

void Pre_Funcion(short int cant,long int op1,long int op2,char top1,char top2,char operacion,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
long int valor;
    if (cant==2){
        if (operacion!=3){
            valor=Operando2(op2,top2,TablaMemoria,TablaRegistros,TablaDeDatos);
            funciones[operacion](op1,top1,valor,TablaMemoria,TablaRegistros,TablaDeDatos);
        }else
            SWAP(op1,op2,top1,top2,TablaMemoria,TablaRegistros,TablaDeDatos);
    }else{
        valor=Operando2(op1,top1,TablaMemoria,TablaRegistros,TablaDeDatos);
        if (operacion<=10 && operacion>=1)
            tipo2[operacion](valor,TablaRegistros);
        else
            tipo1[operacion](op1,top1,TablaMemoria,TablaRegistros,TablaDeDatos);
    }
}

long int Operando2(long int op2,char top2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
 char tiporeg;
 long int resultado=0;
    if (top2==2){
        if (op2==1)
            resultado=TablaDeDatos[1].pos;
        else{
            tiporeg=op2>>4;
            if (tiporeg==3)
                resultado=(TablaRegistros[(op2&0x0F)]&0x0000FFFF);
            if (tiporeg==2)
                resultado=((TablaRegistros[(op2&0x0F)]&0x0000FF00)>>8);
            if (tiporeg==1)
                resultado=(TablaRegistros[(op2&0x0F)]&0x000000FF);
            if (tiporeg==0)
                resultado=TablaRegistros[(op2&0x0F)];
        }
    }
    if (top2==0){
       resultado=Valor_mem(op2,TablaMemoria,TablaRegistros,TablaDeDatos);
    }
    if (top2==1)
        resultado=(op2);
    return (resultado);
}// lo que hace la funcion es ya tomar directamente el valor de el operando 2 y dejarlo listo para oprerar (en caso de ser posible lo deja en los bytes menos significativos)

void MOV(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
char tiporeg;
//long int mascara=0xFF000000;
    if (top1==0){
            for(int i=0;i<=3;i++){
                if (((op1&0x000F0000)>>16)==1)
                    TablaMemoria[TablaDeDatos[1].pos+(op1&0x0000FFFF)+i]=(((valor))>>(3-i)*8)&0x000000FF;
                    //TablaMemoria[TablaDeDatos[1].pos+(op1&0x0000FFFF)+i]=(((valor)&(mascara>>i*8))>>(3-i)*8);
                else
                    TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)+i]=(((valor))>>(3-i)*8);
                    //TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)+i]=(((valor)&(mascara>>i*8))>>(3-i)*8);
            }
        }else{
            tiporeg=op1>>4;
            if (tiporeg==0)
                TablaRegistros[(op1&0x0F)]=0;
            if (tiporeg==1){
                TablaRegistros[(op1&0x0F)]=(TablaRegistros[(op1&0x0F)]&0xFFFFFF00);
                valor=(valor&0x000000FF);
            }
            if (tiporeg==2){
                TablaRegistros[(op1&0x0F)]=(TablaRegistros[(op1&0x0F)]&0xFFFF00FF);
                valor=((valor&0x000000FF)<<8);
            }
            if (tiporeg==3){
                TablaRegistros[(op1&0x0F)]=(TablaRegistros[(op1&0x0F)]&0xFFFF0000);
                valor=(valor&0x000FFFF);
            }
            TablaRegistros[(op1&0x0F)]+=valor;
        }
}
void ADD(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char tiporeg;
    long int aux=0,nz=0;
    if (top1==0){
        long int valorm=Valor_mem(op1,TablaMemoria,TablaRegistros,TablaDeDatos);
        nz=valorm+valor;
        valorm=nz;
        MOV(op1,top1,valorm,TablaMemoria,TablaRegistros,TablaDeDatos);
    }else{
        tiporeg=op1>>4;
        if (tiporeg==0){
            aux=TablaRegistros[(op1&0x0F)]+valor;
            nz=aux;
        }
        if (tiporeg==1){
            aux=(TablaRegistros[(op1&0x0F)]&0x000000FF)+(valor&0x000000FF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFFFF00);
        }
        if (tiporeg==2){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FF00)+((valor&0x000000FF)<<8);
            nz=aux>>8;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF00FF);
        }
        if (tiporeg==3){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FFFF)+(valor&0x000FFFF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF0000);
        }
        TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(TablaRegistros,nz);
}
void SUB(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char tiporeg;
    long int aux=0,nz=0;
    if (top1==0){
        long int valorm=Valor_mem(op1,TablaMemoria,TablaRegistros,TablaDeDatos);
        nz=valorm-valor;
        valorm=nz;
        MOV(op1,top1,valorm,TablaMemoria,TablaRegistros,TablaDeDatos);
    }else{
        tiporeg=op1>>4;
        if (tiporeg==0){
            aux=TablaRegistros[(op1&0x0F)]-valor;
            nz=aux;
        }
        if (tiporeg==1){
            aux=(TablaRegistros[(op1&0x0F)]&0x000000FF)-(valor&0x000000FF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFFFF00);
        }
        if (tiporeg==2){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FF00)-((valor&0x000000FF)<<8);
            nz=aux>>8;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF00FF);
        }
        if (tiporeg==3){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FFFF)-(valor&0x000FFFF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF0000);
        }
        TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(TablaRegistros,nz);
}
void SWAP(long int op1,long int op2,char top1,char top2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
long int aux,aux2;
    if (top1==top2){
        if (top1==0){
           aux=Valor_mem(op1,TablaMemoria,TablaRegistros,TablaDeDatos);
           aux2=Valor_mem(op2,TablaMemoria,TablaRegistros,TablaDeDatos);
          // printf("%d %d",aux,aux2);
        }else{
            aux=Operando2(op1,top1,TablaMemoria,TablaRegistros,TablaDeDatos);
            aux2=Operando2(op2,top2,TablaMemoria,TablaRegistros,TablaDeDatos);
        }
        MOV(op1,top1,aux2,TablaMemoria,TablaRegistros,TablaDeDatos);
        MOV(op2,top2,aux,TablaMemoria,TablaRegistros,TablaDeDatos);
    }
}
void MUL(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char tiporeg;
    long int aux=0,nz=0;
    if (top1==0){
        long int valorm=Valor_mem(op1,TablaMemoria,TablaRegistros,TablaDeDatos);
        nz=valorm*valor;
        valorm=nz;
        MOV(op1,top1,valorm,TablaMemoria,TablaRegistros,TablaDeDatos);
    }else{
        tiporeg=op1>>4;
        if (tiporeg==0){
            aux=TablaRegistros[(op1&0x0F)]*valor;
            nz=aux;
        }
        if (tiporeg==1){
            aux=(TablaRegistros[(op1&0x0F)]&0x000000FF)*(valor&0x000000FF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFFFF00);
        }
        if (tiporeg==2){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FF00)*((valor&0x000000FF)<<8);
            nz=aux>>8;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF00FF);
        }
        if (tiporeg==3){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FFFF)*(valor&0x000FFFF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF0000);
        }
        TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(TablaRegistros,nz);
}
void DIV(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char tiporeg;
    long int aux=0,nz=0;
    if(valor!=0){
        if (top1==0){
            long int valorm=Valor_mem(op1,TablaMemoria,TablaRegistros,TablaDeDatos);
            TablaRegistros[9]=valorm%valor;
            nz=valorm/valor;
            valorm=nz;
            MOV(op1,top1,valorm,TablaMemoria,TablaRegistros,TablaDeDatos);
        }else{
            tiporeg=op1>>4;
            if (tiporeg==0){
                TablaRegistros[9]=TablaRegistros[(op1&0x0F)]%valor;
                aux=TablaRegistros[(op1&0x0F)]/valor;
                nz=aux;
            }
            if (tiporeg==1){
                TablaRegistros[9]=(TablaRegistros[(op1&0x0F)]&0x000000FF)%(valor&0x000000FF);
                aux=(TablaRegistros[(op1&0x0F)]&0x000000FF)/(valor&0x000000FF);
                nz=aux;
                aux+=(TablaRegistros[(op1&0x0F)]&0xFFFFFF00);
            }
            if (tiporeg==2){
                TablaRegistros[9]=((TablaRegistros[(op1&0x0F)]&0x0000FF00)>>8)%(valor&0x000000FF);
                aux=((TablaRegistros[(op1&0x0F)]&0x0000FF00)>>8)/((valor&0x000000FF));
                aux=(aux<<8)&0x0000FF00;
                nz=aux;
                aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF00FF);
            }
            if (tiporeg==3){
                TablaRegistros[9]=(TablaRegistros[(op1&0x0F)]&0x0000FFFF)%(valor&0x000FFFF);
                aux=(TablaRegistros[(op1&0x0F)]&0x0000FFFF)/(valor&0x000FFFF);
                nz=aux;
                aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF0000);
            }
            TablaRegistros[(op1&0x0F)]=aux;
        }
        Ultima_operacion(TablaRegistros,nz);
    }
}
void CMP(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char tiporeg;
    long int valor;
    if (top1==0){
        valor=Valor_mem(op1,TablaMemoria,TablaRegistros,TablaDeDatos);
    }else{
        tiporeg=op1>>4;
        if (tiporeg==0)
            valor=TablaRegistros[(op1&0x0F)];
        if (tiporeg==1)
            valor=(TablaRegistros[(op1&0x0F)]&0x000000FF);
        if (tiporeg==2)
            valor=((TablaRegistros[(op1&0x0F)]&0x0000FF00)>>8);
        if (tiporeg==3)
            valor=(TablaRegistros[(op1&0x0F)]&0x0000FFFF);
    }
    Ultima_operacion(TablaRegistros,valor-valor2);
}
void SHL(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char tiporeg;
    long int aux,nz=0;
    if (top1==0){
        long int valorm=Valor_mem(op1,TablaMemoria,TablaRegistros,TablaDeDatos);
        nz=valorm<<valor2;
        valorm=nz;
        MOV(op1,top1,valorm,TablaMemoria,TablaRegistros,TablaDeDatos);
        }else{
            tiporeg=op1>>4;
            if (tiporeg==0){
               aux=TablaRegistros[(op1&0x0F)]<<valor2;
               nz=aux;
               TablaRegistros[(op1&0x0F)]=0;
            }
            if (tiporeg==1){
                aux=(TablaRegistros[(op1&0x0F)]&0x000000FF);
                TablaRegistros[(op1&0x0F)]=TablaRegistros[(op1&0x0F)]&0xFFFFFF00;
                aux=(aux<<valor2)&0x000000FF;
                nz=aux;
            }
            if (tiporeg==2){
                aux=((TablaRegistros[(op1&0x0F)]&0x0000FF00)>>8);
                TablaRegistros[(op1&0x0F)]=TablaRegistros[(op1&0x0F)]&0xFFFF00FF;
                aux=((aux<<valor2)&0x000000FF);
                nz=aux;
                aux=aux<<8;
            }
            if (tiporeg==3){
                aux=(TablaRegistros[(op1&0x0F)]&0x0000FFFF);
                TablaRegistros[(op1&0x0F)]=TablaRegistros[(op1&0x0F)]&0xFFFF0000;
                aux=(aux<<valor2)&&0x0000FFFF;
                nz=aux;
            }
        TablaRegistros[(op1&0x0F)]+=aux;
        }
     Ultima_operacion(TablaRegistros,nz);
}
void SHR(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char tiporeg;
    long int aux,nz=0;
    if (top1==0){
        long int valorm=Valor_mem(op1,TablaMemoria,TablaRegistros,TablaDeDatos);
        nz=valorm>>valor2;
        valorm=nz;
        MOV(op1,top1,valorm,TablaMemoria,TablaRegistros,TablaDeDatos);
    }else{
        tiporeg=op1>>4;
        if (tiporeg==0){
            aux=TablaRegistros[(op1&0x0F)]>>valor2;
            nz=aux;
            TablaRegistros[(op1&0x0F)]=0;
        }
        if (tiporeg==1){
            aux=(TablaRegistros[(op1&0x0F)]&0x000000FF);
            TablaRegistros[(op1&0x0F)]=TablaRegistros[(op1&0x0F)]&0xFFFFFF00;
            aux=(aux>>valor2)&0x000000FF;
            nz=aux;
        }
        if (tiporeg==2){
            aux=((TablaRegistros[(op1&0x0F)]&0x0000FF00)>>8);
            TablaRegistros[(op1&0x0F)]=TablaRegistros[(op1&0x0F)]&0xFFFF00FF;
            aux=((aux>>valor2)&0x000000FF);
            nz=aux;
            aux=aux<<8;
        }
        if (tiporeg==3){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FFFF);
            TablaRegistros[(op1&0x0F)]=TablaRegistros[(op1&0x0F)]&0xFFFF0000;
            aux=(aux>>valor2)&&0x0000FFFF;
            nz=aux;
        }
        TablaRegistros[(op1&0x0F)]+=aux;
    }
    Ultima_operacion(TablaRegistros,nz);
}
void AND(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char tiporeg;
    long int aux=0,nz=0;
    if (top1==0){
        long int valorm=Valor_mem(op1,TablaMemoria,TablaRegistros,TablaDeDatos);
        nz=valorm&valor2;
        valorm=nz;
        MOV(op1,top1,valorm,TablaMemoria,TablaRegistros,TablaDeDatos);
    }else{
        tiporeg=op1>>4;
        if (tiporeg==0){
            aux=(TablaRegistros[(op1&0x0F)])&(valor2);
            nz=aux;
        }
        if (tiporeg==1){
            aux=(TablaRegistros[(op1&0x0F)]&0x000000FF)&(valor2&0x000000FF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFFFF00);
        }
        if (tiporeg==2){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FF00)&((valor2&0x000000FF)<<8);
            nz=aux>>8;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF00FF);
        }
        if (tiporeg==3){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FFFF)&(valor2&0x000FFFF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF0000);
        }
        TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(TablaRegistros,nz);//no se si opera asi, por si las dudas lo dejo en stand by.
}
void OR(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
char tiporeg;
    long int aux=0,nz=0;
    if (top1==0){
        long int valorm=Valor_mem(op1,TablaMemoria,TablaRegistros,TablaDeDatos);
        nz=valorm|valor2;
        valorm=nz;
        MOV(op1,top1,valorm,TablaMemoria,TablaRegistros,TablaDeDatos);
    }else{
        tiporeg=op1>>4;
        if (tiporeg==0){
            aux=(TablaRegistros[(op1&0x0F)])|(valor2);
            nz=aux;
        }
        if (tiporeg==1){
            aux=(TablaRegistros[(op1&0x0F)]&0x000000FF)|(valor2&0x000000FF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFFFF00);
        }
        if (tiporeg==2){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FF00)|((valor2&0x000000FF)<<8);
            nz=aux>>8;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF00FF);
        }
        if (tiporeg==3){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FFFF)|(valor2&0x000FFFF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF0000);
        }
        TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(TablaRegistros,nz);//no se si opera asi, por si las dudas lo dejo en stand by.
}
void XOR(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
char tiporeg;
    long int aux=0,nz=0;
    if (top1==0){
        long int valorm=Valor_mem(op1,TablaMemoria,TablaRegistros,TablaDeDatos);
        nz=valorm^valor2;
        valorm=nz;
        MOV(op1,top1,valorm,TablaMemoria,TablaRegistros,TablaDeDatos);
    }else{
        tiporeg=op1>>4;
        if (tiporeg==0){
            aux=(TablaRegistros[(op1&0x0F)])^(valor2);
            nz=aux;
        }
        if (tiporeg==1){
            aux=(TablaRegistros[(op1&0x0F)]&0x000000FF)^(valor2&0x000000FF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFFFF00);
        }
        if (tiporeg==2){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FF00)^((valor2&0x000000FF)<<8);
            nz=aux>>8;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF00FF);
        }
        if (tiporeg==3){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FFFF)^(valor2&0x000FFFF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF0000);
        }
        TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(TablaRegistros,nz);//no se si opera asi, por si las dudas lo dejo en stand by.
}

void SYS(long int op,char top,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    long int valor,pos,tot;
    int rep,byt,tipo;
    if (top==0)
        valor=Valor_mem(op,TablaMemoria,TablaRegistros,TablaDeDatos);
    if (top==2)
        valor=Operando2(op,top,TablaMemoria,TablaRegistros,TablaDeDatos);
    else
        valor=op;
    pos=TablaRegistros[13];//EDX
    rep=TablaRegistros[12]&0x000000FF;//CL
    byt=(TablaRegistros[12]&0x0000FF00)>>8;//CH
    tipo=TablaRegistros[10]&0x000000FF;//AL
    //escribe por pantalla
    if (valor==2){
        for(int i=1;i<=rep;i++){
            tot=0;
            for (int j=1;j<=byt;j++){
                tot+=TablaMemoria[pos+j-1];
                if (j!=byt)
                    tot=tot<<8;
            }
            //asumimos que CH es como mucho 4. existe la posibilidad de que sea mayor?
            Escribe(tot,tipo);
            printf("\n");
        pos++;
        }
    }
    //lee por pantalla
    if (valor==1){
        for (int i=1;i<=rep;i++){
            Leer(tipo,&tot);
            for (int j=1;j<=byt;j++){
                TablaMemoria[pos]=(tot>>(byt-j)*8)&0x000000FF;
                pos++;
            }
        }
    }
}
void Leer(int tipo,long int *tot){
    if (tipo==1)
        scanf("%d",tot);
    if (tipo==2)
        scanf("%c",tot);
    if (tipo==4)
        scanf("%o",tot);
    if (tipo==8)
        scanf("%x",tot);
}
void Escribe(long int valor,int tipo){
    if (tipo==1)
        printf("%d ",valor);
    if (tipo==2)
        printf("%c ",valor);
    if (tipo==4)
        printf("%o ",valor);
    if (tipo==8)
        printf("%X ",valor);
    if (tipo==0x000F)
        //falta imprimir todo a la vez en esta solucion no imprime todos los byte en una linea, va byte a byte x linea
        printf("%c %X %o %d ",valor,valor,valor,valor);


}
void JMP(long int valor,long int TablaRegistros[]){
    TablaRegistros[5]=valor;
}
void JZ(long int valor,long int TablaRegistros[]){
    if(TablaRegistros[8]==0x40000000)
        JMP(valor,TablaRegistros);
}
void JP(long int valor,long int TablaRegistros[]){
    if(TablaRegistros[8]==0x00000000)
        JMP(valor,TablaRegistros);
}
void JN(long int valor,long int TablaRegistros[]){
    if(TablaRegistros[8]==0x80000000)
        JMP(valor,TablaRegistros);
}
void JNZ(long int valor,long int TablaRegistros[]){
    if(TablaRegistros[8]==0x80000000 || TablaRegistros[8]==0x00000000)
        JMP(valor,TablaRegistros);
}
void JNP(long int valor,long int TablaRegistros[]){
    if(TablaRegistros[8]==0x80000000 || TablaRegistros[8]==0x40000000)
        JMP(valor,TablaRegistros);
}
void JNN(long int valor,long int TablaRegistros[]){
    if(TablaRegistros[8]==0x40000000 || TablaRegistros[8]==0x00000000)
        JMP(valor,TablaRegistros);
}
void LDH(long int valor,long int TablaRegistros[]){
    TablaRegistros[9]=(TablaRegistros[9]&0x0000FFFF)+(valor&0x0000FFFF)<<16;
    //con los 2 bytes menos significativos del operando no se si directamente es del operando o del valor que este apunhta
}
void LDL(long int valor,long int TablaRegistros[]){
    TablaRegistros[9]=(TablaRegistros[9]&0xFFFF0000)+(valor&0x0000FFFF);
}
void RND(long int valor,long int TablaRegistros[]){
    srand(time(NULL));
    TablaRegistros[9]=rand()%valor;
}
void NOT(long int op,char top,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
long int aux=0,tiporeg,nz=0;
    if (top==0){
        long int valorm=Valor_mem(op,TablaMemoria,TablaRegistros,TablaDeDatos);
        nz=!valorm;
        MOV(op,top,valorm,TablaMemoria,TablaRegistros,TablaDeDatos);
    }else{
        tiporeg=op>>4;
        if (tiporeg==0){
            aux=!(TablaRegistros[(op&0x0F)]);
            nz=aux;
        }
        if (tiporeg==1){
            aux=(!(TablaRegistros[(op&0x0F)]&0x000000FF)&0x000000FF);
            nz=aux;
            aux+=(TablaRegistros[(op&0x0F)]&0xFFFFFF00);
        }
        if (tiporeg==2){
            aux=(!(TablaRegistros[(op&0x0F)]&0x0000FF00)&0x0000FF00);
            nz=aux>>8;
            aux+=(TablaRegistros[(op&0x0F)]&0xFFFF00FF);
        }
        if (tiporeg==3){
            aux=(!(TablaRegistros[(op&0x0F)]&0x0000FFFF)&0x0000FFFF);
            nz=aux;
            aux+=(TablaRegistros[(op&0x0F)]&0xFFFF0000);
        }
        TablaRegistros[(op&0x0F)]=aux;
    }
    Ultima_operacion(TablaRegistros,nz);
}

void Ultima_operacion(long int TablaRegistros[],long int nz){
    //efectuo los cambios en el CC.
    TablaRegistros[8]=0;// seteo el cc en 0 por si las dudas
    if (nz==0)
        TablaRegistros[8]=0x40000000; //segundo bit mas significativo en 1 por ser una operacion que dio 0
    if (nz<0)
        TablaRegistros[8]=0x80000000;  //primer bit mas significativo en 1 por ser una operacion que dio <0
}
long int Valor_mem(long int op,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    long int resultado=0;
    for (int i=0;i<=3;i++){
        if (((op&0x000F0000)>>16)==1)
            resultado+=(TablaMemoria[TablaDeDatos[1].pos+(op&0x0000FFFF)+i])&0x000000FF;
        else
            resultado+=(TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op&0x000F0000)>>16)]+(op&0x0000FFFF)+i])&0x000000FF;
        if (i!=3)
            resultado=resultado<<8;
    }
    return resultado;
}

