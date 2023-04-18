#include<time.h>
#define MAX 16384
typedef struct{
    long int pos,tamano;
}TDD;

typedef struct{
    char TablaMemoria[MAX];
    long int TablaRegistros[16];
    TDD TablaDeDatos[2];
}MV;

typedef void Funciones2op(long int op1,char top1,long int valor,MV *mv);
void MOV(long int op1,char top1,long int valor,MV *mv);
void ADD(long int op1,char top1,long int valor,MV *mv);
void SUB(long int op1,char top1,long int valor,MV *mv);
void SWAP(long int op1,long int op2,char top1,char top2,MV *mv);
void MUL(long int op1,char top1,long int valor,MV *mv);
void DIV(long int op1,char top1,long int valor,MV *mv);
void CMP(long int op1,char top1,long int valor,MV *mv);
void SHL(long int op1,char top1,long int valor2,MV *mv);
void SHR(long int op1,char top1,long int valor2,MV *mv);
void AND(long int op1,char top1,long int valor2,MV *mv);
void OR(long int op1,char top1,long int valor2,MV *mv);
void XOR(long int op1,char top1,long int valor2,MV *mv);
Funciones2op *funciones[]={MOV,ADD,SUB,0,MUL,DIV,CMP,SHL,SHR,AND,OR,XOR};

typedef void Func_1op_tipo2(long int valor,MV *mv);
void JMP(long int valor,MV *mv);
void JZ(long int valor,MV *mv);
void JP(long int valor,MV *mv);
void JN(long int valor,MV *mv);
void JNZ(long int valor,MV *mv);
void JNP(long int valor,MV *mv);
void JNN(long int valor,MV *mv);
void LDH(long int valor,MV *mv);
void LDL(long int valor,MV *mv);
void RND(long int valor,MV *mv);
Func_1op_tipo2 *tipo2[]={0,JMP,JZ,JP,JN,JNZ,JNP,JNN,LDL,LDH,RND,0};

typedef void Func_1op_tipo1(long int op,char top,MV *mv);
void NOT(long int op,char top,MV *mv);
void SYS(long int op,char top,MV *mv);
Func_1op_tipo1 *tipo1[]={SYS,0,0,0,0,0,0,0,0,0,0,NOT};

void Pre_Funcion(short int cant,long int op1,long int op2,char top1,char top2,char operacion,MV *mv);
long int Valor_op(long int op,char top,MV mv);
long int Valor_mem(long int op,MV mv);
void Ultima_operacion(MV *mv,long int nz);
void Integridad_op(long int op,MV mv);
void Leer(int tipo,long int *tot);
void Escribe(long int valor,int tipo);
long int Suma_reg(char tipo,long int op,MV mv);
long int Valor_reg(char tipo,long int op,MV mv);
long int Mascara_registro(long int valor, char tipo);

void Pre_Funcion(short int cant,long int op1,long int op2,char top1,char top2,char operacion,MV *mv){
    long int valor;
    if (cant==2){
        if (operacion!=3){
            valor=Valor_op(op2,top2,*mv);
            funciones[operacion](op1,top1,valor,mv);
        }else
            SWAP(op1,op2,top1,top2,mv);
    }else{
        if (operacion<=10 && operacion>=1){
            valor=Valor_op(op1,top1,*mv);
            tipo2[operacion](valor,mv);
        }
        else
           tipo1[operacion](op1,top1,mv);
    }
}

void MOV(long int op1,char top1,long int valor,MV *mv){
    char tiporeg=op1>>4;
    if (top1==0){
            for(int i=0;i<=3;i++){
                if (((op1&0x000F0000)>>16)==1)
                    (*mv).TablaMemoria[(*mv).TablaDeDatos[1].pos+(op1&0x0000FFFF)+i]=(((valor))>>(3-i)*8)&0x000000FF;
                else
                    (*mv).TablaMemoria[(*mv).TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)+i]=(((valor))>>(3-i)*8);
            }
        }else{
            valor=Mascara_registro(valor,tiporeg);
            (*mv).TablaRegistros[(op1&0x0F)]=Suma_reg(tiporeg,op1,*mv);
            (*mv).TablaRegistros[(op1&0x0F)]+=valor;
        }
}
void ADD(long int op1,char top1,long int valor,MV *mv){
    char tiporeg=op1>>4;
    long int aux=0,nz=0;
    if (top1==0){
        long int valorm=Valor_mem(op1,*mv);
        nz=valorm+valor;
        valorm=nz;
        MOV(op1,top1,valorm,mv);
    }else{
        aux=Valor_reg(tiporeg,op1,*mv);
        aux+=valor;
        nz=aux;
        aux=Mascara_registro(aux,tiporeg);
        if (tiporeg!=0)
            aux+=Suma_reg(tiporeg,op1,*mv);
        (*mv).TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(mv,nz);
}
void SUB(long int op1,char top1,long int valor,MV *mv){
    char tiporeg=op1>>4;
    long int aux=0,nz=0;
    if (top1==0){
        long int valorm=Valor_mem(op1,*mv);
        nz=valorm-valor;
        valorm=nz;
        MOV(op1,top1,valorm,mv);
    }else{
        aux=Valor_reg(tiporeg,op1,*mv);
        aux-=valor;
        nz=aux;
        aux=Mascara_registro(aux,tiporeg);
        if (tiporeg!=0)
            aux+=Suma_reg(tiporeg,op1,*mv);
        (*mv).TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(mv,nz);
}
void SWAP(long int op1,long int op2,char top1,char top2,MV *mv){
    long int aux,aux2;
    if (top1==top2){
        if (top1==0){
           aux=Valor_mem(op1,*mv);
           aux2=Valor_mem(op2,*mv);
        }else{
            aux=Valor_op(op1,top1,*mv);
            aux2=Valor_op(op2,top2,*mv);
        }
        MOV(op1,top1,aux2,mv);
        MOV(op2,top2,aux,mv);
    }
}
void MUL(long int op1,char top1,long int valor,MV *mv){
    char tiporeg=op1>>4;
    long int aux=0,nz=0;
    if (top1==0){
        long int valorm=Valor_mem(op1,*mv);
        nz=valorm*valor;
        valorm=nz;
        MOV(op1,top1,valorm,mv);
    }else{
        aux=Valor_reg(tiporeg,op1,*mv);
        aux*=valor;
        nz=aux;
        aux=Mascara_registro(aux,tiporeg);
        if (tiporeg!=0)
            aux+=Suma_reg(tiporeg,op1,*mv);
        (*mv).TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(mv,nz);
}
void DIV(long int op1,char top1,long int valor,MV *mv){
    char tiporeg=op1>>4;
    long int aux=0,nz=0;
    if(valor!=0){
        if (top1==0){
            long int valorm=Valor_mem(op1,*mv);
            (*mv).TablaRegistros[9]=valorm%valor;
            nz=valorm/valor;
            valorm=nz;
            MOV(op1,top1,valorm,mv);
        }else{
            aux=Valor_reg(tiporeg,op1,*mv);
            (*mv).TablaRegistros[9]=aux%valor;
            aux/=valor;
            nz=aux;
            aux=Mascara_registro(aux,tiporeg);
            if (tiporeg!=0)
                aux+=Suma_reg(tiporeg,op1,*mv);
            (*mv).TablaRegistros[(op1&0x0F)]=aux;
        }
        Ultima_operacion(mv,nz);
    }else{
        printf("ERROR(2): no se puede dividir por 0");
        exit(-1);
    }
}
void CMP(long int op1,char top1,long int valor2,MV *mv){
    char tiporeg;
    long int valor=Valor_op(op1,top1,*mv);
    Ultima_operacion(mv,valor-valor2);
}
void SHL(long int op1,char top1,long int valor,MV *mv){
    char tiporeg=op1>>4;
    long int aux,nz=0;
    if (top1==0){
        long int valorm=Valor_mem(op1,*mv);
        nz=valorm<<valor;
        valorm=nz;
        MOV(op1,top1,valorm,mv);
        }else{
        aux=Valor_reg(tiporeg,op1,*mv);
        aux=aux<<valor;
        nz=aux;
        aux=Mascara_registro(aux,tiporeg);
        if (tiporeg!=0)
            aux+=Suma_reg(tiporeg,op1,*mv);
        (*mv).TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(mv,nz);
}
void SHR(long int op1,char top1,long int valor,MV *mv){
    char tiporeg;
    long int aux,nz=0;
    if (top1==0){
        long int valorm=Valor_mem(op1,*mv);
        nz=valorm>>valor;
        valorm=nz;
        MOV(op1,top1,valorm,mv);
    }else{
        aux=Valor_reg(tiporeg,op1,*mv);
        aux=aux>>valor;
        nz=aux;
        aux=Mascara_registro(aux,tiporeg);
        if (tiporeg!=0)
            aux+=Suma_reg(tiporeg,op1,*mv);
        (*mv).TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(mv,nz);
}
void AND(long int op1,char top1,long int valor,MV *mv){
    char tiporeg=op1>>4;
    long int aux=0,nz=0;
    if (top1==0){
        long int valorm=Valor_mem(op1,*mv);
        nz=valorm&valor;
        valorm=nz;
        MOV(op1,top1,valorm,mv);
    }else{
        aux=Valor_reg(tiporeg,op1,*mv);
        aux&=valor;
        nz=aux;
        aux=Mascara_registro(aux,tiporeg);
        if (tiporeg!=0)
            aux+=Suma_reg(tiporeg,op1,*mv);
        (*mv).TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(mv,nz);
}
void OR(long int op1,char top1,long int valor,MV *mv){
    char tiporeg=op1>>4;
    long int aux=0,nz=0;
    if (top1==0){
        long int valorm=Valor_mem(op1,*mv);
        nz=valorm|valor;
        valorm=nz;
        MOV(op1,top1,valorm,mv);
    }else{
        aux=Valor_reg(tiporeg,op1,*mv);
        aux|=valor;
        nz=aux;
        aux=Mascara_registro(aux,tiporeg);
        if (tiporeg!=0)
            aux+=Suma_reg(tiporeg,op1,*mv);
        (*mv).TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(mv,nz);
}
void XOR(long int op1,char top1,long int valor,MV *mv){
    char tiporeg;
    long int aux=0,nz=0;
    if (top1==0){
        long int valorm=Valor_mem(op1,*mv);
        nz=valorm^valor;
        valorm=nz;
        MOV(op1,top1,valorm,mv);
    }else{
        aux=Valor_reg(tiporeg,op1,*mv);
        aux^=valor;
        nz=aux;
        aux=Mascara_registro(aux,tiporeg);
        if (tiporeg!=0)
            aux+=Suma_reg(tiporeg,op1,*mv);
        (*mv).TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(mv,nz);
}

void SYS(long int op,char top,MV *mv){
    long int valor,pos,tot;
    int rep,byt,tipo;
    valor=Valor_op(op,top,*mv);
    pos=(*mv).TablaRegistros[13];//EDX
    rep=(*mv).TablaRegistros[12]&0x000000FF;//CL
    byt=((*mv).TablaRegistros[12]&0x0000FF00)>>8;//CH
    tipo=(*mv).TablaRegistros[10]&0x000000FF;//AL
    //escribe por pantalla
    if (valor==2){
        for(int i=1;i<=rep;i++){
            tot=0;
            for (int j=1;j<=byt;j++){
                tot+=((*mv).TablaMemoria[pos+j-1])&0x000000FF;
                if (j!=byt)
                    tot=tot<<8;
            }
            Escribe(tot,tipo);
            printf("\n");
            pos+=byt;
        }
    }
    //lee por pantalla
    if (valor==1){
        for (int i=1;i<=rep;i++){
            Leer(tipo,&tot);
            for (int j=1;j<=byt;j++){
                (*mv).TablaMemoria[pos]=(tot>>(byt-j)*8)&0x000000FF;
                pos+=byt;
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
    if (tipo==2){
        if (valor>=32 && valor<=254)
            printf("%c ",valor);
        else
            printf(". ");
    }
    if (tipo==4)
        printf("%o ",valor);
    if (tipo==8)
        printf("%X ",valor);
    if (tipo==0x000F)
        printf("%c %X %o %d ",valor,valor,valor,valor);
}
void JMP(long int valor,MV *mv){
    (*mv).TablaRegistros[5]=valor;
}
void JZ(long int valor,MV *mv){
    if((*mv).TablaRegistros[8]==0x40000000)
        JMP(valor,mv);
}
void JP(long int valor,MV *mv){
    if((*mv).TablaRegistros[8]==0x00000000)
        JMP(valor,mv);
}
void JN(long int valor,MV *mv){
    if((*mv).TablaRegistros[8]==0x80000000)
        JMP(valor,mv);
}
void JNZ(long int valor,MV *mv){
    if((*mv).TablaRegistros[8]==0x80000000 || (*mv).TablaRegistros[8]==0x00000000)
        JMP(valor,mv);
}
void JNP(long int valor,MV *mv){
    if((*mv).TablaRegistros[8]==0x80000000 || (*mv).TablaRegistros[8]==0x40000000)
        JMP(valor,mv);
}
void JNN(long int valor,MV *mv){
    if((*mv).TablaRegistros[8]==0x40000000 || (*mv).TablaRegistros[8]==0x00000000)
        JMP(valor,mv);
}
void LDH(long int valor,MV *mv){
    (*mv).TablaRegistros[9]=((*mv).TablaRegistros[9]&0x0000FFFF)+((valor&0x0000FFFF)<<16);
}
void LDL(long int valor,MV *mv){
    (*mv).TablaRegistros[9]=((*mv).TablaRegistros[9]&0xFFFF0000)+(valor&0x0000FFFF);
}
void RND(long int valor,MV *mv){
    srand(time(NULL));
    (*mv).TablaRegistros[9]=rand()%valor;
}
void NOT(long int op,char top,MV *mv){
    long int aux=0,tiporeg=op>>4,nz=0;
    if (top==0){
        long int valorm=Valor_mem(op,*mv);
        valorm!=valorm;
        nz=!valorm;
        MOV(op,top,valorm,mv);
    }else{
        aux=Valor_reg(tiporeg,op,*mv);
        aux=!aux;
        aux=Mascara_registro(aux,tiporeg);
        nz=aux;
        if (tiporeg!=0)
            aux+=Suma_reg(tiporeg,op,*mv);
        (*mv).TablaRegistros[(op&0x0F)]=aux;
    }
    Ultima_operacion(mv,nz);
}
long int Valor_op(long int op,char top,MV mv){
    char tiporeg;
    long int resultado=0;
    if (top==2){
        if (op==1)
            resultado=mv.TablaDeDatos[1].pos;
        else{
            tiporeg=op>>4;
            if (tiporeg==3)
                resultado=(mv.TablaRegistros[(op&0x0F)]&0x0000FFFF);
            if (tiporeg==2)
                resultado=((mv.TablaRegistros[(op&0x0F)]&0x0000FF00)>>8);
            if (tiporeg==1)
                resultado=(mv.TablaRegistros[(op&0x0F)]&0x000000FF);
            if (tiporeg==0)
                resultado=mv.TablaRegistros[(op&0x0F)];
        }
    }
    if (top==0){
       resultado=Valor_mem(op,mv);
    }
    if (top==1)
        resultado=(op);
    return (resultado);
}
void Ultima_operacion(MV *mv,long int nz){
    (*mv).TablaRegistros[8]=0;
    if (nz==0)
        (*mv).TablaRegistros[8]=0x40000000;
    if (nz<0)
        (*mv).TablaRegistros[8]=0x80000000;
}
long int Valor_mem(long int op,MV mv){
    long int resultado=0;
    for (int i=0;i<=3;i++){
        if (((op&0x000F0000)>>16)==1)
            resultado+=(mv.TablaMemoria[mv.TablaDeDatos[1].pos+(op&0x0000FFFF)+i])&0x000000FF;
        else
            resultado+=(mv.TablaMemoria[mv.TablaRegistros[((op&0x000F0000)>>16)]+(op&0x0000FFFF)+i])&0x000000FF;
        if (i!=3)
            resultado=resultado<<8;
    }
    return resultado;
}
void Integridad_op(long int op,MV mv){
    char pos0=mv.TablaDeDatos[1].pos,reg=((op&0x00FF0000)>>16);
    short int valor=0,offset=(op&0x0000FFFF);
    if (reg==0){
        valor=offset;
    }else{
        if (reg==1)
            valor=pos0+offset;
        else
            valor=mv.TablaRegistros[reg]+offset;
    }
    if (valor<pos0){
        printf("ERROR (3): estas entrando al CS");
        exit(-1);
    }
}
long int Valor_reg(char tipo,long int op,MV mv){
    long int resultado=0;
    if (tipo==0)
        resultado=mv.TablaRegistros[(op&0x0F)];
    if (tipo==1)
        resultado=mv.TablaRegistros[(op&0x0F)]&0x000000FF;
    if (tipo==2)
        resultado=(mv.TablaRegistros[(op&0x0F)]&0x0000FF00)>>8;
    if (tipo==3)
        resultado=mv.TablaRegistros[(op&0x0F)]&0x0000FFFF;
    return resultado;
}
long int Suma_reg(char tipo,long int op,MV mv){
    long int resultado=0;
    if (tipo==1)
        resultado=mv.TablaRegistros[(op&0x0F)]&0xFFFFFF00;
    if (tipo==2)
        resultado=mv.TablaRegistros[(op&0x0F)]&0xFFFF00FF;
    if (tipo==3)
        resultado=mv.TablaRegistros[(op&0x0F)]&0xFFFF0000;
    return resultado;
}
long int Mascara_registro(long int valor, char tipo){
    long int resultado=0;
    if (tipo==0)
        resultado=valor;
    if (tipo==1)
        resultado=valor&0x000000FF;
    if (tipo==2)
        resultado=(valor&0x0000FF)<<8;
    if (tipo==3)
        resultado=valor&0x000FFFF;
    return resultado;
}
