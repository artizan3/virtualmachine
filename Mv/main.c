#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"
#include "dissasembler.h"

int main(int argc,char *argv[])
{
    TDD TablaDeDatos[1];//tabla de datos, pos 1 CS, pos 2 DS.
    long int TablaRegistros[15];//long int ya que son 4 bytes.
    char TablaMemoria[16384]; //TablaMemoria es la tabla de memoria donde las primeras n posiciones pertencen al CS.

    Inicia_registro(TablaRegistros);//inicializo la tabla de registros.
    Inicia_memoria(argv[1],TablaMemoria,TablaDeDatos);//inicio la tabla de memoria y la de datos
    //if(argv[2]!=NULL && strcmp(argv[2],"-d")==0)
    //  Dissasembler_mostrar(TablaMemoria,TablaRegistros,TablaDeDatos);
    Lectura(TablaMemoria,TablaRegistros,TablaDeDatos);
}

void Inicia_registro(long int TablaRegistros[]){
        TablaRegistros[0]=0;//cs
        TablaRegistros[1]=0x00010000;//ds
        TablaRegistros[5]=0;//ip
        TablaRegistros[8]=0;//cc
        TablaRegistros[9]=0;//ac
}

void Inicia_memoria(char *dire,char *TablaMemoria,TDD TablaDeDatos[]){
    char lec,nombre[6];
    FILE *arch=fopen(dire,"rb");
    int i=0,valor=0;
    if (arch!=NULL){
        fread(nombre,sizeof(char),5,arch);//lee el titulo
        fread(&lec,sizeof(char),1,arch);//lee la version
        fread(&lec,sizeof(char),1,arch);//leeo el 1er byte del tamaño
        valor+=lec;
        fread(&lec,sizeof(char),1,arch);//leo el segundo
        valor=(valor<<8);//como se lee el little indian tengo q haer corrimineto
        valor+=lec;
        TablaDeDatos[0].pos=0;
        TablaDeDatos[0].tamano=valor;
        TablaDeDatos[1].pos=valor;
        TablaDeDatos[1].tamano=sizeof(TablaMemoria)-valor;
        while (!feof(arch)){
            fread(&lec,sizeof(char),1,arch);
            TablaMemoria[i]=lec;
            i++;
        }
        fclose(arch);
    }else{
        printf("ERROR(0): el archivo no existe");
        exit(-1);
    }
}

void Lectura(char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char top1,top2,operacion;
    short int cant;
    long int op1,op2;
    int corte;
    while (TablaRegistros[5]!=TablaDeDatos[1].pos)
    {
        op1=0;
        op2=0;
        operacion=TablaMemoria[TablaRegistros[5]]&0x0F;
        Cant_op(TablaMemoria[TablaRegistros[5]],&top1,&top2,&cant);
        corte=top1^0x3;//le asigno el opuesto del tipo1 el cual representa la longitud
        if (cant==2){
            for (int i=1;i<=corte;i++){
                TablaRegistros[5]++;
                op1+=(TablaMemoria[TablaRegistros[5]])&0x000000FF;
                if (i!=corte)
                    op1=op1<<8;
            }
            corte=top2^0x3;//le asigno el opuesto del tipo2 el cual representa la longitud
            for (int i=1;i<=corte;i++){
                TablaRegistros[5]++;
                op2+=(TablaMemoria[TablaRegistros[5]])&0x000000FF;
                if (i!=corte)
                    op2=op2<<8;
            }
        }else{
            for (int i=1;i<=corte;i++){
                TablaRegistros[5]++;
                op1+=(TablaMemoria[TablaRegistros[5]])&0x000000FF;
                if (i!=corte)
                    op1=op1<<8;
            }
        }
        TablaRegistros[5]++;// le sumo uno mas al ip antes de que se ejecute la operacion.
        Pre_Funcion(cant,op1,op2,top1,top2,operacion,TablaMemoria,TablaRegistros,TablaDeDatos);//con esto arracamos la parte de hacer la funcion
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

