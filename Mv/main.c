#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"
#include "dissasembler.h"

void Inicia_memoria(char *dire,MV *mv,unsigned int memory);
void Inicia_registro(MV *mv);
void Cant_op(char instruccion,char *top1,char *top2,short int *cant);
void Lectura(MV *mv);
void Lectura_argumentos(int argc,char *argv[],short int *diss,unsigned int *memory,char *direVMI);
void chequeo_errores(char top1,char top2,short int cant,char operacion,long int op1,long int op2,MV mv);

int main(int argc,char *argv[])
{

    MV mv;
    unsigned short int diss=0;
    Lectura_argumentos(argc,argv,&diss,&memory,direVMI);
    Inicia_registro(&mv);
    Inicia_memoria(argv[1],&mv,memory);
    if(diss==1)
        Dissasembler_mostrar(mv);
    Lectura(&mv);
}

void Inicia_registro(MV *mv){
        (*mv).TablaRegistros[0]=0;//cs
        (*mv).TablaRegistros[1]=0x00010000;//ds
        (*mv).TablaRegistros[5]=0;//ip
        (*mv).TablaRegistros[8]=0;//cc
        (*mv).TablaRegistros[9]=0;//ac
}

void Inicia_memoria(char *dire,MV *mv,unsigned int memory){
    char version,lec,nombre[5];
    FILE *arch=fopen(dire,"rb");
    int i=0;
    unsigned short int valor=0;
    unsigned char val=0;
    if (arch!=NULL){
        fread(nombre,sizeof(nombre),1,arch);//lee el titulo
        fread(&version,sizeof(char),1,arch);//lee la version
        if (version==2){
            unsigned short int vec[5]={0,0,0,0,0};
            for (int j=0;j<5;j++){
                fread(&val,sizeof(char),1,arch);
                valor+=val;
                valor=(valor<<8);
                fread(&val,sizeof(char),1,arch);
                valor+=val;
                vec[j]=valor;
            }
            SeteoV2(mv,vec,memory);
        }else{
            fread(&val,sizeof(char),1,arch);
            valor+=val;
            valor=(valor<<8);
            fread(&val,sizeof(char),1,arch);
            valor+=val;
            (*mv).TablaDeDatos[0].pos=0;
            (*mv).TablaDeDatos[0].tamano=valor;
            (*mv).TablaDeDatos[1].pos=valor;
            (*mv).TablaDeDatos[1].tamano=sizeof((*mv).TablaMemoria)-valor;
        }
        while (!feof(arch)){
            fread(&lec,sizeof(char),1,arch);
            (*mv).TablaMemoria[i]=lec;//cargo el CS
            i++;
        }
        fclose(arch);
    }else{
        printf("ERROR(0): el archivo no existe");
        exit(-1);
    }
}

void Lectura(MV *mv){
    char top1,top2,operacion;
    short int cant;
    long int op1,op2;
    int corte;
    char dbg;
    while ((*mv).TablaRegistros[5]!=(*mv).TablaDeDatos[1].pos)
    {
        op1=0;
        op2=0;
        operacion=(*mv).TablaMemoria[(*mv).TablaRegistros[5]]&0x0F;
        Cant_op((*mv).TablaMemoria[(*mv).TablaRegistros[5]],&top1,&top2,&cant);
        corte=top1^0x3;//le asigno el opuesto del tipo1 el cual representa la longitud
        if (cant==2){
            for (int i=1;i<=corte;i++){
                (*mv).TablaRegistros[5]++;
                op1+=((*mv).TablaMemoria[(*mv).TablaRegistros[5]])&0x000000FF;
                if (i!=corte)
                    op1=op1<<8;
            }
            corte=top2^0x3;//le asigno el opuesto del tipo2 el cual representa la longitud
            for (int i=1;i<=corte;i++){
                (*mv).TablaRegistros[5]++;
                op2+=((*mv).TablaMemoria[(*mv).TablaRegistros[5]])&0x000000FF;
                if (i!=corte)
                    op2=op2<<8;
            }
        }else{
            if (cant==0){
                op1=operacion;
            }else
                for (int i=1;i<=corte;i++){
                    (*mv).TablaRegistros[5]++;
                    op1+=((*mv).TablaMemoria[(*mv).TablaRegistros[5]])&0x000000FF;
                    if (i!=corte)
                        op1=op1<<8;
                }
        }
        (*mv).TablaRegistros[5]++;// le sumo uno mas al ip antes de que se ejecute la operacion.
        chequeo_errores(top1,top2,cant,operacion,op1,op2,*mv);
        if (cant==0 && op1==0)
            (*mv).TablaRegistros[5]=(*mv).TablaDeDatos[1].pos;
        else{
            if (Toggle_debbug==1){
                scanf("%c",&dbg);
                fflush(stdin);
                if (dbg=='q')
                    Toggle_debbug=0;
                Pre_Funcion(cant,op1,op2,top1,top2,operacion,mv);
                IMG_debug(*mv);
            }else
                Pre_Funcion(cant,op1,op2,top1,top2,operacion,mv);//con esto arracamos la parte de hacer la funcion
        }
    }
}
void Cant_op(char instruccion,char *top1,char *top2,short int *cant){
    *top1=(instruccion>>6)&0x03;//haciendo el corrimiento me quedaria 00XX donde XX es el valor del operando
    *top2=(instruccion>>4)&0x03;//haciendo la operacion tambien quedaria 00XX.
    if (*top1==3)
        *cant=0;
    else
        if (*top2==3)
            *cant=1;
        else
            *cant=2;
}
void Lectura_argumentos(int argc,char *argv[],short int *diss,unsigned int *memory,char *direVMI){
    for (int i=2;i<argc;i++){
        if (strcmp(argv[i],"-d")==0)
            (*diss)=1;
        else{
            if (argv[i][strlen(argv[i])-1]=='i'){
                strcpy(direVMI,argv[i]);
            }else{
                (*memory)=0;
                for (int j=0;j<strlen(argv[i]);j++){
                    (*memory)=(*memory)*10;
                    (*memory)+=argv[i][j]-48;
                }
            }
        }
    }
}
void SeteoV2(MV *mv,unsigned short int vec[],unsigned int memory){
    unsigned short int total=0,aux=0;
    int c=0;
    for (int i=0;i<=4;i++){
        if (vec[i]>0){
            total+=vec[i];
            if (total>memory){
                printf("ERROR(4): los segmentos ocupan mas de la memoria disponible");
                exit(-1);
            }
            if (i==1)
                (*mv).TablaRegistros[2]==c<<16;
            if (i==2)
                (*mv).TablaRegistros[1]==c<<16;
            if (i>2)
                (*mv).TablaRegistros[i]==c<<16;
            if (i==4)
                (*mv).TablaRegistros[6]=aux+vec[i];
            (*mv).TablaDeDatos[c].pos=aux;
            (*mv).TablaDeDatos[c].tamano=vec[i];
            aux=vec[i];
            c++;
        }
    }
}
void IMG_debug(MV mv){
    FILE *arch=fopen(direVMI,"wb");
    if (arch==NULL){
        printf("ERROR no se pudo abrir el archivo binario");
        return 0;
    }
    char pal[5]="VMI23";
    //header
    fwrite(pal,sizeof(pal),1,arch);
    fwrite(1,sizeof(char),1,arch);
    fwrite(16384,sizeof(int),1,arch);
    //registros
    for (int i=0;i<=15;i++){
        fwrite(mv.TablaRegistros[i],sizeof(long int),1,arch);
    }
    //TDS
    long int aux=0;
    for (int i=0;i<=7;i++){
        aux=0;
        aux=mv.TablaDeDatos[i].tamano;
        aux=aux<<16;
        aux+=mv.TablaDeDatos[i].pos;
        fwrite(aux,sizeof(long int),1,arch);
    }
    //memoria dinamica
    fwrite(memory,sizeof(memory),1,arch);
    fclose(arch);
}
void chequeo_errores(char top1,char top2,short int cant,char operacion,long int op1,long int op2,MV mv){
    if (top1==0){
        Integridad_op(op1,mv);
    if (top2==0)
        Integridad_op(op2,mv);
    }
    //RE HACER LA FUNCION INTEGRIDAD OP!!!!!!!!
    if (cant==2 && (operacion<0 || operacion>12)){
        printf("ERROR(1): el codigo de operacion no existe");
        exit(-1);
    }
    if (cant==1 && (operacion<0 || operacion>14)){
        printf("ERROR(1): el codigo de operacion no existe");
        exit(-1);
    }
    if (cant==0 && (operacion<0 || operacion>1)){
        printf("ERROR(1): el codigo de operacion no existe");
        exit(-1);
    }
}

