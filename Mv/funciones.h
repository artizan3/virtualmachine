#include<time.h>
#define MAX 16384
#define MAX2 255
//esto es para los discos
typedef struct{
    char arch_disk[MAX2][MAX2];
    int n;
}TDS;

typedef struct{
    short int pos,tamano;
}TDD;

typedef struct{
    char TablaMemoria[MAX];
    long int TablaRegistros[16];
    TDD TablaDeDatos[8];
    short int cant_seg;
    TDS tds;
}MV;

//variables globales del programa final
unsigned short int Toggle_debbug=0;
char direVMI[50]="";
unsigned int memory=16384;
char ah_aux;

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
void PUSH(long int valor,MV *mv);
void CALL(long int valor,MV *mv);
Func_1op_tipo2 *tipo2[]={0,JMP,JZ,JP,JN,JNZ,JNP,JNN,LDL,LDH,RND,0,PUSH,0,CALL};

typedef void Func_1op_tipo1(long int op,char top,MV *mv);
void NOT(long int op,char top,MV *mv);
void SYS(long int op,char top,MV *mv);
void POP(long int op,char top,MV *mv);
Func_1op_tipo1 *tipo1[]={SYS,0,0,0,0,0,0,0,0,0,0,NOT,0,POP};

void RET(MV *mv);

void Pre_Funcion(short int cant,long int op1,long int op2,char top1,char top2,char operacion,MV *mv);
long int Valor_op(long int op,char top,MV mv);
long int Valor_mem(long int op,MV mv);
void Ultima_operacion(MV *mv,long int nz);
void Integridad_op(long int op,MV mv);
void Leer(char tipo,long int *tot);
void Escribe(long int valor,unsigned char tipo,int tamano);
void EscribeString(char *palabra,unsigned int tamanostr,long int pos,MV *mv);
void LeeString(unsigned int tamanostr,long int pos,MV *mv);
void sys_segmento(MV *mv);
void sys_disco(MV *mv);
char comprobar_parametros_disco(short int ah,short int dl,short int ch,short int cl,short int dh,MV mv);
unsigned int max_disco(short int dl,MV mv);
unsigned long int movimiento_disco(short int ch,short int cl,short int dh,short int dl,MV mv);
char check_escritura_disco(short int al,short int ch,short int cl,short int dh,short int dl,long int EBX,MV mv);
char chek_lectura_disco(long int EBX,MV mv,short int al);
long int Suma_reg(char tipo,long int op,MV mv);
long int Valor_reg(char tipo,long int op,MV mv);
long int Mascara_registro(long int valor, char tipo);
long int puntero_memo(long int op,MV mv);
unsigned short int celdas_memo(long int op);
unsigned int memory_left(MV mv);

void Pre_Funcion(short int cant,long int op1,long int op2,char top1,char top2,char operacion,MV *mv){
    long int valor;
    if (cant==2){
        if (operacion!=3){
            valor=Valor_op(op2,top2,*mv);
            funciones[operacion](op1,top1,valor,mv);
        }else
            SWAP(op1,op2,top1,top2,mv);
    }else{
        if (cant==0)
            RET(mv);
        else
            if (operacion<=10 && operacion>=1 || operacion==12 || operacion==14){
                valor=Valor_op(op1,top1,*mv);
                tipo2[operacion](valor,mv);
            }else
                tipo1[operacion](op1,top1,mv);
    }
}

void MOV(long int op1,char top1,long int valor,MV *mv){
    char tiporeg=(op1>>4)&0x3;
    if (top1==0){
        long int pos_memo=puntero_memo(op1,*mv);
        unsigned short int cant=celdas_memo(op1)-1;
        for(int i=cant;i>=0;i--){
            (*mv).TablaMemoria[pos_memo+i]=(((valor))>>(cant-i)*8);
        }
    }else{
        valor=Mascara_registro(valor,tiporeg);
        (*mv).TablaRegistros[(op1&0x0F)]=Suma_reg(tiporeg,op1,*mv);
        (*mv).TablaRegistros[(op1&0x0F)]+=valor;
    }
}
void ADD(long int op1,char top1,long int valor,MV *mv){
    char tiporeg=(op1>>4)&0x3;
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
    char tiporeg=(op1>>4)&0x3;
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
    char tiporeg=(op1>>4)&0x3;
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
    char tiporeg=(op1>>4)&0x3;
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
    long int valor=Valor_op(op1,top1,*mv);
    Ultima_operacion(mv,valor-valor2);
}
void SHL(long int op1,char top1,long int valor,MV *mv){
    char tiporeg=(op1>>4)&0x3;
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
    char tiporeg=(op1>>4)&0x3;
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
    char tiporeg=(op1>>4)&0x3;
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
    char tiporeg=(op1>>4)&0x3;
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
    char tiporeg=(op1>>4)&0x3;
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
    int rep,byt;
    char tipo;
    unsigned int tamanostr;
    valor=Valor_op(op,top,*mv);
    //preparo pos en edx
        long int regpointer=((*mv).TablaRegistros[13]&0xFFFF0000)>>16;
        long int memo=(*mv).TablaDeDatos[regpointer].pos;
        long int offset=((*mv).TablaRegistros[13]&0x0000FFFF);//DX
        pos=memo+offset;
        rep=(*mv).TablaRegistros[12]&0x000000FF;//CL
        byt=((*mv).TablaRegistros[12]&0x0000FF00)>>8;//CH
        tipo=(*mv).TablaRegistros[10]&0x000000FF;//AL
        tamanostr=(*mv).TablaRegistros[12]&0x0000FFFF;//CX
    //escribe por pantalla
    if (valor==2){
        for(int i=1;i<=rep;i++){
            tot=0;
            for (int j=1;j<=byt;j++){
                tot+=((*mv).TablaMemoria[pos+j-1])&0xFF;
                if (j!=byt)
                    tot=tot<<8;
            }
            //Muestra_mem(pos-(*mv).TablaDeDatos[1].pos);
            Muestra_mem(pos-memo);
            Escribe(tot,tipo,byt);
            printf("\n");
            pos+=byt;
        }
    }
    //lee por pantalla
    if (valor==1){
        for (int i=1;i<=rep;i++){
            Muestra_mem(pos-memo);
            Leer(tipo,&tot);
            for (int j=1;j<=byt;j++){
                (*mv).TablaMemoria[pos+j-1]=(tot>>(byt-j)*8)&0x000000FF;
            }
            pos+=byt;
        }
    }
    if (valor==3){
        char palabra[1600];
        gets(palabra);
        fflush(stdin);
        EscribeString(palabra,tamanostr,pos,mv);
    }
    if (valor==4){
        LeeString(tamanostr,pos,mv);
    }
    if (valor==7)
        system("clear");
    if (valor==0xE)
        sys_segmento(mv);
    if (valor==0xD)
        sys_disco(mv);
    if (valor==0xF && strcmp(direVMI,"")!=0)
        Toggle_debbug=1;
}
void Leer(char tipo,long int *tot){
    if (tipo==1)
        scanf("%d",tot);
    if (tipo==2)
        scanf("%c",tot);
    if (tipo==4)
        scanf("%o",tot);
    if (tipo==8)
        scanf("%x",tot);
}
void Escribe(long int valor,unsigned char tipo,int tamano){
    unsigned char u;
    long int aux;
        u=tipo&0x1;
        if (u!=0)
            printf("#%d ",valor);
        u=tipo&0x2;
        if (u!=0){
            printf("'");
            //mostrar byte a byte
            for(int i=1;i<=tamano;i++){
                aux=(valor>>(tamano-i)*8)&0x000000FF;
                if (aux>=32 && aux<=126)
                    printf("%c",aux);
                else
                    printf(".");
            }
            printf(" ");
        }
        u=tipo&0x4;
        if (u!=0)
            printf("@%o ",valor);
        u=tipo&0x8;
        if (u!=0)
            printf("%c%X ",'%',valor);
    }
void EscribeString(char *palabra,unsigned int tamanostr,long int pos,MV *mv){
    int i=0;
    while (i<strlen(palabra) && i!=tamanostr){
        (*mv).TablaMemoria[pos+i]=palabra[i];
        i++;
    }
    if (i==tamanostr)
        (*mv).TablaMemoria[pos+i]=0x00;
    else
        (*mv).TablaMemoria[pos+i+1]=0x00;
}
void LeeString(unsigned int tamanostr,long int pos,MV *mv){
    int i=0;
    while ((*mv).TablaMemoria[pos+i]!=0x00 && i!=tamanostr){
        printf("%c",(*mv).TablaMemoria[pos+i]);
        i++;
    }
}
void sys_segmento(MV *mv){
    int i;
    int op_seg=(*mv).TablaRegistros[10]&0x0000FFFF;
    int tamano=(*mv).TablaRegistros[12]&0x0000FFFF;
    if (op_seg==0 || op_seg==1){
        if (op_seg==0){
            int puntero=(*mv).TablaRegistros[11]>>16;
            if ((*mv).TablaDeDatos[puntero].pos!=-1 && puntero<7 && puntero>=0){
                (*mv).TablaRegistros[12]&=0xFFFF0000;
                (*mv).TablaRegistros[12]+=(*mv).TablaDeDatos[puntero].tamano;
                (*mv).TablaRegistros[10]&=0xFFFF0000;
            }else{
                (*mv).TablaRegistros[10]=(*mv).TablaRegistros[10]&0xFFFF0000;
                (*mv).TablaRegistros[10]+=49;
                (*mv).TablaRegistros[12]&=0xFFFF0000;
            }
        }else{//va a aniadir un segmento :)
            if (tamano<memory_left(*mv)){//si hay memoria dispo
                int aux=0;
                i=0;
                while (i!=8 && (*mv).TablaDeDatos[i].pos!=-1){
                    aux+=(*mv).TablaDeDatos[i].tamano;
                    i++;
                }
                if (i==8){//ya esta el limite maximo de segmentos
                    (*mv).TablaRegistros[11]=-1;//EBX=-1;
                    (*mv).TablaRegistros[10]=(*mv).TablaRegistros[10]&0xFFFF0000;
                    (*mv).TablaRegistros[10]+=(0x0000FFFF);
                }else{//se crea el segmento
                    (*mv).TablaRegistros[11]=0;
                    (*mv).TablaRegistros[11]+=i<<16;
                    (*mv).TablaDeDatos[i].pos=aux;
                    (*mv).TablaDeDatos[i].tamano=tamano;
                    (*mv).TablaRegistros[10]&=0xFFFF0000;//ax=00
                }
            }else{//si no hay espacio para alojar en nuevo segmento
                (*mv).TablaRegistros[11]=-1;//EBX=-1;
                (*mv).TablaRegistros[10]&=0xFFFF0000;
                (*mv).TablaRegistros[10]+=(0xCC);//AH=CC
            }
        }
    }else//si la operacion no existe
        (*mv).TablaRegistros[10]=(*mv).TablaRegistros[10]&0xFFFF0000+1;
}
void sys_disco(MV *mv){
    FILE *arch;
    short int al=(*mv).TablaRegistros[10]&0x000000FF;//cantidad de sectores para leer o escribir
    short int ah=((*mv).TablaRegistros[10]&0x0000FF00)>>8;//accion y retroalimentacion
    short int cl=(*mv).TablaRegistros[12]&0x000000FF;//num cabeza
    short int ch=((*mv).TablaRegistros[12]&0x0000FF00)>>8;//num cilindro
    short int dl=(*mv).TablaRegistros[13]&0x000000FF;//num disco
    short int dh=((*mv).TablaRegistros[13]&0x0000FF00)>>8;//num sector
    long int EBX=(*mv).TablaRegistros[11];//EBX indica la primer celda del buffer de lectura/escritura
    char aux=comprobar_parametros_disco(ah,dl,ch,cl,dh,*mv);
    if (aux==0){
        if (ah==0){//Consultar último estado
            ah=ah_aux;
            (*mv).TablaRegistros[10]&=0xFFFF00FF;
            (*mv).TablaRegistros[10]+=(ah<<8)&0x0000FF00;
        }
        if (ah==0x02){//lectura
            aux=chek_lectura_disco(EBX,*mv,al);
            (*mv).TablaRegistros[10]&=0xFFFF00FF;
            if(aux==0){
                FILE *arch=fopen((*mv).tds.arch_disk[dl],"rb");
                fseek(arch,movimiento_disco(ch,cl,dh,dl,*mv),SEEK_SET);
                char dato;
                int puntero=(EBX&0xFFFF0000)>>16; //deberia apuntar despues de extrasegment a partir de 64
                int offset=(EBX&0x0000FFFF);
                int i=0;
                for (i;i<al*512;i++){
                    fread(&dato,sizeof(dato),1,arch);
                    (*mv).TablaMemoria[(*mv).TablaDeDatos[puntero].pos+offset+i]=dato;
                }
                //preguntar si se quiere leer sector inexistente pero dentro de parametros de disco se devuelve 0 y se extiende arch
                fclose(arch);
                ah=0;
                al=(i+1)/512;
                (*mv).TablaRegistros[10]&=0xFFFF0000;//ah=0
                (*mv).TablaRegistros[10]+=(al)&0xFF;//al=sectores tranferidos
            }else{
                ah=aux;
                (*mv).TablaRegistros[10]+=(ah<<8)&0x0000FF00;
            }
        }
        if (ah==0x03){//escritura
            aux=check_escritura_disco(al,ch,cl,dh,dl,EBX,*mv);
            if (aux==0){//aca realiza la escritura
                FILE *arch=fopen((*mv).tds.arch_disk[dl],"r+b");
                fseek(arch,movimiento_disco(ch,cl,dh,dl,*mv),SEEK_SET);
                char dato;
                int puntero=(EBX&0xFFFF0000)>>16;
                int offset=(EBX&0x0000FFFF);
                for (int i=0;i<al*512;i++){
                    dato=(*mv).TablaMemoria[(*mv).TablaDeDatos[puntero].pos+offset+i];
                    fwrite(&dato,sizeof(dato),1,arch);
                }
                fclose(arch);
                ah=0;//aca el feedback
                (*mv).TablaRegistros[10]&=0xFFFF00FF;//operacion exitosa
            }else
                ah=aux;
                (*mv).TablaRegistros[10]&=0xFFFF00FF;
                (*mv).TablaRegistros[10]+=(ah<<8)&0x0000FF00;
        }
        if (ah==0x08){
            int i=33;
            arch=fopen((*mv).tds.arch_disk[dl],"rb");
            fseek(arch, i, SEEK_SET);
            fread(&ch,sizeof(char),1,arch);//guardar en reg
            (*mv).TablaRegistros[12]&=0xFFFF00FF;
            (*mv).TablaRegistros[12]+=(ch<<8);
            fread(&cl,sizeof(char),1,arch);//guardar en reg
            (*mv).TablaRegistros[12]&=0xFFFFFF00;
            (*mv).TablaRegistros[12]+=(cl);
            fread(&dh,sizeof(char),1,arch);//guardar en reg
            (*mv).TablaRegistros[13]&=0xFFFF00FF;
            (*mv).TablaRegistros[13]+=(dh<<8);
            ah=0;
            (*mv).TablaRegistros[10]&=0xFFFF00FF;//guarda ah
        }
    }else{
        ah=aux;
        (*mv).TablaRegistros[10]&=0xFFFF00FF;
        (*mv).TablaRegistros[10]+=(ah<<8)&0x0000FF00;
    }
    ah_aux=ah;
}
char comprobar_parametros_disco(short int ah,short int dl,short int ch,short int cl,short int dh,MV mv){
    int aux;
    unsigned char aux2;
    FILE *arch;
    if (ah!=0x00 && ah!=0x02 && ah!=0x03 && ah!=0x08){
        aux=0x01;//si la op es incorrecta
    }else{
        if (dl>mv.tds.n)
            aux=0x31;//si el numero de disco no existe
        else{
            arch=fopen(mv.tds.arch_disk[dl],"rb");
            int i=33;
            fseek(arch, i, SEEK_SET);
            fread(&aux2,sizeof(char),1,arch);
            if (ch>=aux2)
                aux=0x0B;//si el numero de cilindro no existe
            else{
                fread(&aux2,sizeof(char),1,arch);
                if (cl>=aux2)
                    aux=0x0C;//si el numero de cabeza no existe
                else{
                    fread(&aux2,sizeof(char),1,arch);
                    if (dh>=aux2)
                        aux=0x0D;//si el numero de sector no existe
                    else
                        aux=0;//si esta en condiciones
                }
            }
            fclose(arch);
        }
    }
    return aux;
}
unsigned int max_disco(short int dl,MV mv){
    unsigned char tot_dis,tot_h,tot_sec;
    int i=33;
    FILE *arch=fopen(mv.tds.arch_disk[dl],"rb");
    fseek(arch, i, SEEK_SET);
    fread(&tot_dis,sizeof(char),1,arch);//guardar en reg
    fread(&tot_h,sizeof(char),1,arch);//guardar en reg
    fread(&tot_sec,sizeof(char),1,arch);//guardar en reg
    fclose(arch);
    return (1+tot_dis*tot_h*tot_sec)*512;
}
unsigned long int movimiento_disco(short int ch,short int cl,short int dh,short int dl,MV mv){
    unsigned char cant_cab,cant_sec;
    int i=34;
    FILE *arch=fopen(mv.tds.arch_disk[dl],"rb");
    fseek(arch, i, SEEK_SET);
    fread(&cant_cab,sizeof(char),1,arch);//guardar en reg
    fread(&cant_sec,sizeof(char),1,arch);//guardar en reg
    fclose(arch);
    return (1+ch*cant_cab*cant_sec+cl*cant_sec+dh)*512;
}
char check_escritura_disco(short int al,short int ch,short int cl,short int dh,short int dl,long int EBX,MV mv){
    unsigned long int stop_disk=max_disco(dl,mv);//valor total del disco
    int offset=EBX&0x0000FFFF;
    int pointer=(EBX&0xFFFF0000)>>16;
    int tamano=mv.TablaDeDatos[pointer].tamano-offset;//me fijo si tengo espacio para leer de la memo
    long int resta_final=stop_disk-movimiento_disco(ch,cl,dh,dl,mv)-al*512;//me fijo si tengo espacio para escribir en el disco
    if (resta_final>=0){
        if (tamano>=al*512-1){
            return 0;
        }else
            return 0xCC;
    }else
        return 0xCC;
}
char chek_lectura_disco(long int EBX,MV mv,short int al){
    int offset=EBX&0x0000FFFF;
    int pointer=(EBX&0xFFFF0000)>>16;
    int tamano=mv.TablaDeDatos[pointer].tamano-offset;
    if(tamano>=al*512-1)
        return 0;
    else
        return 0x04;
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
        valorm=~valorm;
        nz=valorm;
        MOV(op,top,valorm,mv);
    }else{
        aux=Valor_reg(tiporeg,op,*mv);
        aux=~aux;
        aux=Mascara_registro(aux,tiporeg);
        nz=aux;
        if (tiporeg!=0)
            aux+=Suma_reg(tiporeg,op,*mv);
        (*mv).TablaRegistros[(op&0x0F)]=aux;
    }
    Ultima_operacion(mv,nz);
}
void PUSH(long int valor,MV *mv){
    int ss_pointer=(((*mv).TablaRegistros[6]&0xFFFF0000)>>16);
    int sp_offset=(*mv).TablaRegistros[6]&0x0000FFFF;
    int pos_ss=(*mv).TablaDeDatos[ss_pointer].pos;
    if (sp_offset-4<0){
        printf("ERROR(5): Stack overflow error");
        exit(-1);
    }
    for (int i=0;i<=3;i++){
        sp_offset-=1;
        (*mv).TablaMemoria[pos_ss+sp_offset]=(valor>>(i*8));
    }
    (*mv).TablaRegistros[6]=(*mv).TablaRegistros[6]&0xFFFF0000;
    (*mv).TablaRegistros[6]+=sp_offset;
}
void POP(long int op,char top,MV *mv){
    long int aux=0;
    int ss_pointer=(((*mv).TablaRegistros[4]&0xFFFF0000)>>16);
    int sp_offset=(*mv).TablaRegistros[6]&0x0000FFFF;
    int pos_ss=(*mv).TablaDeDatos[ss_pointer].pos;
    if (sp_offset+4>(*mv).TablaDeDatos[ss_pointer].tamano){
        printf("ERROR(6): Stack underflow error");
        exit(-1);
    }
    for (int i=0;i<=3;i++){
        aux+=((*mv).TablaMemoria[pos_ss+sp_offset])&0xFF;
        if (i!=3)
            aux=aux<<8;
        sp_offset++;
    }
    (*mv).TablaRegistros[6]=(*mv).TablaRegistros[6]&0xFFFF0000;
    (*mv).TablaRegistros[6]+=sp_offset;
    MOV(op,top,aux,mv);
}
void CALL(long int valor,MV *mv){
    PUSH((*mv).TablaRegistros[5],mv);
    JMP(valor,mv);
}
void RET(MV *mv){
    POP(0x05,2,mv);
}
long int Valor_op(long int op,char top,MV mv){
    long int tiporeg=op>>4;;
    long int resultado=0;
    if (top==2){
            if (tiporeg==3)
                resultado=(short int)(mv.TablaRegistros[(op&0x0F)]&0x0000FFFF);
            if (tiporeg==2)
                resultado=(char)((mv.TablaRegistros[(op&0x0F)]&0x0000FF00)>>8);
            if (tiporeg==1)
                resultado=(char)(mv.TablaRegistros[(op&0x0F)]&0x000000FF);
            if (tiporeg==0)
                resultado=mv.TablaRegistros[(op&0x0F)];
    }
    if (top==0){
       resultado=Valor_mem(op,mv);
    }
    if (top==1){
        resultado=(short int)op;
    }
    return (resultado);
}
long int Valor_mem(long int op,MV mv){
    long int resultado=0;
    long int pos_memo=puntero_memo(op,mv);
    unsigned short int cant=celdas_memo(op);
    for (int i=0;i<cant;i++){
        resultado+=(mv.TablaMemoria[pos_memo+i])&0x000000FF;
        if (i!=cant-1)
            resultado=resultado<<8;
    }
    long int aux=resultado;
    if (cant==1)
        resultado=(char)aux;
    if (cant==2)
        resultado=(short int)aux;
    return resultado;

}
void Ultima_operacion(MV *mv,long int nz){
    (*mv).TablaRegistros[8]=0;
    if (nz==0)
        (*mv).TablaRegistros[8]=0x40000000;
    if (nz<0)
        (*mv).TablaRegistros[8]=0x80000000;
}
void Integridad_op(long int op,MV mv){
    int valor=0;
    short int offset=op&0x00FFFF;
    int treg=(op&0x00FF0000)>>16;
    long int reg=mv.TablaRegistros[treg];
    long int reg_pointer=(reg>>16)&0xFF;
    short int reg_offset=reg;
    int pos0=mv.TablaDeDatos[reg_pointer].tamano;
    int pos1=mv.TablaDeDatos[reg_pointer].pos;
        valor=offset+reg_offset;
        if (valor>=pos0 || pos0+valor<0 || pos1+valor<pos1){
            printf("ERROR (3): Segmentation fault");
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
long int puntero_memo(long int op,MV mv){
    short int offset=(op&0x0000FFFF);
    int treg=(op&0x000F0000)>>16;
    long int reg=mv.TablaRegistros[treg];
    long int reg_pointer=reg>>16;
    short int reg_offset=(reg&0x0000FFFF);
    return mv.TablaDeDatos[reg_pointer].pos+reg_offset+offset;
}
unsigned short int celdas_memo(long int op){
    unsigned long int aux=op&(0x00C00000);
    unsigned int aux2=aux>>22;
    aux2=aux2^0x3;
    aux2++;
    return aux2;
}
unsigned int memory_left(MV mv){
    int i=0;
    unsigned int aux=memory;
    while (mv.TablaDeDatos[i].pos!=-1){
        aux-=mv.TablaDeDatos[i].tamano;
        i++;
    }
    return aux;
}
