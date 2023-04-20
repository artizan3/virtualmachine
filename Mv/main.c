#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"
#include "dissasembler.h"

void Inicia_memoria(char *dire,MV *mv);
void Inicia_registro(MV *mv);
void Cant_op(char instruccion,char *top1,char *top2,short int *cant);
void Lectura(MV *mv);

int main(int argc,char *argv[])
{

    MV mv;
    Inicia_registro(&mv);
    Inicia_memoria(argv[1],&mv);
    if(argv[2]!=NULL && strcmp(argv[2],"-d")==0)
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

void Inicia_memoria(char *dire,MV *mv){
    char version,lec,nombre[5];
    FILE *arch=fopen(dire,"rb");
    int i=0;
    unsigned short int valor=0;
    char val=0;
    if (arch!=NULL){
        fread(nombre,sizeof(nombre),1,arch);//lee el titulo
        fread(&version,sizeof(char),1,arch);//lee la version
        fread(&val,sizeof(char),1,arch);
        valor+=val;
        valor=(valor<<8);
        fread(&val,sizeof(char),1,arch);
        valor+=val;
        (*mv).TablaDeDatos[0].pos=0;
        (*mv).TablaDeDatos[0].tamano=valor;
        (*mv).TablaDeDatos[1].pos=valor;
        (*mv).TablaDeDatos[1].tamano=sizeof((*mv).TablaMemoria)-valor;
        if (strcmp(nombre,"VMX23")==0 && version==1)
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
            for (int i=1;i<=corte;i++){
                (*mv).TablaRegistros[5]++;
                op1+=((*mv).TablaMemoria[(*mv).TablaRegistros[5]])&0x000000FF;
                if (i!=corte)
                    op1=op1<<8;
            }
        }
        (*mv).TablaRegistros[5]++;// le sumo uno mas al ip antes de que se ejecute la operacion.
        if (top1==0){
            Integridad_op(op1,*mv);
            if (top2==0)
                Integridad_op(op2,*mv);
        }
        if (operacion<0 || operacion>12){
            printf("ERROR(1): el codigo de operacion no existe");
            exit(-1);
        }
        if(cant!=0)
            Pre_Funcion(cant,op1,op2,top1,top2,operacion,mv);//con esto arracamos la parte de hacer la funcion
        else
            (*mv).TablaRegistros[5]=(*mv).TablaDeDatos[1].pos;
       // mue(*mv);
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

